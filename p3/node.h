#ifndef NODE_H
#define NODE_H

#include "token.h"

struct Node
{
    std::string name;
    int depth;

    Token token_1;
    Token token_2;

    Node* child_1; 
    Node* child_2;
    Node* child_3;
    Node* child_4;

    Node()
    {
        this->name = "";
        this->depth = 0;
        this->token_1 = Token();
        this->token_2 = Token();
        this->child_1 = nullptr;
        this->child_2 = nullptr;
        this->child_3 = nullptr;
        this->child_4 = nullptr;
    }
    Node(std::string name, int depth)
    {
        this->name = name;
        this->depth = depth;
        this->token_1 = Token();
        this->token_2 = Token();
        this->child_1 = nullptr;
        this->child_2 = nullptr;
        this->child_3 = nullptr;
        this->child_4 = nullptr;
    }
};

#endif