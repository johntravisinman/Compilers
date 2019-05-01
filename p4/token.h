#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/* Enum to represent each token */
enum token_id
{
    /* Keyword Tokens */
    BEGIN_TK,
    END_TK,
    ITER_TK,
    VOID_TK,
    VAR_TK,
    RETURN_TK,
    READ_TK,
    PRINT_TK,
    PROGRAM_TK,
    COND_TK,
    THEN_TK,
    LET_TK,
    /* Operator Tokens */
    EQUALS_TK,
    LESS_THAN_TK,
    GREATER_THAN_TK,
    COLON_TK,
    PLUS_TK,
    MINUS_TK,
    ASTERISK_TK,
    SLASH_TK,
    PERCENT_TK,
    PERIOD_TK,
    /* Delimiter Tokens */
    LEFT_PAREN_TK,
    RIGHT_PAREN_TK,
    LEFT_BRACKET_TK,
    RIGHT_BRACKET_TK,
    LEFT_BRACE_TK,
    RIGHT_BRACE_TK,
    SEMI_COLON_TK,
    COMMA_TK,
    /* Identifier and Integer Tokens */
    ID_TK,
    INT_TK,
    /* EOF and Error Tokens */
    EOF_TK,
    ERROR_TK
};

/* Token struct */
struct Token
{
    token_id token_identifier;
    std::string token_string;
    unsigned int line_number;

    /* Default Token 
     * initializes a Token as an ERROR_TK because if a token is never changed after init
     * then an some kind of error has occured */
    Token()
    {
        this->token_identifier = ERROR_TK;
        this->token_string = "";
        this->line_number = 0;
    }

    /* Standard Token */
    Token(token_id token_identifier, std::string token_string, unsigned int line_number)
    {
        this->token_identifier = token_identifier;
        this->token_string = token_string;
        this->line_number = line_number;
    }
};

#endif