use std::{env, fs};

mod lexer;
use lexer::*;

static CODE_TO_PARSE: &str = 
"
print(\"Hello, world!\");
exit(0);
";

fn parse_args(args: &Vec<String>, input_file: &mut String, output_file: &mut String) {
    if args.len() < 3 {
        panic!("Not enough arguments. ./mylang <input-file> <output-file>");
    }

    if args.len() > 3 {
        println!("Warning: More than 2 arguments. Ignoring extra arguments.");
    }

    *input_file = args[1].clone();
    *output_file = args[2].clone();
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let mut input_file: String = String::new();
    let mut output_file: String = String::new();

    parse_args(&args, &mut input_file, &mut output_file);
    
    println!("Input: {input_file}");
    println!("Output: {output_file}");

    let input_source: String = fs::read_to_string(input_file)
        .expect("Failed to open input file.");

    let mut lexer: Lexer = Lexer::new(input_source);

    let tokens: Vec<Token> = match lexer.tokenise() {
        Ok(tokens) => tokens,
        Err(error) => {
            panic!("{error}");
        }
    };

    println!("{:?}", tokens);
}