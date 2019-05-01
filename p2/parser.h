#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "node.h"

extern std::ifstream in_file;

Node* parser();

Node* program();
Node* block(int depth);
Node* vars(int depth);
Node* expr(int depth);
Node* a(int depth);
Node* m(int depth);
Node* r(int depth);
Node* stats(int depth);
Node* mstat(int depth);
Node* stat(int depth);
Node* in(int depth);
Node* out(int depth);
Node* cond(int depth);
Node* loop(int depth);
Node* assign(int depth);
Node* ro(int depth);

void error(token_id expected, Token recieved);
void error(token_id expected1, token_id expected2, Token recieved);
void error(token_id expected1, token_id expected2, token_id expected3, Token recieved);
void error_stat(Token recieved);

bool is_stat(token_id tk);
bool is_ro(token_id tk);

void print_tree(Node* node);

#endif