use std::{error, fmt};

#[allow(dead_code)]
#[derive(Debug, Clone)]
pub enum Token {	
	Semicolon,
	Comma,
    LeftParen, RightParen,
	LeftBrace, RightBrace,

	StringLiteral(String),
	IntegerLiteral(i64),
	Identifier(String),
}

#[derive(Debug)]
pub struct LexerError {
	pub message: String
}

impl LexerError {
	pub fn new(message: &str) -> Self {
		let message: String = String::from(message);
	
		Self { message }
	}
}

impl error::Error for LexerError {}
impl fmt::Display for LexerError {
	fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
		write!(f, "LexerError: {}", self.message)
	}
}

pub struct Lexer {
	source: Vec<char>,
	position: usize,
}

impl Lexer {
	pub fn new(source: String) -> Self {
		let position: usize = 0;
		let source: Vec<char> = source.chars().collect();

		Self { source, position }
 	}

	fn peek(&self, offset: usize) -> Option<char> {
		self.source.get(self.position + offset).copied()
	}

	fn consume(&mut self) -> Option<char> {
		let old: usize = self.position;
		self.position += 1;

		self.source.get(old).copied()
	}

	pub fn tokenise(&mut self) -> Result<Vec<Token>, LexerError> {
		let mut tokens: Vec<Token> = Vec::new();

		while self.peek(0).is_some() {
			if matches!(self.peek(0), Some(';')) {
				self.consume();
				tokens.push(Token::Semicolon);
			}
			else if matches!(self.peek(0), Some(',')) {
				self.consume();
				tokens.push(Token::Comma);
			}
			else if matches!(self.peek(0), Some('(')) {
				self.consume();
				tokens.push(Token::LeftParen);
			}
			else if matches!(self.peek(0), Some(')')) {
				self.consume();
				tokens.push(Token::RightParen);
			}
			else if matches!(self.peek(0), Some('{')) {
				self.consume();
				tokens.push(Token::LeftBrace);
			}
			else if matches!(self.peek(0), Some('}')) {
				self.consume();
				tokens.push(Token::RightBrace);
			}
			else if self.peek(0).unwrap().is_whitespace() {
				self.consume();
			}
			else if matches!(self.peek(0), Some('\"')) {
				let mut buffer: Vec<char> = Vec::new();
				self.consume();

				while self.peek(0).is_some() && self.peek(0).unwrap() != '\"' {
					buffer.push(self.consume().unwrap());
				}

				if self.peek(0).is_none() {
					return Err(LexerError::new("Expected '\"' to end string."))
				}

				self.consume();
				tokens.push(Token::StringLiteral(buffer.iter().collect()));
			}
			else if self.peek(0).unwrap().is_digit(10) {
				let mut buffer: Vec<char> = Vec::new();
				
				while self.peek(0).is_some() && self.peek(0).unwrap().is_digit(10) {
					buffer.push(self.consume().unwrap());
				}

				buffer.reverse();
				let mut integer: i64 = 0;
				let mut scale: i64 = 1;
				for digit in buffer {
					integer += digit.to_digit(10).unwrap() as i64 * scale;
					scale *= 10;
				}

				tokens.push(Token::IntegerLiteral(integer));
			}
			else if self.peek(0).unwrap().is_alphanumeric() {
				let mut buffer: Vec<char> = Vec::new();

				while self.peek(0).is_some() && (self.peek(0).unwrap().is_alphanumeric() || self.peek(0).unwrap() == '_') {
					buffer.push(self.consume().unwrap());
				}

				tokens.push(Token::Identifier(buffer.iter().collect()));
			}
			else {
				return Err(LexerError::new("Unknown character."))
			}
		}

		Ok(tokens)
	}
}