#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
#include <map>
#include <fstream>

/* Scanner Function */
Token scan(std::ifstream& in_file, unsigned int &line_number);

/* FSA Column Getter */
int get_fsa_column(char current_char);

/* Token Getter */
Token get_token(int state, std::string word, unsigned int line_number);

#endif