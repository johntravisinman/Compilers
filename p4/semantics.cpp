#include "semantics.h"
#include <iostream>
#include <string>

/* assume no more than 100 items in a program and generate stack overflow if more */
const int max_stack_size = 100;
/* Array of tokens implemented as a stack */
Token stack[max_stack_size];
/* total variable count */
int var_count = 0;
/* hold the element position in the stack where the current scope begins */
int scope_begin = 0;
/* total temporary variable count */
static int temp_var_count = 0;
/* total number of labels */
static int label_count = 0;
/* array to hold temp variables */
std::string temp_vars[max_stack_size];

/* builds an empty stack */
void build_stack()
{
    for (int i = 0; i <= max_stack_size; i++)
        stack[i].token_string = "";
}
/* the same as build stack but for the temporary variable array */
void build_temp_vars()
{
    for (int i = 0; i <= max_stack_size; i++)
        temp_vars[i] = "";
}
/* return a new temporary variable name, with the name "T#" 
 * where # = temp_var_count
 */
std::string get_temp_var()
{
    /* create the temp_var name */
    std::string temp_var = "T" + std::to_string(temp_var_count + 1);
    /* put the temp_var in the temp_var array */
    temp_vars[temp_var_count] = temp_var;
    /* created new temp_var so increment the count of the variables */
    temp_var_count++;
    /* return the temp_var name */
    return temp_var;
}
/* similar to get_temp_var but returns a label "L#" instead of "T#" */
std::string get_label()
{
    std::string label = "L" + std::to_string(label_count + 1);
    label_count++;
    return label;
}

void push(Token tk)
{
    if (var_count >= max_stack_size)
    {
        std::cout << "ERROR: Full stack" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        for (int i = scope_begin; i < var_count; i++)
        {
            if (stack[i].token_string == tk.token_string)
            {
                std::cout << "ERROR: " << tk.token_string << " has already been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        stack[var_count] = tk;
        out_file << "\tPUSH\n";
        var_count++;
    }
}

void pop(int scope_begin)
{
    for (int i = var_count; i > scope_begin; i--)
    {
        var_count--;
        stack[i].token_string == "";
        out_file << "\tPOP\n";
    }
}

int find(Token tk)
{
    for (int i = var_count; i >= scope_begin; i--)
    {
        if (stack[i].token_string == tk.token_string)
        {
            int tos_distance = var_count - 1 - i;
            return tos_distance;
        }
    }

    return -1;
}

// bool var_exists(Token tk)
// {
//     for(int i = var_count - 1; i > -1; i--)
//     {
//         if(stack[i].token_string == tk.token_string)
//             return true;
//     }

//     return false;
// }

int var_exists(Token tk)
{
    for (int i = var_count - 1; i > -1; i--)
    {
        if (stack[i].token_string == tk.token_string)
        {
            std::cout << tk.token_string << " found at " << var_count - 1 - i << "\n";
            return var_count - 1 - i;
        }
        else
        {
            std::cout << tk.token_string << " not found at " << var_count - 1 - i << "\n";
        }
    }

    return -1;
}

void semantic_check(Node *node, int count)
{
    if (node == nullptr)
        return;

    if (node->name == "<program>")
    {
        unsigned int num_vars = 0;
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, num_vars);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, num_vars);
        out_file << "STOP\n";
        for (int i = 0; i < max_stack_size; i++)
        {
            if (temp_vars[i] != "")
                out_file << temp_vars[i] << " 0\n";
        }
    }
    else if (node->name == "<vars>")
    {
        int tos_distance = find(node->token_1);
        scope_begin = var_count;

        if (tos_distance == -1 || tos_distance > count)
        {
            push(node->token_1);
            count++;
        }
        else if (tos_distance < count)
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has already been defined in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<block>")
    {
        unsigned int num_vars = 0;
        scope_begin = var_count;

        if (node->child_1 != nullptr)
            semantic_check(node->child_1, num_vars);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, num_vars);

        pop(scope_begin);
    }
    else if (node->name == "<expr>")
    {
        if (node->token_1.token_identifier == SLASH_TK)
        {
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            std::string temp_var = get_temp_var();
            out_file << "\t\tSTORE " << temp_var << "\n";
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            out_file << "\t\tDIV " << temp_var << "\n";
        }
        else if (node->token_1.token_identifier == ASTERISK_TK)
        {
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            std::string temp_var = get_temp_var();
            out_file << "\t\tSTORE " << temp_var << "\n";
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            out_file << "\t\tMULT " << temp_var << "\n";
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<A>")
    {
        if (node->token_1.token_identifier == PLUS_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            std::string temp_var = get_temp_var();
            out_file << "\t\tSTORE " << temp_var << "\n";
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            out_file << "\t\tADD " << temp_var << "\n";
        }
        else if (node->token_1.token_identifier == MINUS_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            std::string temp_var = get_temp_var();
            out_file << "\t\tSTORE " << temp_var << "\n";
            if (node->child_2 != nullptr)
                semantic_check(node->child_2, count);
            out_file << "\t\tSUB " << temp_var << "\n";
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<M>")
    {
        if (node->token_1.token_identifier == MINUS_TK)
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
            out_file << "\t\tMULT -1\n";
        }
        else
        {
            if (node->child_1 != nullptr)
                semantic_check(node->child_1, count);
        }
    }
    else if (node->name == "<R>")
    {
        if (node->token_1.token_identifier == ID_TK)
        {
            int var_location = var_exists(node->token_1);
            if (var_location == -1)
            {
                std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
            out_file << "\t\tSTACKR " << var_location << "\n";
        }
        else if (node->token_1.token_identifier == INT_TK)
        {
            out_file << "\t\tLOAD " << node->token_1.token_string << "\n";
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
    }
    else if (node->name == "<in>")
    {
        int var_location = var_exists(node->token_1);
        if (var_location == -1)
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::string temp_var = get_temp_var();
        out_file << "\t\tREAD " << temp_var << "\n";
        out_file << "\t\tLOAD " << temp_var << "\n";
        out_file << "\t\tSTACKW " << var_location << "\n";
    }
    else if (node->name == "<out>")
    {
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        std::string temp_var = get_temp_var();
        out_file << "\t\tSTORE " << temp_var << "\n";
        out_file << "\t\tWRITE " << temp_var << "\n";
    }
    else if (node->name == "<cond>")
    {
        /* store condition operator */
        token_id cond_op = node->child_2->token_1.token_identifier;
        token_id cond_op_2 = node->child_2->token_2.token_identifier;
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, count);
        std::string temp_var = get_temp_var();
        out_file << "\t\tSTORE " << temp_var << "\n";
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        out_file << "\t\tSUB " << temp_var << "\n";
        std::string label = get_label();
        /* Series of if statments to determine which conditional operator is used */
        if (cond_op == GREATER_THAN_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRNEG\t" << label << "\n";
            }
            else
            {
                out_file << "BRZNEG\t" << label << "\n";
            }
        }
        else if (cond_op == LESS_THAN_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRPOS\t" << label << "\n";
            }
            else
            {
                out_file << "BRZPOS\t" << label << "\n";
            }
        }
        else if (cond_op == EQUALS_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRZERO\t" << label << "\n";
            }
            else
            {
                out_file << "BRPOS\t" << label << "\n";
                out_file << "BRNEG\t" << label << "\n";
            }
        }
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, count);
        out_file << label << ": \tNOOP\n";
    }
    else if (node->name == "<loop>")
    {
        token_id cond_op = node->child_2->token_1.token_identifier;
        token_id cond_op_2 = node->child_2->token_2.token_identifier;
        std::string temp_var = get_temp_var();
        std::string start_label = get_label();
        std::string end_label = get_label();
        out_file << start_label << ": \tNOOP\n";
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, count);
        out_file << "\t\tSTORE " << temp_var << "\n";
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        out_file << "\t\tSUB " << temp_var << "\n";
        /* Series of if statments to determine which conditional operator is used */
        if (cond_op == GREATER_THAN_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRNEG\t" << end_label << "\n";
            }
            else
            {
                out_file << "BRZNEG\t" << end_label << "\n";
            }
        }
        else if (cond_op == LESS_THAN_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRPOS\t" << end_label << "\n";
            }
            else
            {
                out_file << "BRZPOS\t" << end_label << "\n";
            }
        }
        else if (cond_op == EQUALS_TK)
        {
            if (cond_op_2 == EQUALS_TK)
            {
                out_file << "BRZERO\t" << end_label << "\n";
            }
            else
            {
                out_file << "BRPOS\t" << end_label << "\n";
                out_file << "BRNEG\t" << end_label << "\n";
            }
        }
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, count);
        out_file << "BR\t\t" << start_label << "\n";
        out_file << end_label << ": \tNOOP\n";
    }
    else if (node->name == "<assign>")
    {
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        int var_location = var_exists(node->token_1);
        if (var_location == -1)
        {
            std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
            exit(EXIT_FAILURE);
        }
        out_file << "\t\tSTACKW " << var_location << "\n";
    }
    else
    {
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, count);
        if (node->child_2 != nullptr)
            semantic_check(node->child_2, count);
        if (node->child_3 != nullptr)
            semantic_check(node->child_3, count);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, count);
    }
}
