#include "testScanner.h"
#include "scanner.h"
#include <iostream>
#include <iomanip>

std::string tokens[] = {
    /* Keyword Tokens */
    "BEGIN_TK",
    "END_TK",
    "ITER_TK",
    "VOID_TK",
    "VAR_TK",
    "RETURN_TK",
    "READ_TK",
    "PRINT_TK",
    "PROGRAM_TK",
    "COND_TK",
    "THEN_TK",
    "LET_TK",
    /* Operator Tokens */
    "EQUALS_TK",
    "LESS_THAN_TK",
    "GREATER_THAN_TK",
    "COLON_TK",
    "PLUS_TK",
    "MINUS_TK",
    "ASTERISK_TK",
    "SLASH_TK",
    "PERCENT_TK",
    "PERIOD_TK",
    /* Delimiter Tokens */
    "LEFT_PAREN_TK",
    "RIGHT_PAREN_TK",
    "LEFT_BRACKET_TK",
    "RIGHT_BRACKET_TK",
    "LEFT_BRACE_TK",
    "RIGHT_BRACE_TK",
    "SEMI_COLON_TK",
    "COMMA_TK",
    /* Identifier and Integer Tokens */
    "ID_TK",
    "INT_TK",
    /* EOF and Error Tokens */
    "EOF_TK",
    "ERROR_TK"
};

void test_scanner(std::ifstream& input)
{
    /* Temporary blank/default token */
    Token current_token = Token();
    /* File line number */
    unsigned int line_number = 1;
    do
    {
        Token current_token = scan(input, line_number);
        if (current_token.token_identifier == ERROR_TK)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            print_token(current_token);
        }
    
    } while(input);

    return;
}

void print_token(Token token)
{
    std::cout << std::left;
    std::cout << "Line: " << std::setw(3) << token.line_number;
    std::cout << "\tToken: " << std::setw(15) << tokens[token.token_identifier];
    std::cout << "\tValue: " << std::setw(10) << token.token_string << std::endl;
    return;
}