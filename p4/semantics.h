#ifndef SEMANTICS_H
#define SEMANTICS_H
#include <fstream>
#include "token.h"
#include "node.h"

extern std::ofstream out_file;

void push(Token tk);
void pop(int scope_begin);
void build_stack();
int find(Token tk);
int var_exists(Token tk);

void semantic_check(Node* node, int count);


#endif