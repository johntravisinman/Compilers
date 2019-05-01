#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include <fstream>
#include "token.h"

void test_scanner(std::ifstream& input);

void print_token(Token token);

#endif