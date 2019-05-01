#include "tree.h"

// Tree Constructor initializes empty tree
Tree::Tree()
{
	root = NULL;
}

// Tree Destructor which deletes each node of the tree through empty() helper function
Tree::~Tree()
{
	root = empty(root);
}

// This function builds a binary tree of strings from given istream based on string length
Node* Tree::buildTree(std::istream& input)
{
	// Helper string variable
	std::string word;

	// Looping until EOF
	while(input >> word)
	{
		// inserting each word from the stream into the tree
		this->insert(word);
	}
	return root;
}

// This function inserts a given string value into the tree
void Tree::insert(std::string value)
{
	root = insert(root, value);
}

// This function opens(or creates) a file for writing and prints a tree inorder to said file
void Tree::traverseInorder(Node* node, std::string filename)
{
	// Open file for writing
	filename += ".inorder";
	std::ofstream outFile;
	outFile.open(filename, std::ios::trunc);

	// Write tree inorder to the file
	traverseInorder(node, 0, outFile);
}

// This function opens(or creates) a file for writing and prints a tree postorder to said file
void Tree::traversePostorder(Node* node, std::string filename)
{
	// Open file for writing
	filename += ".postorder";
	std::ofstream outFile;
	outFile.open(filename, std::ios::trunc);

	// Write tree postorder to the file
	traversePostorder(node, 0, outFile);
}

// This function opens(or creates) a file for writing and prints a tree preorder to said file
void Tree::traversePreorder(Node* node, std::string filename)
{
	// Open file for writing
	filename += ".preorder";
	std::ofstream outFile;
	outFile.open(filename, std::ios::trunc);

	// Write tree preorder to the file
	traversePreorder(node, 0, outFile);
}

// Private Functions


// This function inserts a string into a tree with a given root based on the string length
Node* Tree::insert(Node* node, std::string value)
{
	// Adding new node to tree
	if(node == NULL) return new Node(value);

	// less than so go to left node
	if (value.length() < node->length)
		node->left = insert(node->left, value);

	// greater than so go to right node
	else if (value.length() > node->length)
		node->right = insert(node->right, value);

	// equals so add to node set
	else if (value.length() == node->length)
		node->data.insert(value);

	return node;
}

// This function deletes every node of a tree
Node* Tree::empty(Node* node)
{
	if(node == NULL)
		return NULL;

	empty(node->left);
	empty(node->right);
	delete node;

	return NULL;
}

void Tree::traverseInorder(Node* node, int level, std::ostream& output)
{
	if(node == NULL)
		return;

	traverseInorder(node->left, (level + 1), output);

	std::string indents((level * 2), ' ');
	output << indents;
	for(auto word : node->data)
		output << word << " ";
	output << std::endl;

	traverseInorder(node->right, (level + 1), output);
}

void Tree::traversePostorder(Node* node, int level, std::ostream& output)
{
	if(node == NULL)
		return;

	traversePostorder(node->left, (level + 1), output);

	traversePostorder(node->right, (level + 1), output);

	std::string indents((level * 2), ' ');
	output << indents;
	for(auto word : node->data)
		output << word << " ";
	output << std::endl;
}

void Tree::traversePreorder(Node* node, int level, std::ostream& output)
{
	if(node == NULL)
		return;

	std::string indents((level * 2), ' ');
	output << indents;
	for(auto word : node->data)
		output << word << " ";
	output << std::endl;

	traversePreorder(node->left, (level + 1), output);

	traversePreorder(node->right, (level + 1), output);

}
