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

std::vector<std::shared_ptr<Stmt>> Parser::GetAst() {
	std::vector<std::shared_ptr<Stmt>> program;
	
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

std::shared_ptr<Stmt> Parser::statement() {
	if (match(TokenType::VAR)) {
		return variableDeclaration();
	} else if (match(TokenType::PRINT)) {
		return printStatement();
	} else if (match(TokenType::LEFT_BRACE)) {
		return blockStatement();
	}

	throw makeCompilerError("Unexpected token '" + peek().GetLiteral() + "'.");
}

std::shared_ptr<Stmt> Parser::variableDeclaration() {
	Token var = consume(TokenType::IDENTIFIER, "Unexpected keyword '" + peek().GetLiteral() + "' in variable declaration.");

	m_scope.Push(var.GetLiteral());

	std::shared_ptr<Expr> expr(new Primary(Value()));

	if (peek().GetType() == TokenType::EQUAL) {
		advance();

		expr = expression();
	}

	consumeSemicolon();

	return std::shared_ptr<Stmt>(new VariableDeclaration(expr));
}

std::shared_ptr<Stmt> Parser::printStatement() {
	consume(TokenType::LEFT_PAREN, "Expected '(' after print statement.");

	std::shared_ptr<Expr> expr = expression();

	consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");

	consumeSemicolon();

	return std::shared_ptr<Stmt>(new PrintStatement(expr));
}

std::shared_ptr<Stmt> Parser::blockStatement() {
	int startLine = m_scanner.GetLine();
	
	std::vector<std::shared_ptr<Stmt>> statements;	

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

	return std::shared_ptr<Stmt>(new BlockStatement(statements));
}

std::shared_ptr<Expr> Parser::expression() {
	return logical();
}

std::shared_ptr<Expr> Parser::logical() {
	std::shared_ptr<Expr> left = comparison();
	TokenType op;

	while ((op = match(2, TokenType::AND, TokenType::OR))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = comparison();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
}

std::shared_ptr<Expr> Parser::comparison() {
	std::shared_ptr<Expr> left = term();
	TokenType op;

	while ((op = match(4, TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL, 
			TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL)) != TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = term();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
}

std::shared_ptr<Expr> Parser::term() {
	std::shared_ptr<Expr> left = factor();
	TokenType op;

	while ((op = match(2, TokenType::PLUS, TokenType::MINUS))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = factor();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
}

std::shared_ptr<Expr> Parser::factor() {
	std::shared_ptr<Expr> left = primary();
	TokenType op;

	while ((op = match(3, TokenType::STAR, TokenType::SLASH, TokenType::MOD))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = primary();

		left = std::shared_ptr<Expr>(new Binary(op, right, left));
	}

	return left;
}

std::shared_ptr<Expr> Parser::primary() {
	if (match(TokenType::TRUE)) return std::shared_ptr<Expr>(new Primary(true));
	if (match(TokenType::FALSE)) return std::shared_ptr<Expr>(new Primary(false));

	// Variable resolution
	if (peek().GetType() == TokenType::IDENTIFIER) {
		std::string name = advance().GetLiteral();
		int id;

		if ((id = m_scope.Resolve(name)) == -1) {
			throw makeCompilerError("Unkown identifier '" + name + "'."); 
		}

		return std::shared_ptr<Expr>(new Identifier(id));
	}

	// Value constructor with no args constructs a nil value
	if (match(TokenType::NIL)) return std::shared_ptr<Expr>(new Primary(Value()));

	if (match(TokenType::LEFT_PAREN)) {
		std::shared_ptr<Expr> expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after group.");

		return std::shared_ptr<Expr>(new Grouping(expr));
	}

	if (peek().GetType() == TokenType::NUMBER) {
		Token number = advance();
	
		// Implicit constructor, constructor to Value is being called
		// with a double
		return std::shared_ptr<Expr>(new Primary(number.GetValue()));
	}

	if (peek().GetType() == TokenType::STRING) {
		Token str = advance();

		return std::shared_ptr<Expr>(new Primary(str.GetLiteral()));
	}

	throw makeCompilerError("Unknown primary '" + peek().GetLiteral() + "'."); 
}
