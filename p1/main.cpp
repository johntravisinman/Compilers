/* Includes */
#include <iostream>
#include <string>
#include <fstream>
#include "testScanner.h"

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
        file_name += ".fs18";
        
        /* Opening the file */
        std::ifstream in_file;
        in_file.open(file_name);

        /* Given filename cannot be opened */
        if (!in_file)
        {
            std::cout << "Error opening " << file_name << " for reading";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Call the test scanner */
        test_scanner(in_file);
        /* Close the input file */
        in_file.close();

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
        std::ifstream in_file;
        in_file.open(file_name);

        /* Cannot open temporary file */
        if (!in_file)
        {
            std::cout << "Error opening " << file_name << " for reading";
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Call the test scanner */
        test_scanner(in_file);
        /* Close the input file */
        in_file.close();
    }
}