#ifndef TREE_H_
#define TREE_H_
#include "node.h"
#include <string>
#include <fstream>
#include <iostream>

class Tree
{
	public:

    Node* root;

    Tree();
    ~Tree();

    void insert(std::string value);

    Node* buildTree(std::istream& input);
    void traverseInorder(Node* node, std::string filename);
    void traversePostorder(Node* node, std::string filename);
    void traversePreorder(Node* node, std::string filename);

	private:

	Node* insert(Node* node, std::string value);
	Node* empty(Node* node);

	void traverseInorder(Node* node, int level, std::ostream& output);
    void traversePostorder(Node* node, int level, std::ostream& output);
    void traversePreorder(Node* node, int level, std::ostream& output);




    //    void traverseInorder(Node*);
    //    // parameters: tree root, and output basefilename
    //    void traversePreorder(Node*, const char[]);
    //    void traversePostorder(Node*, const char[]);
};

#endif /* TREE_H_ */
