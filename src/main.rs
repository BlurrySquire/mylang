mod lexer;
use lexer::*;

static CODE_TO_PARSE: &str = 
"
print(\"Hello, world!\");
exit(0);
";

fn main() {
    let mut lexer: Lexer = Lexer::new(String::from(CODE_TO_PARSE));

    let tokens: Vec<Token> = match lexer.tokenise() {
        Ok(tokens) => tokens,
        Err(error) => {
            panic!("{error}");
        }
    };

    println!("{:?}", tokens);
}