/* Includes */
#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
#include "semantics.h"

std::ofstream out_file;

void append_new_line(std::string file_name);

int main(int argc, char** argv)
{
    /* Too many arguments */
    if (argc > 2)
    {
        std::cout << "Too many arguments given" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Filename argument given */
    else if (argc == 2)
    {
        /* Get filenaema and append extension */
        std::string file_name = argv[1];
        std::string in_file_name = file_name + ".fs18";
        std::string out_file_name = file_name + ".asm";
        
        append_new_line(in_file_name);

        /* Opening the input file */
        in_file.open(in_file_name);

        /* Given filename cannot be opened */
        if (!in_file)
        {
            std::cout << "Error opening " << file_name << " for reading";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        out_file.open(out_file_name, std::ios::trunc);
        /* Call the parser */
        Node* root = parser();
        print_tree(root);
        semantic_check(root, 0);        
        /* Close the files */
        in_file.close();
        out_file.close();

    }

    /* No filename argument given */
    else if (argc == 1)
    {
        /* Creating a temporary file to hold keyboard input */ 
        std::ofstream temp_file;
        std::string file_name = "keyboard_input.txt";
        temp_file.open(file_name, std::ios::trunc);

        /* Cannot open temporary file */
        if (!temp_file)
        {
            std::cout << "Error opening " << file_name << " for writing";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Helper string variable */
        char current_char;

        /* Writing keyboard input to temporary file */
        while (std::cin >> std::noskipws >> current_char)
            temp_file << current_char;
    
        /* Closing the file */
        temp_file.close();        
        
        /* Opening keyboard input */
        in_file.open(file_name);

        /* Cannot open temporary file */
        if (!in_file)
        {
            std::cout << "Error opening " << file_name << " for reading";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        /* Opening output file */
        std::string out_file_name = "out.asm";
        out_file.open(out_file_name, std::ios::trunc);
        /* Cannot open target file */
        if (!out_file)
        {
            std::cout << "Error opening " << out_file_name << " for reading";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        /* Call the parser */
        Node* root = parser();
        print_tree(root);
        semantic_check(root, 0);
        /* Close the files */
        in_file.close();
        out_file.close();
    }
}

void append_new_line(std::string file_name)
{
    std::ofstream file;
    file.open(file_name, std::ios_base::app);
    file << "\n";
    return;
}