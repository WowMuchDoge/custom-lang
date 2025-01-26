#include "compiler/Parser.hpp"

#include <cstdarg>

#include "error/CompilerError.hpp"

Token Parser::peek() {
	return m_next;
}

Token Parser::advance() {
	m_prev = m_next;
	m_next = m_scanner.ScanToken();
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
	consume(TokenType::SEMICOLON, "Expected ';' after statement");
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
	}

	throw makeCompilerError("Unexpected token '" + peek().GetLiteral() + "'.");
}

StmtPtr Parser::variableDeclaration() {
	Token var = consume(TokenType::IDENTIFIER, "Unexpected keyword '" + peek().GetLiteral() + "' in variable declaration.");

	m_scope.Push(var.GetLiteral());

	ExprPtr expr(new Primary(Value()));

	if (peek().GetType() == TokenType::EQUAL) {
		advance();

		expr = expression();
	}

	consumeSemicolon();

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

ExprPtr Parser::expression() {
	return logical();
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

	while ((op = match(4, TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL, 
			TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL)) != TokenType::UNKNOWN) {
		
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
	ExprPtr left = primary();
	TokenType op;

	while ((op = match(3, TokenType::STAR, TokenType::SLASH, TokenType::MOD))
			!= TokenType::UNKNOWN) {
		
		ExprPtr right = primary();

		left = ExprPtr(new Binary(op, right, left));
	}

	return left;
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
