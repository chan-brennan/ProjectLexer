# ProjectLexer

This project was coded in Visual Studio Code. Enclosed within are the files for Project 1 of CSE 340, copyright claim is to Riza Bazzi for the initial code base. Lexer.cc is where my work is located.
Project 1 is the first of a set of three projects, each of which advances and builds upon the previous concepts examined within each project.
This project implements a lexical analyzer (lexer) for a simplified programming language. The lexer reads input code and breaks it down into tokens, which are the building blocks for parsing and further compilation. The tokens represent language constructs like keywords, identifiers, operators, and delimiters.

# Features:
Token Identification: The lexer recognizes different tokens such as keywords, identifiers, numbers, operators, and delimiters.
Keyword Recognition: It identifies reserved keywords like TYPE, VAR, REAL, INT, etc.
Number Parsing: Supports parsing both integer and real numbers.
Error Handling: Detects invalid tokens and returns an ERROR token type.
Whitespace Handling: Skips whitespace characters while processing input.

# Key Components
Token Class: Defines the structure of a token with attributes like lexeme, token_type, and line_no.
LexicalAnalyzer Class: Contains the core logic for tokenization. Key methods include:
GetToken(): Retrieves the next token from input.
UngetToken(): Pushes back tokens to support peeking behavior.
ScanNumber(): Handles numeric literals.
ScanIdOrKeyword(): Identifies keywords and identifiers.
SkipSpace(): Skips whitespace characters.

# Token Types
The lexer supports the following token types:
- Keywords: TYPE, VAR, REAL, INT, BOOLEAN, STRING, LONG, WHILE, DO, SWITCH, CASE
- Operators: +, *, /, =, <, >, <=, >=, <> (not equal)
- Delimiters: ,, :, ;, {, }, (, )
- Identifiers and Numbers: Generic identifiers (like variable names) and numeric literals.
- Special Tokens: END_OF_FILE, ERROR
# How It Works
Input Handling: The lexer reads input character-by-character and builds tokens based on patterns (e.g., numbers, keywords, etc.).
Tokenization Logic: Depending on the character encountered, the lexer decides whether it's part of a number, identifier, keyword, or an operator.
Ungetting Tokens: Tokens can be "unread" to allow for lookahead or backtracking during parsing.
