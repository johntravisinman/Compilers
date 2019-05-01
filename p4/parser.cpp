#include "parser.h"
#include "scanner.h"
#include <iostream>

/*Globals*/
/* File to be scanned and parsed */
std::ifstream in_file;
/* Line number in in_file */
unsigned int line_number = 1;
/* Current working token */
Token tk;

/* Array of strings mapped to token_id enum */
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
    "ERROR_TK"};

/* Main parsing function called from main */
Node* parser()
{
    /* creating the root node */
    Node* node;
    /* scanning for the first token */
    tk = scan(in_file, line_number);
    /* calling first non-terminal function */
    node = program();
    /* check for EOF tk */
    if (tk.token_identifier == EOF_TK)
    {
        /* no errors and EOF tk recieved so parse was successful */
        std::cout << "Parse Success" << std::endl;
        /* return root node */
        return node;
    }
    /* unexpected token recieved */
    else
        error(EOF_TK, tk);
}

/* Function for <program> -> void <vars> <block> */
Node* program()
{
    /* depth of node in tree */
    int depth = 0;
    /* creating a node for <program> */
    Node* node = new Node("<program>", depth);
    if (tk.token_identifier == VOID_TK)
    {
        /* scan for next token */
        tk = scan(in_file, line_number);
        /* children are <vars> and <block> */
        node->child_1 = vars(depth);
        node->child_2 = block(depth);
        /* return node with attached children */
        return node;
    }
    else
        error(VOID_TK, tk);
}

/* Function for <vars> -> empty | let Identifier = Integer <vars> */
Node* vars(int depth)
{
    /* increase depth */
    depth++;
    /* create <vars> node */
    Node* node = new Node("<vars>", depth);
    /* predict: let Identifier = Integer <vars> */ 
    /* check the token and then scan for next token until end of grammar */
    if (tk.token_identifier == LET_TK)
    {
        tk = scan(in_file, line_number);
        if (tk.token_identifier == ID_TK)
        {
            /* store identifier token*/
            node->token_1 = tk;
            tk = scan(in_file, line_number);       
            if (tk.token_identifier == EQUALS_TK)
            {
                tk = scan(in_file, line_number);
                if (tk.token_identifier == INT_TK)
                {
                    /* store integer token */
                    node->token_2 = tk;
                    tk = scan(in_file, line_number);
                    /* child is <vars> */
                    node->child_1 = vars(depth);
                    return node;
                }
                else
                    error(INT_TK, tk);
            }
            else
                error(EQUALS_TK, tk);
        }
        else
            error(ID_TK, tk);
    }
    /* empty production */
    else
        return nullptr;
}


/* Non-terminal function for <block> -> begin <vars> <stats> end */
Node* block(int depth)
{

    depth++;
    Node* node = new Node("<block>", depth);
    /* predict: begin <vars> <stats> end */
    if (tk.token_identifier == BEGIN_TK)
    {
        tk = scan(in_file, line_number);
        /* children are <vars> and <stats> */
        node->child_1 = vars(depth);
        node->child_2 = stats(depth);
        if (tk.token_identifier == END_TK)
        {
            tk = scan(in_file, line_number);
            return node;
        }
        else
            error(END_TK, tk);
    }
    else
        error(BEGIN_TK, tk);
}

/* Non-terminal function for <expr> -> <A> / <expr> | <A> * <expr> | <A> */
Node* expr(int depth)
{
    depth++;
    Node* node = new Node("<expr>", depth);
    /* children are <a> and <expr> or just <a> */
    node->child_1 = a(depth);
    /* predict: / <expr> */
    if (tk.token_identifier == SLASH_TK)
    {
        /* store operator */
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        node->child_2 = expr(depth);
        return node;
    }
    /* predict: * <expr> */
    else if (tk.token_identifier == ASTERISK_TK)
    {
        /* store operator */
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        node->child_2 = expr(depth);
        return node;
    }
    else
        return node;
}

/* Non-terminal function for <M> -> <M> + <A> | <M> - <A> | <M> */
Node* a(int depth)
{
    depth++;
    Node* node = new Node("<A>", depth);
    /* children are <M> and <A> or just <M> */
    node->child_1 = m(depth);
    if (tk.token_identifier == PLUS_TK)
    {
        /* store operator */
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        node->child_2 = a(depth);
        return node;
    }
    else if (tk.token_identifier == MINUS_TK)
    {
        /* store operator */
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        node->child_2 = a(depth);
        return node;
    }
    else
        return node;
}

/* Non-terminal function for <M> -> - <M> |  <R> */
Node* m(int depth)
{
    depth++;
    Node* node = new Node("<M>", depth);
    if (tk.token_identifier == MINUS_TK)
    {
        /* strore operator */
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        node->child_1 = m(depth);
        return node;
    }
    else
    {
        node->child_1 = r(depth);
        return node;        
    }
}

/* Non-terminal function for <R> -> ( <expr> ) | Identifier | Integer */
Node* r(int depth)
{
    depth++;
    Node* node = new Node("<R>", depth);
    if (tk.token_identifier == LEFT_PAREN_TK)
    {
        tk = scan(in_file, line_number);
        node->child_1 = expr(depth);
        if (tk.token_identifier == RIGHT_PAREN_TK)
        {
            tk = scan(in_file, line_number);
            return node;
        }
        else
            error(RIGHT_PAREN_TK, tk);
    }
    else if (tk.token_identifier == ID_TK)
    {
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        return node;
    }
    else if (tk.token_identifier == INT_TK)
    {
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        return node;
    }
    else
        error(LEFT_PAREN_TK, ID_TK, INT_TK, tk);
}

/* Non-terminal function for <stats> -> <stat>  <mStat> */
Node* stats(int depth)
{
    depth++;
    Node* node = new Node("<stats>", depth);
    node->child_1 = stat(depth);
    node->child_2 = mstat(depth);
    return node;
}

/* Non-terminal function for <mstat> -> empty |  <stat>  <mStat> */
Node* mstat(int depth)
{
    depth++;
    Node* node = new Node("<mstat>", depth);
    if (is_stat(tk.token_identifier))
    {
        node->child_1 = stat(depth);
        node->child_2 = mstat(depth);
        return node;
    }
    else
        return nullptr;
}

/* Non-terminal function for <stat> -> <in> | <out> | <block> | <cond> | <loop> | <assign> */
Node* stat(int depth)
{
    depth++;
    Node* node = new Node("<stat>", depth);
    if(tk.token_identifier == READ_TK)
    {
        node->child_1 = in(depth);
        return node;
    }
    else if(tk.token_identifier == PRINT_TK)
    {
        node->child_1 = out(depth);
        return node;
    }
    else if(tk.token_identifier == BEGIN_TK)
    {
        node->child_1 = block(depth);
        return node;
    }
    else if(tk.token_identifier == COND_TK)
    {
        node->child_1 = cond(depth);
        return node;
    }
    else if(tk.token_identifier == ITER_TK)
    {
        node->child_1 = loop(depth);
        return node;
    }
    else if(tk.token_identifier == ID_TK)
    {
        node->child_1 = assign(depth);
        return node;
    }
    else
        error_stat(tk);
}

/* Non-terminal function for <in> -> read ( Identifier ) : */
Node* in(int depth)
{
    depth++;
    Node* node = new Node("<in>", depth);
    if(tk.token_identifier == READ_TK)
    {
        tk = scan(in_file, line_number);
        if(tk.token_identifier == LEFT_PAREN_TK)
        {
            tk = scan(in_file, line_number);
            if(tk.token_identifier == ID_TK)
            {
                node->token_1 = tk;
                tk = scan(in_file, line_number);
                if(tk.token_identifier == RIGHT_PAREN_TK)
                {
                    tk = scan(in_file, line_number);
                    if(tk.token_identifier == COLON_TK)
                    {
                        tk = scan(in_file, line_number);
                        return node;
                    }
                    else
                        error(COLON_TK, tk);
                }
                else
                    error(RIGHT_PAREN_TK, tk);
            }
            else
                error(ID_TK, tk);
        }
        else
            error(LEFT_PAREN_TK, tk);
    }
    else
        error(READ_TK, tk);
}

/* Non-terminal function for <out> -> print ( <expr>  ) : */
Node* out(int depth)
{
    depth++;
    Node* node = new Node("<out>", depth);
    if (tk.token_identifier == PRINT_TK)
    {
        tk = scan(in_file, line_number);
        if (tk.token_identifier == LEFT_PAREN_TK)
        {
            tk = scan(in_file, line_number);
            node->child_1 = expr(depth);
            if (tk.token_identifier == RIGHT_PAREN_TK)
            {
                tk = scan(in_file, line_number);
                if (tk.token_identifier == COLON_TK)
                {
                    tk = scan(in_file, line_number);
                    return node;
                }
                else
                    error(COLON_TK, tk);
            }
            else
                error(RIGHT_PAREN_TK, tk);
        }
        else
            error(LEFT_PAREN_TK, tk);
    }
    else
        error(PRINT_TK, tk);
}

/* Non-terminal function for <cond> -> cond ( <expr> <RO> <expr> ) <stat> */
Node* cond(int depth)
{
    depth++;
    Node* node = new Node("<cond>", depth);
    if(tk.token_identifier == COND_TK)
    {
        tk = scan(in_file, line_number);
        if(tk.token_identifier == LEFT_PAREN_TK)
        {
            tk = scan(in_file, line_number);
            node->child_1 = expr(depth);
            node->child_2 = ro(depth);
            node->child_3 = expr(depth);
            if(tk.token_identifier == RIGHT_PAREN_TK)
            {
                tk = scan(in_file, line_number);
                node->child_4 = stat(depth);
                return node;
            }
            else
                error(RIGHT_PAREN_TK, tk);
        }
        else
            error(LEFT_PAREN_TK, tk);
    }
    else
        error(COND_TK, tk);
}

/* Non-terminal function for <loop> -> iter ( <expr> <RO> <expr> ) <stat> */
Node* loop(int depth)
{
    depth++;
    Node* node = new Node("<loop>", depth);
    if(tk.token_identifier == ITER_TK)
    {
        tk = scan(in_file, line_number);
        if(tk.token_identifier == LEFT_PAREN_TK)
        {
            tk = scan(in_file, line_number);
            node->child_1 = expr(depth);
            node->child_2 = ro(depth);
            node->child_3 = expr(depth);
            if(tk.token_identifier == RIGHT_PAREN_TK)
            {
                tk = scan(in_file, line_number);
                node->child_4 = stat(depth);
                return node;
            }
            else
                error(RIGHT_PAREN_TK, tk);
        }
        else
            error(LEFT_PAREN_TK, tk);
    }
    else
        error(ITER_TK, tk);
}

/* Non-terminal function for <assign> -> Identifier  = <expr> : */
Node* assign(int depth)
{
    depth++;
    Node* node = new Node("<assign>", depth);
    if(tk.token_identifier == ID_TK)
    {
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        if(tk.token_identifier == EQUALS_TK)
        {
            tk = scan(in_file, line_number);
            node->child_1 = expr(depth);
            if(tk.token_identifier == COLON_TK)
            {
                tk = scan(in_file, line_number);
                return node;
            }
            else
                error(COLON_TK, tk);
        }
        else
            error(EQUALS_TK, tk);
    }
    else
        error(ID_TK, tk);
}

/* Non-terminal function for <RO> ->  < | <  = | > | >  = | =  = | = */
Node* ro(int depth)
{
    depth++;
    Node* node = new Node("<RO>", depth);
    if(is_ro(tk.token_identifier))
    {
        node->token_1 = tk;
        tk = scan(in_file, line_number);
        if(tk.token_identifier == EQUALS_TK)
        {
            node->token_2 = tk;
            tk = scan(in_file, line_number);
            return node;
        }
        else
            return node;
    }
    else
        error(LESS_THAN_TK, GREATER_THAN_TK, EQUALS_TK, tk);
}

/* error function for single expected token */
void error(token_id expected, Token recieved)
{
    std::cout << "Parsing Error" << std::endl;
    std::cout << "Expected token: " << tokens[expected] << std::endl;
    std::cout << "Recieved token: " << tokens[recieved.token_identifier];
    std::cout << " At line: " << recieved.line_number << std::endl;
    exit(EXIT_FAILURE);
}

/* error function for two expeected tokens */
void error(token_id expected1, token_id expected2, Token recieved)
{
    std::cout << "Parsing Error" << std::endl;
    std::cout << "Expected tokens: " << tokens[expected1] << " or " << tokens[expected2] << std::endl;
    std::cout << "Recieved token: " << tokens[recieved.token_identifier];
    std::cout << " At line: " << recieved.line_number << std::endl;
    exit(EXIT_FAILURE);
}

/* error function for three expected tokens */
void error(token_id expected1, token_id expected2, token_id expected3, Token recieved)
{
    std::cout << "Parsing Error" << std::endl;
    std::cout << "Expected tokens: " << tokens[expected1] << ", " << tokens[expected2];
    std::cout << ", or " << tokens[expected3] << std::endl;
    std::cout << "Recieved token: " << tokens[recieved.token_identifier];
    std::cout << " At line: " << recieved.line_number << std::endl;
    exit(EXIT_FAILURE);
}
/* error function for expected statement token */
void error_stat(Token recieved)
{
    std::cout << "Parsing Error" << std::endl;
    std::cout << "Expected tokens: " << tokens[READ_TK] << ", " << tokens[PRINT_TK];
    std::cout << ", " << tokens[BEGIN_TK] << ", " << tokens[COND_TK]; 
    std::cout << ", " << tokens[ITER_TK] << ", or " << tokens[ID_TK] << std::endl;
    std::cout << "Recieved token: " << tokens[recieved.token_identifier];
    std::cout << " At line: " << recieved.line_number << std::endl;
    exit(EXIT_FAILURE);
}
/* function to determine if token is a statement token */
bool is_stat(token_id tk)
{
    if (tk == READ_TK || tk == PRINT_TK || tk == BEGIN_TK || tk == COND_TK || tk == ITER_TK || tk == ID_TK)
        return true;
    else
        return false;
}

bool is_ro(token_id tk)
{
    if (tk == LESS_THAN_TK || tk == GREATER_THAN_TK || tk == EQUALS_TK)
        return true;
    else
        return false;
}

void print_tree(Node* node)
{
    if(node == nullptr)
    {
        std::cout << "<Empty Tree>" << std::endl;
        return;
    }
    else
    {
        std::string indents((node->depth * 2), ' ');
        std::cout << indents;
        std::cout << node->name << "  ";
        std::cout << node->token_1.token_string << "  " << node->token_2.token_string;
        if(node->token_1.token_string != "")
            std::cout << " On Line: " << node->token_1.line_number; 
        std::cout << std::endl;

        if(node->child_1 != nullptr)
            print_tree(node->child_1);
        if(node->child_2 != nullptr)
            print_tree(node->child_2);
        if(node->child_3 != nullptr)
            print_tree(node->child_3);
        if(node->child_4 != nullptr)
            print_tree(node->child_4);
    }
    return;
}