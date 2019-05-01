#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "token.h"
#include "node.h"

void push(Token tk);
void pop(int scope_begin);
void build_stack();
int find(Token tk);
bool var_exists(Token tk);

void semantic_check(Node* node, int count);


#endif