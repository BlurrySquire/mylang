use std::{error, fmt};

use crate::lexer::Token;

#[derive(Debug)]
pub struct ParserError {
	pub message: String
}

impl ParserError {
	pub fn new(message: &str) -> Self {
		let message: String = String::from(message);

		Self { message }
	}
}

impl error::Error for ParserError {}
impl fmt::Display for ParserError {
	fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
		write!(f, "ParserError: {}", self.message)
	}
}

#[allow(dead_code)]
#[derive(Debug)]
pub enum Expr {
	StringLiteral(String),
	IntegerLiteral(i64),
	FunctionCall{ identifier: String, args: Vec<Expr> },
}

#[allow(dead_code)]
#[derive(Debug)]
pub enum Stmt {
	Expr(Expr),
}

#[derive(Debug)]
pub struct ProgramAst {
	pub statements: Vec<Stmt>
}

impl ProgramAst {
	pub fn new() -> Self {
		let statements: Vec<Stmt> = Vec::new();

		Self { statements }
	}
}

pub struct Parser {
	source: Vec<Token>,
	position: usize
}

impl Parser {
	pub fn new(source: Vec<Token>) -> Self {
		let position: usize = 0;

		Self { source, position }
	}

	fn peek(&self, offset: usize) -> Option<&Token> {
		self.source.get(self.position + offset)
	}

	fn consume(&mut self) -> Option<Token> {
		let old: usize = self.position;
		self.position += 1;

		self.source.get(old).cloned()
	}

	fn parse_expression(&mut self) -> Result<Expr, ParserError> {
		if self.peek(0).is_some() {
			match self.peek(0).unwrap() {
				Token::Identifier(_) => {
					let identifier = match self.consume() {
						Some(Token::Identifier(name)) => name,
						_ => unreachable!()
					};

					if matches!(self.peek(0), Some(Token::LeftParen)) {
						self.consume();

						let mut args: Vec<Expr> = Vec::new();
						while self.peek(0).is_some() && !matches!(self.peek(0), Some(Token::RightParen)) {
							let expression: Expr = self.parse_expression()?;
							args.push(expression);
						}

						if matches!(self.peek(0), Some(Token::RightParen)) {
							self.consume();
							Ok(Expr::FunctionCall { identifier, args })
						}
						else {
							Err(ParserError::new("Expected ')' (RightParen) at the end of a function call."))
						}
					}
					else {
						Err(ParserError::new("Expected '(' (LeftParen) after Identifier."))
					}
				},

				Token::StringLiteral(_) => {
					let value: String = match self.consume() {
						Some(Token::StringLiteral(value)) => value,
						_ => unreachable!()
					};

					Ok(Expr::StringLiteral(value))
				},

				Token::IntegerLiteral(_) => {
					let value: i64 = match self.consume() {
						Some(Token::IntegerLiteral(value)) => value,
						_ => unreachable!()
					};

					Ok(Expr::IntegerLiteral(value))
				}

				_ => Err(ParserError::new("Cannot parse expression."))
			}
		}
		else {
			Err(ParserError::new("Cannot parse expression."))
		}
	}

	fn parse_statement(&mut self) -> Result<Stmt, ParserError> {
		match self.peek(0).unwrap() {
			_ => {
				let statement: Stmt = Stmt::Expr(self.parse_expression()?);
				
				if matches!(self.peek(0), Some(Token::Semicolon)) {
					self.consume();
					Ok(statement)
				}
				else {
					Err(ParserError::new("Expected ';' (Semicolon) at the end of a statement."))
				}
			}
		}
	}

	pub fn parse(&mut self) -> Result<ProgramAst, ParserError> {
		let mut program: ProgramAst = ProgramAst::new();
		
		while self.peek(0).is_some() {
			let statement: Stmt = self.parse_statement()?;
			program.statements.push(statement);
		}

		Ok(program)
	}
}