/*
 * Copyright (C) Brennan Chan
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM","BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char current_char, next_char1, next_char2, next_char3;
    int has_first_zero = 0;
    int base_eight = 0;
    int has_base = 0;
    char number_table[128];
    string temp2;
    input.GetChar(current_char);
    if (isdigit(current_char)) {
        if (current_char == '0') {
            tmp.lexeme = "0";
            has_first_zero = 1;
        } else {
            tmp.lexeme = "";
            while ((!input.EndOfInput() && isdigit(current_char))) {
                tmp.lexeme += current_char;
                input.GetChar(current_char);
                if (current_char == '8' || current_char == '9') {
                    base_eight = 1;
                }
                if (current_char == '0') {
                    has_base = 1;
                }
            }
            if (!input.EndOfInput()) {
                input.UngetChar(current_char);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        input.GetChar(next_char1);
        if (next_char1 == 'x') {
            input.GetChar(next_char2);
            if (next_char2 == ' ') {
                input.UngetChar(next_char2);
            }//
            if (next_char2 == '0') {
                input.GetChar(next_char3);
                if (next_char3 == '8' && base_eight == 0) {
                    tmp.lexeme = tmp.lexeme + next_char1 + next_char2 + next_char3;
                    tmp.token_type = BASE08NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } else {
                    input.UngetChar(next_char3);
                }
                input.UngetChar(next_char2);
            } else if (next_char2 == '1') {
                input.GetChar(next_char3);
                if (next_char3 == '6') {
                    tmp.lexeme = tmp.lexeme + next_char1 + next_char2 + next_char3;
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } else {
                    input.UngetChar(next_char3);
                }
                input.UngetChar(next_char2);
            }
            input.UngetChar(next_char1);
        } else if (next_char1 == '.') {
            int count1 = 0;
            int count2 = 0;
            input.GetChar(next_char2);
            if (isdigit(next_char2)) {
                while (!input.EndOfInput() && isdigit(next_char2)) {
                    count1++;
                    if (next_char2 == '0') {
                        count2++;
                    }
                    temp2 += next_char2;
                    input.GetChar(next_char2);
                }
                if (!input.EndOfInput()) {
                    input.UngetChar(next_char2);
                }
                if (count1 == count2 && has_first_zero == 1) {
                    input.UngetString(temp2);
                } else {
                    input.UngetString(temp2);
                    input.GetChar(next_char2);
                    tmp.lexeme += '.';
                    while (!input.EndOfInput() && (isdigit(next_char2))) {
                        tmp.lexeme += next_char2;
                        input.GetChar(next_char2);
                    }
                    if (!input.EndOfInput()) {
                        input.UngetChar(next_char2);
                    }
                    tmp.token_type = REALNUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
            } else {
                input.UngetChar(next_char2);
            }
            input.UngetChar(next_char1);
        } else if (next_char1 == 'A' || next_char1 == 'B' || next_char1 == 'C' || next_char1 == 'D' || next_char1 == 'E' || next_char1 == 'F') {
            int count = 0;
            int count2 = 0;
            input.UngetChar(next_char1);
            input.GetChar(number_table[count]);
            while (isdigit(number_table[count]) || (!input.EndOfInput() && number_table[count] == 'A' || number_table[count] == 'B' || number_table[count] == 'C' || number_table[count] == 'D' || number_table[count] == 'E' || number_table[count] == 'F')) {
                count++;
                count2++;
                input.GetChar(number_table[count]);
            }
            if (number_table[count] == 'x') {
                input.GetChar(number_table[count + 1]);
                if (number_table[count + 1] == '1') {
                    input.GetChar(number_table[count + 2]);
                    if (number_table[count + 2] == '6') {
                        while (count > -3) {
                            input.UngetChar(number_table[count + 2]);
                            count--;
                        }
                        while (count2 > -3) {
                            count2--;
                            input.GetChar(current_char);
                            tmp.lexeme += current_char;
                        }
                        tmp.token_type = BASE16NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    } else {
                        input.UngetChar(number_table[count + 2]);
                        input.UngetChar(number_table[count + 1]);
                    }
                } else {
                    input.UngetChar(number_table[count + 1]);
                }
            }
            while (count > 0) {
                input.UngetChar(number_table[count]);
                count--;
            }
            input.UngetChar(next_char1);
        } else {
            input.UngetChar(next_char1);
        }
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(current_char);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}




Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
