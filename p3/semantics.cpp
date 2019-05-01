#include "semantics.h"
#include <iostream>

/* assume no more than 100 items in a program and generate stack overflow if more */
const int max_stack_size = 100;

Token stack[max_stack_size];

int var_count = 0;
int scope_begin = 0;

void build_stack()
{
    for(int i = 0; i <= max_stack_size; i++)
        stack[i].token_string = "";
}

void push(Token tk) 
{
    if(var_count >= max_stack_size)
    {
        std::cout << "ERROR: Full stack" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        for(int i = scope_begin; i < var_count; i++)
        {
            if(stack[i].token_string == tk.token_string)
            {
                std::cout << "ERROR: " << tk.token_string << " has already been declared in this scope" <<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        stack[var_count] = tk;
        var_count++;
    }
}

void pop(int scope_begin)
{
    for(int i = var_count; i > scope_begin; i--)
    {
        var_count--;
        stack[i].token_string == "";
    }
}

int find(Token tk)
{
    for(int i = var_count; i >= scope_begin; i--)
    {
        std::cout << scope_begin << " " << var_count << std::endl; 
        if(stack[i].token_string == tk.token_string)
        {
            int tos_distance = var_count - 1 - i;
            return tos_distance;
        }
    }

    return -1;
}

bool var_exists(Token tk)
{
    for(int i = var_count - 1; i > -1; i--)
    {
        if(stack[i].token_string == tk.token_string)
            return true;
    }

    return false;
}

void semantic_check(Node* node, int count)
{
    if(node == nullptr)
        return;
    
    if(node->name == "<program>")
    {
        unsigned int num_vars = 0;
        if(node->child_1 != nullptr)
            semantic_check(node->child_1, num_vars);
        if(node->child_2 != nullptr)
            semantic_check(node->child_2, num_vars);
    }
    else if(node->name == "<vars>")
    {
        int tos_distance = find(node->token_1);
        scope_begin = var_count;

        if(tos_distance == -1 || tos_distance > count)
        {
            push(node->token_1);
            count++;
        }
        else if(tos_distance < count)
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has already been defined in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        if(node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if(node->name == "<block>")
    {
        unsigned int num_vars = 0;
        scope_begin = var_count;

        if(node->child_1 != nullptr)
            semantic_check(node->child_1, num_vars);
        if(node->child_2 != nullptr)
            semantic_check(node->child_2, num_vars);
        
        pop(scope_begin);
    }
    else if(node->name == "<expr>")
    {
        if(node->token_1.token_identifier == SLASH_TK || node->token_1.token_identifier == ASTERISK_TK)
        {
            if(node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if(node->child_2 != nullptr)
                semantic_check(node->child_2, count);
        }
        else if(node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if(node->name == "<A>")
    {
        if(node->token_1.token_identifier == PLUS_TK || node->token_1.token_identifier == MINUS_TK)
        {
            if(node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            if(node->child_2 != nullptr)
                semantic_check(node->child_2, count);
        }
        else if(node->child_1 != nullptr)
                semantic_check(node->child_1, count);
    }
    // else if(node->name == "<M>")
    // {
    //     if(node->child_1 != nullptr)
    //         semantic_check(node->child_1, count);
    // }
    else if(node->name == "<R>")
    {
        if(node->token_1.token_identifier == ID_TK)
        {
            if(!var_exists(node->token_1))
            {
                std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if(node->name == "<in>")
    {
        if(!var_exists(node->token_1))
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(node->name == "<assign>")
    {
        if(!var_exists(node->token_1))
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }
        if(node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else
    {
		if(node->child_1 != nullptr)
		    semantic_check(node->child_1, count);
		if(node->child_2 != nullptr)
		    semantic_check(node->child_2, count);
		if(node->child_3 != nullptr)
		    semantic_check(node->child_3, count);
		if(node->child_4 != nullptr)
		    semantic_check(node->child_4, count);
	}
}