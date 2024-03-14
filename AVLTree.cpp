#include <string>
#include <iostream>
#include <stack>
using namespace std;

class GatorAVLTree
{
	struct Student
	{
		string name;
		unsigned int ID;
		int height;
		Student* left;
		Student* right;

		Student();
		Student(string _name, unsigned int _ID);
	};
	int getHeight(Student* node);
	int getMaxHeight(Student* node);
	int getBalanceFactor(Student* node);
	Student* rotateLeft(Student* node);
	Student* rotateRight(Student* node);
	Student* rotateLeftRight(Student* node);
	Student* rotateRightLeft(Student* node);
	Student* checkImbalance(Student* root);
	void destructorHelper(Student* root);

public:
	Student* Treeroot = NULL;
	GatorAVLTree();
	~GatorAVLTree();
	bool isValidName(string NAME);
	bool isValidID(unsigned int ID);
	Student* insert(string _name, unsigned int _ID, Student* root);
	Student* remove(unsigned int _ID, Student* root);
	void searchID(unsigned int _ID, Student* root);
	void searchName(string _name, Student* root, bool matched);
	void printInorder(Student* root, bool isfirst);
	void printPreorder(Student* root, bool isfirst);
	void printPostorder(Student* root, bool isfirst);
	void printLevelCount(Student* root);
	Student* removeInorder(unsigned int N, Student* root, unsigned int tracker);
};
//student constructors 
GatorAVLTree::Student::Student()
{
	name = "";
	ID = 0;
	height = 1;
	left = NULL;
	right = NULL;
}
GatorAVLTree::Student::Student(string NAME, unsigned int ID)
{
	name = NAME;
	this->ID = ID;
	height = 1;
	left = NULL;
	right = NULL;
}

//helper functions
int GatorAVLTree::getHeight(Student* node)
{
	if (node == NULL)
		return 0;
	else
		return node->height;
}
int GatorAVLTree::getMaxHeight(Student* node)
{
	if (getHeight(node->right) > getHeight(node->left))
		return getHeight(node->right);
	else
		return getHeight(node->left);
}
int GatorAVLTree::getBalanceFactor(Student* node)
{
	if (node == NULL)
		return 0;
	else
		return getHeight(node->left) - getHeight(node->right);
}
//rotation functions derived from my stepik solutions
GatorAVLTree::Student* GatorAVLTree::rotateLeft(Student* node)
{
	Student* gc = node->right->left;
	Student* newRoot = node->right;
	newRoot->left = node;
	node->right = gc;
	//alter all the heights starting at the new "leaves"
	newRoot->left->height = 1 + getMaxHeight(newRoot->left);
	newRoot->right->height = 1 + getMaxHeight(newRoot->right);
	newRoot->height = 1 + getMaxHeight(newRoot);
	return newRoot;
}
GatorAVLTree::Student* GatorAVLTree::rotateRight(Student* node)
{
	Student* gc = node->left->right;
	Student* newRoot = node->left;
	newRoot->right = node;
	node->left = gc;
	//rebalance all the heights
	newRoot->left->height = 1 + getMaxHeight(newRoot->left);
	newRoot->right->height = 1 + getMaxHeight(newRoot->right);
	newRoot->height = 1 + getMaxHeight(newRoot);
	return newRoot;
}
GatorAVLTree::Student* GatorAVLTree::rotateLeftRight(Student* node)
{
	node->left = rotateLeft(node->left);
	return rotateRight(node);
}
GatorAVLTree::Student* GatorAVLTree::rotateRightLeft(Student* node)
{
	node->right = rotateRight(node->right);
	return rotateLeft(node);
}
GatorAVLTree::Student* GatorAVLTree::checkImbalance(Student* node)
{
	//if you reach leaf stop recursion
	if (node == NULL)
		return node;
	else
	{
		//inorder check of the entire tree, left subtree first
		checkImbalance(node->left);
		//if imbalanced, perform rotations
		if (getBalanceFactor(node) == 2)
		{
			//left left imbalance
			if (getBalanceFactor(node->left) == 1)
				node = rotateRight(node);
			//left right imbalance
			else
				node = rotateLeftRight(node);
		}
		else if (getBalanceFactor(node) == -2)
		{
			//right right imbalance
			if (getBalanceFactor(node->right) == -1)
				node = rotateLeft(node);
			//right left imbalance
			else
				node = rotateRightLeft(node);
		}
		//right subtree check
		checkImbalance(node->right);
		return node;
	}
}
void GatorAVLTree::destructorHelper(Student* root)
{
	//iterate through the tree deleting everything
	if (root != NULL)
	{
		if (root->left != NULL)
			destructorHelper(root->left);
		if (root->right != NULL)
			destructorHelper(root->right);
	}
	delete root;
}

//AVLTree functions
GatorAVLTree::GatorAVLTree()
{
	Treeroot = NULL;
}
GatorAVLTree::~GatorAVLTree()
{
	destructorHelper(Treeroot);
}
bool GatorAVLTree::isValidName(string NAME)
{
	for (unsigned int i = 0; i < NAME.size(); i++)
	{
		if (isalpha(NAME[i]) == false)
		{
			return false;
		}
	}
	return true;
}
bool GatorAVLTree::isValidID(unsigned int ID)
{
	int digits = 0;
	unsigned int number = ID;
	//check if ID has 8 digits like it should
	while (number != 0)
	{
		number /= 10;
		digits++;
	}
	if (digits != 8)
	{
		cout << "unsuccessful\n";
		return false;
	}
	else
	{
		return true;
	}
}
GatorAVLTree::Student* GatorAVLTree::insert(string NAME, unsigned int ID, Student* root)
{
	//end recursion
	if (root == NULL)
	{
		Student* temp = new Student(NAME, ID);
		cout << "successful\n";
		return temp;
	}
	//if attempting to insert an ID that already exists, unsuccessful
	if (ID == root->ID)
	{
		cout << "unsuccessful\n";
		return NULL;
	}
	else if (ID < root->ID)
		root->left = insert(NAME, ID, root->left);
	else if (ID > root->ID)
		root->right = insert(NAME, ID, root->right);
	//change height of the nodes
	root->height = 1 + getMaxHeight(root);
	//perform rotations if needed
	return checkImbalance(root);
}
GatorAVLTree::Student* GatorAVLTree::remove(unsigned int ID, Student* root)
{
	//if you reach end of tree without a match, unsuccessful
	if (root == NULL)
	{
		cout << "unsuccessful\n";
		return root;
	}
	//the ID were looking for is further right
	else if (ID > root->ID)
	{
		root->right = remove(ID, root->right);
	}

	//...or further left
	else if (ID < root->ID)
	{
		root->left = remove(ID, root->left);
	}

	//match
	else
	{
		Student* temp = NULL;
		//if it only has one child, switch the data (but not the children) and then delete the child. IF it has no children, jsut deletes the node
		if (root->left == NULL)
		{
			temp = root->right;
			delete root;
			root = NULL;
			return temp;
		}
		else if (root->right == NULL)
		{
			temp = root->left;
			delete root;
			root = NULL;
			return temp;
		}
		//else if it has 2 children, find the inorder predecessor
		else
		{
			//set temp to the predecessor which is the right-most value of the left subtree
			temp = root->left;
			while (temp->right != NULL)
			{
				temp = temp->right;
			}
			//assign the temp's data to the root
			root->name = temp->name;
			root->ID = temp->ID;
			//remove the old data
			root->left = remove(temp->ID, root->left);
		}
		cout << "successful\n";
		//rebalance height
		root->height = 1 + getMaxHeight(root);
		//now check for imbalances and start doing rotations
		return checkImbalance(root);
	}
}
void GatorAVLTree::searchID(unsigned int ID, Student* root)
{
	if (root == NULL)
		cout << "unsuccessful\n";
	else if (ID == root->ID)
	{
		cout << root->name << '\n';
		return;
	}
	else if (ID < root->ID)
		searchID(ID, root->left);
	else
		searchID(ID, root->right);
}
void GatorAVLTree::searchName(string NAME, Student* root, bool matched)
{
	if (Treeroot == NULL)
	{
		cout << "unsuccessful\n";
		return;
	}
	if (root == NULL)
	{
		return;
	}
	else
	{
		//traverse left subtree first
		if (root->left != NULL)
			searchName(NAME, root->left, matched);
		//then right
		if (root->right != NULL)
			searchName(NAME, root->right, matched);
		//if root matches
		if (NAME == root->name)
		{
			if (matched == false)
			{
				matched = true;
				cout << root->ID << '\n';
			}
		}
		return;
	}
	//if no matches by the end of the tree, unsuccessful
	if (matched == false)
		cout << "unsuccessful\n";
}
void GatorAVLTree::printInorder(Student* root, bool isfirst)
{
	if (root == NULL)
		return;
	else
	{
		printInorder(root->left, isfirst);
		if (root->left != NULL)
			cout << ", ";
		cout << root->name;
		if (root->right != NULL)
			cout << ", ";
		printInorder(root->right, isfirst);
	}
}
void GatorAVLTree::printPostorder(Student* root, bool isfirst)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		printPostorder(root->left, isfirst);
		if (root->left != NULL)
			cout << ", ";
		printPostorder(root->right, isfirst);
		if (root->right != NULL)
			cout << ", ";
		cout << root->name;
	}
}
void GatorAVLTree::printPreorder(Student* root, bool isfirst)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		cout << root->name;
		if (root->left != NULL)
			cout << ", ";
		printPreorder(root->left, isfirst);
		if (root->right != NULL)
			cout << ", ";
		printPreorder(root->right, isfirst);
	}
}
void GatorAVLTree::printLevelCount(Student* root)
{
	if (root == NULL)
		cout << "0\n";
	else
		cout << getHeight(root) << '\n';
}
//inspired by this geeksforgeeks post: https://www.geeksforgeeks.org/find-n-th-node-inorder-traversal/
GatorAVLTree::Student* GatorAVLTree::removeInorder(unsigned int N, Student* root, unsigned int tracker)
{
	if (root == NULL)
	{
		return root;
	}
	if (tracker < N)
	{
		//traverse left subtree first
		removeInorder(N, root->left, tracker);
		tracker++;
		//When we reach the Nth node, delete it
		if (tracker == N)
		{
			if (root == NULL)
				cout << "unsuccessful\n";
			else
			{
				remove(root->ID, Treeroot);
				cout << "successful\n";
			}
		}
		//traverse right subtree
		removeInorder(N, root->right, tracker);
	}
	return root;
}

int main()
{
	GatorAVLTree* tree = new GatorAVLTree();
	string inputs;
	getline(cin, inputs);
	int numofinputs = stoi(inputs);
	for (int i = 0; i < numofinputs; i++)
	{
		string command;
		getline(cin, command);
		string function;
		//if there are no spaces, it's a one word input 
		if (command.find_first_of(' ') == -1)
			function = command;
		else
			function = command.substr(0, command.find_first_of(' '));
		if (function == "insert")
		{
			string inputname = command.substr(command.find_first_of('"') + 1, command.find_last_of('"') - command.find_first_of('"') - 1);
			string inputID = command.substr(command.find_last_of(' ') + 1, command.find_first_of('\n') - command.find_last_of('"') - 2);
			//check if the name is valid
			if (tree->isValidName(inputname) == false)
			{
				cout << "unsuccessful\n";
				break;
			}
			unsigned int ID = stoul(inputID);
			//check if the ID is valid
			if (tree->isValidID(ID) == false)
			{
				cout << "unsuccessful\n";
				break;
			}
			tree->Treeroot = tree->insert(inputname, ID, tree->Treeroot);
		}
		else if (function == "remove")
		{
			string inputID;
			inputID = command.substr(command.find_first_of(' '), command.find_first_of('\n') - command.find_first_of(' ') - 2);
			unsigned int ID = stoul(inputID);
			tree->Treeroot = tree->remove(ID, tree->Treeroot);
		}
		else if (function == "search")
		{
			string input;
			//if the input has a name in it, perform search name
			if (command.find_first_of('\"') != -1)
			{
				int temp1 = command.find_last_of('\"');
				int temp2 = command.find_first_of('\"');
				input = command.substr(command.find_first_of('\"') + 1, command.find_last_of('\"') - command.find_first_of('\"') - 1);
				tree->searchName(input, tree->Treeroot, false);
			}
			//otherwise search for ID
			else
			{
				input = command.substr(command.find_first_of(' ') + 1, command.find_first_of('\n') - command.find_first_of(' ' ) - 2);
				tree->searchID(stoul(input), tree->Treeroot);
			}
		}
		else if (function == "printInorder")
		{
			tree->printInorder(tree->Treeroot, true);
			cout << '\n';
		}
		else if (function == "printPreorder")
		{
			tree->printPreorder(tree->Treeroot, true);
			cout << '\n';
		}
		else if (function == "printPostorder")
		{
			tree->printPostorder(tree->Treeroot, true);
			cout << '\n';
		}
		else if (function == "printLevelCount")
		{
			tree->printLevelCount(tree->Treeroot);
		}
		else if (function == "removeInorder")
		{
			string inputN;
			inputN = command.substr(command.find_first_of(' ') + 1);
			unsigned int N = stoul(inputN);
			tree->Treeroot = tree->removeInorder(N, tree->Treeroot, 0);
		}
	}
	delete tree;
}