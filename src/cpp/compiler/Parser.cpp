#include "compiler/Parser.hpp"

#include <cstdarg>

#include "error/CompilerError.hpp"

Token Parser::peek() {
	return m_next;
}

Token Parser::peekNext() {
	return m_twoNext;
}

Token Parser::advance() {
	m_prev = m_next;
	m_next = m_twoNext;
	m_twoNext = m_scanner.ScanToken();
	return m_prev;
}

bool Parser::atEnd() {
	return m_next.GetType() == TokenType::END;
}

Token Parser::consume(TokenType type, std::string msg) {
	if (peek().GetType() != type)
		throw makeCompilerError(msg);
	
	return advance();
}

void Parser::consumeSemicolon() {
	consume(TokenType::SEMICOLON, "Expected ';' after statement.");
}

TokenType Parser::match(int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		TokenType type = va_arg(args, TokenType);
		
		if (peek().GetType() == type) {
			return advance().GetType();
		}
	}

	return TokenType::UNKNOWN;
}

bool Parser::match(TokenType type) {
	if (peek().GetType() == type) {
		advance();
		return true;
	}

	return false;
}

bool Parser::matchTwo(TokenType t1, TokenType t2) {
	if (peek().GetType() == t1 && peekNext().GetType() == t2) {
		advance();
		advance();
		return true;
	}

	return false;
}

CompilerError* Parser::makeCompilerError(std::string message) {
	m_error = CompilerError(message, m_scanner.GetLine());
	return &m_error;
}

CompilerError* Parser::makeCompilerError(std::string message, int line) {
	m_error = CompilerError(message, line);
	return &m_error;
}

void Parser::skipStatement() {
	while (!atEnd() && advance().GetType() != TokenType::SEMICOLON);
}

std::vector<StmtPtr> Parser::GetAst() {
	std::vector<StmtPtr> program;
	
	while (!atEnd()) {
		try {
			program.push_back(statement());
		} catch (CompilerError* e) {
			e->Print();
			skipStatement();
		}
	}

	return program;
}

StmtPtr Parser::statement() {
	if (match(TokenType::VAR)) {
		return variableDeclaration();
} else if (match(TokenType::PRINT)) {
		return printStatement();
	} else if (match(TokenType::LEFT_BRACE)) {
		return blockStatement();
	} else if (match(TokenType::IF)) {
		return ifStatement();
	} else if (match(TokenType::WHILE)) {
		return whileStatement();
	} else if (match(TokenType::FOR)) {
		return forStatement();
	} else {
		return expressionStatement();
	}
}

StmtPtr Parser::variableDeclaration(bool requireSemicolon) {
	Token var = consume(TokenType::IDENTIFIER, "Unexpected keyword '" + peek().GetLiteral() + "' in variable declaration.");

	m_scope.Push(var.GetLiteral());

	ExprPtr expr(new Primary(Value()));

	if (peek().GetType() == TokenType::EQUAL) {
		advance();

		expr = expression();
	}

	if (requireSemicolon) consumeSemicolon();

	return StmtPtr(new VariableDeclaration(expr));
}

StmtPtr Parser::printStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after print statement.");

	ExprPtr expr = expression();

	consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");

	consumeSemicolon();

	return StmtPtr(new PrintStatement(expr));
}

StmtPtr Parser::blockStatement() {
	int startLine = m_scanner.GetLine();
	
	std::vector<StmtPtr> statements;	

	while (peek().GetType() != TokenType::RIGHT_BRACE) {
		if (atEnd()) {
			throw makeCompilerError("Expected '}' after block.", startLine);
		}
	
		try {
			statements.push_back(statement());
		} catch (CompilerError *e) {
			e->Print();
			skipStatement();
		}
	}
	
	consume(TokenType::RIGHT_BRACE, "Expected '}' after block."); 

	return StmtPtr(new BlockStatement(statements));
}

StmtPtr Parser::ifStatement() {
	std::vector<IfObject> ifs;

	consume(TokenType::LEFT_PAREN, "Expected '(' after if statement.");

	ExprPtr expr = expression();
	
	consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");

	ifs.push_back(IfObject(expr, statement()));

	while (matchTwo(TokenType::ELSE, TokenType::IF)) {
		consume(TokenType::LEFT_PAREN, "Expected '(' after if statement.");

		expr = expression();
		
		consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
		
		ifs.push_back(IfObject(expr, statement()));
	}

	if (match(TokenType::ELSE)) {
		ifs.push_back(IfObject(std::nullopt, statement()));
	}

	return StmtPtr(new IfStatement(ifs));
}

StmtPtr Parser::whileStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after while statement.");

	ExprPtr expr = expression();

	consume(TokenType::RIGHT_PAREN, "Expected ')' after while expression.");

	StmtPtr stmt = statement();

	return StmtPtr(new WhileStatement(expr, stmt));
}

StmtPtr Parser::forStatement() {
	std::vector<StmtPtr> outer;
	ExprPtr condition;
	std::vector<StmtPtr> block;

	consume(TokenType::LEFT_PAREN, "Expected '(' after for statement.");

	std::vector<StmtPtr> outerBlock;

	while (true) {
		if (match(TokenType::VAR)) {
			outer.push_back(variableDeclaration(false));
		} else {
			outer.push_back(expressionStatement(false));
		}

		if (!match(TokenType::COMMA)) break;
	}

	consumeSemicolon();

	condition = expression();

	consumeSemicolon();

	while (true) {
		block.push_back(expressionStatement(false));

		if (!match(TokenType::COMMA)) break;
	}

	consume(TokenType::RIGHT_PAREN, "Expected ')' after for statement.");

	StmtPtr stmt = statement();

	block.insert(block.begin(), stmt);
	
	StmtPtr whileStmt(new WhileStatement(condition, stmt));

	outer.push_back(whileStmt);

	return StmtPtr(new BlockStatement(outer));
}

StmtPtr Parser::expressionStatement(bool requireSemicolon) {
	ExprPtr expr = expression();
	if (requireSemicolon) consumeSemicolon();

	return StmtPtr(new ExpressionStatement(expr));
}

ExprPtr Parser::expression() {
	return assignment();
}

ExprPtr Parser::assignment() {
	ExprPtr left = logical();

	// Assignment is right associative, so a = b = c should
	// be a = (b = c); this differs from addition which evaluates
	// like a + b + c is (a + b) + c. This is why we have to
	// make this recursive call
	if (match(TokenType::EQUAL)) {
		ExprPtr right = assignment();
		return ExprPtr(new Binary(TokenType::EQUAL, left, right));
	}

	return left;
}

ExprPtr Parser::logical() {
	ExprPtr left = comparison();
	TokenType op;

	while ((op = match(2, TokenType::AND, TokenType::OR))
			!= TokenType::UNKNOWN) {
		
		ExprPtr right = comparison();

		left = ExprPtr(new Binary(op, left, right));
	}

	return left;
}

ExprPtr Parser::comparison() {
	ExprPtr left = term();
	TokenType op;

	while ((op = match(6, TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL, TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL, 
					TokenType::GREATER, TokenType::LESS)) != TokenType::UNKNOWN) {
		
		ExprPtr right = term();

		left = ExprPtr(new Binary(op, left, right));
	}

	return left;
}

ExprPtr Parser::term() {
	ExprPtr left = factor();
	TokenType op;

	while ((op = match(2, TokenType::PLUS, TokenType::MINUS))
			!= TokenType::UNKNOWN) {
		
		ExprPtr right = factor();

		left = ExprPtr(new Binary(op, left, right));
	}

	return left;
}

ExprPtr Parser::factor() {
	ExprPtr left = unary();
	TokenType op;

	while ((op = match(3, TokenType::STAR, TokenType::SLASH, TokenType::MOD))
			!= TokenType::UNKNOWN) {
		
		ExprPtr right = unary();

		left = ExprPtr(new Binary(op, right, left));
	}

	return left;
}

ExprPtr Parser::unary() {
	TokenType op;

	if ((op = match(2, TokenType::BANG, TokenType::MINUS)) != TokenType::UNKNOWN) {
		ExprPtr right = primary();

		return ExprPtr(new Unary(op, right));
	}

	return primary();
}

ExprPtr Parser::primary() {
	if (match(TokenType::TRUE)) return ExprPtr(new Primary(true));
	if (match(TokenType::FALSE)) return ExprPtr(new Primary(false));

	// Variable resolution
	if (peek().GetType() == TokenType::IDENTIFIER) {
		std::string name = advance().GetLiteral();
		int id;

		if ((id = m_scope.Resolve(name)) == -1) {
			throw makeCompilerError("Unkown identifier '" + name + "'."); 
		}

		return ExprPtr(new Identifier(id));
	}

	// Value constructor with no args constructs a nil value
	if (match(TokenType::NIL)) return ExprPtr(new Primary(Value()));

	if (match(TokenType::LEFT_PAREN)) {
		ExprPtr expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after group.");

		return ExprPtr(new Grouping(expr));
	}

	if (peek().GetType() == TokenType::NUMBER) {
		Token number = advance();
	
		// Implicit constructor, constructor to Value is being called
		// with a double
		return ExprPtr(new Primary(number.GetValue()));
	}

	if (peek().GetType() == TokenType::STRING) {
		Token str = advance();

		return ExprPtr(new Primary(str.GetLiteral()));
	}

	throw makeCompilerError("Unknown primary '" + peek().GetLiteral() + "'."); 
}
