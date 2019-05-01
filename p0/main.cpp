#include <iostream>
#include <fstream>
#include <string>
#include "tree.h"

int main(int argc, char** argv) {

    // Check for too many arguments
	if(argc > 2)
	{
		// Error message for too many arguments
		std::cout << "Too many arguments" << std::endl;
		return 0;
	}

	// Check if file name given
    if(argc == 2)
    {
        // Take in file name argument
        std::string fileName = argv[1];

        // Open file with added file extension
        std::ifstream inFile;
        inFile.open(fileName + ".fs18");

        // Check if the file exists
        if(inFile)
        {
        	// Create tree
        	Tree binaryTree;

        	// Populate tree from input file
        	binaryTree.root = binaryTree.buildTree(inFile);

        	// Traverse tree inorder and print to $filename.inorder
        	binaryTree.traverseInorder(binaryTree.root, fileName);

        	// Traverse tree postorder and print to $filename.postorder
        	binaryTree.traversePostorder(binaryTree.root, fileName);

        	// Traverse tree preorder and print to $filename.preorder
        	binaryTree.traversePreorder(binaryTree.root, fileName);

        	// Closing the input file
        	inFile.close();

        	return 0;
        }

        // Handling file not found under given file name
        else
        {
        	// Error message for file not found
            std::cout << '"' << fileName + ".fs18" << '"' << " could not be found" << std::endl;
            return 0;
        }
    }

    // Checking for no file name given
    if(argc == 1)
    {
    	// Creating a temporary file to hold console input
    	std::ofstream tempFile;
    	tempFile.open("temp.txt", std::ios::trunc); // Overwrites if file already exists

    	// Helper string variable
    	std::string word = "";

    	// Looping until EOF is reached
    	while(std::cin >> word)
    		tempFile << (word + ' ');

    	// Closing temporary file
    	tempFile.close();

    	// New input file filled with console input
    	std::ifstream inFile;
    	inFile.open("temp.txt", std::ios::in);

        // Confirm the file exists
        if(inFile)
        {
        	// Create tree
        	Tree binaryTree;

        	// Populate tree from input file
        	binaryTree.root = binaryTree.buildTree(inFile);

        	// Traverse tree inorder and print to out.inorder
        	binaryTree.traverseInorder(binaryTree.root, "out");

        	// Traverse tree postorder and print to out.postorder
        	binaryTree.traversePostorder(binaryTree.root, "out");

        	// Traverse tree preorder and print to out.preorder
        	binaryTree.traversePreorder(binaryTree.root, "out");

        	// Closing input file
        	inFile.close();

        	return 0;
        }

        // Handling temporary file not found
        else
        {
        	// Error message for temporary file not found
            std::cout << "Temporary file could not be found" << std::endl;
            return 0;
        }

    }


    return 0;
}
