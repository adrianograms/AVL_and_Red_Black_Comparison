/** C++ implementation for
Red-Black Tree Insertion
This code is adopted from
the code provided by
Dinesh Khandelwal in comments **/
#include <bits/stdc++.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

enum Color {RED, BLACK};
int count_construction = 0;
int count_search = 0;

struct Node
{
	int data;
	bool color;
	Node *left, *right, *parent;

	// Constructor
	Node(int data)
	{
	this->data = data;
	left = right = parent = NULL;
	this->color = RED;
	}
};

// Class to represent Red-Black Tree
class RBTree
{
private:
	Node *root;
protected:
	void rotateLeft(Node *&, Node *&);
	void rotateRight(Node *&, Node *&);
	void fixViolation(Node *&, Node *&);
	void deleteTree(Node *&);
	bool search(int, Node*&);
	int depth(int, Node *&);
public:
	// Constructor
	RBTree() { root = NULL; }
	void insert(const int &n);
	bool search(int n);
	void inorder();
	void levelOrder();
	void deleteTree();
	int depth();
};

// A recursive function to do inorder traversal
void inorderHelper(Node *root)
{
	if (root == NULL)
		return;

	inorderHelper(root->left);
	cout << root->data << " ";
	inorderHelper(root->right);
}

/* A utility function to insert
	a new node with given key
in BST */
Node* BSTInsert(Node* root, Node *pt)
{
	/* If the tree is empty, return a new node */
	if (root == NULL)
	return pt;

	/* Otherwise, recur down the tree */
	if (pt->data < root->data)
	{ 
		count_construction++;
		root->left = BSTInsert(root->left, pt);
		root->left->parent = root;
	}
	else if (pt->data > root->data)
	{
		count_construction +=2;
		root->right = BSTInsert(root->right, pt);
		root->right->parent = root;
	}

	/* return the (unchanged) node pointer */
	return root;
}

// Utility function to do level order traversal
void levelOrderHelper(Node *root)
{
	if (root == NULL)
		return;

	std::queue<Node *> q;
	q.push(root);

	while (!q.empty())
	{
		Node *temp = q.front();
		cout << temp->data << " ";
		q.pop();

		if (temp->left != NULL)
			q.push(temp->left);

		if (temp->right != NULL)
			q.push(temp->right);
	}
}

void RBTree::rotateLeft(Node *&root, Node *&pt)
{
	Node *pt_right = pt->right;

	pt->right = pt_right->left;

	if (pt->right != NULL)
		pt->right->parent = pt;

	pt_right->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_right;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_right;

	else
		pt->parent->right = pt_right;

	pt_right->left = pt;
	pt->parent = pt_right;
}

void RBTree::rotateRight(Node *&root, Node *&pt)
{
	Node *pt_left = pt->left;

	pt->left = pt_left->right;

	if (pt->left != NULL)
		pt->left->parent = pt;

	pt_left->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_left;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_left;

	else
		pt->parent->right = pt_left;

	pt_left->right = pt;
	pt->parent = pt_left;
}

// This function fixes violations
// caused by BST insertion
void RBTree::fixViolation(Node *&root, Node *&pt)
{
	Node *parent_pt = NULL;
	Node *grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != BLACK) &&
		(pt->parent->color == RED))
	{

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		/* Case : A
			Parent of pt is left child
			of Grand-parent of pt */
		if (parent_pt == grand_parent_pt->left)
		{

			Node *uncle_pt = grand_parent_pt->right;

			/* Case : 1
			The uncle of pt is also red
			Only Recoloring required */
			if (uncle_pt != NULL && uncle_pt->color ==
												RED)
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}

			else
			{
				/* Case : 2
				pt is right child of its parent
				Left-rotation required */
				if (pt == parent_pt->right)
				{
					rotateLeft(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				pt is left child of its parent
				Right-rotation required */
				rotateRight(root, grand_parent_pt);
				swap(parent_pt->color,
						grand_parent_pt->color);
				pt = parent_pt;
			}
		}

		/* Case : B
		Parent of pt is right child
		of Grand-parent of pt */
		else
		{
			Node *uncle_pt = grand_parent_pt->left;

			/* Case : 1
				The uncle of pt is also red
				Only Recoloring required */
			if ((uncle_pt != NULL) && (uncle_pt->color ==
													RED))
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}
			else
			{
				/* Case : 2
				pt is left child of its parent
				Right-rotation required */
				if (pt == parent_pt->left)
				{
					rotateRight(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				pt is right child of its parent
				Left-rotation required */
				rotateLeft(root, grand_parent_pt);
				swap(parent_pt->color,
						grand_parent_pt->color);
				pt = parent_pt;
			}
		}
	}

	root->color = BLACK;
}

void RBTree::deleteTree()
{
	if(root == NULL)
		return;
    if(root->left != NULL)
		deleteTree(root->left);
	if(root->right != NULL)
		deleteTree(root->right);
	delete root;
	root = NULL;
}

void RBTree::deleteTree(Node *&node)
{
    if(node->left != NULL)
		deleteTree(node->left);
	if(node->right != NULL)
		deleteTree(node->right);
	delete node;
	node = NULL;
}

// Function to insert a new node with given data
void RBTree::insert(const int &data)
{
	Node *pt = new Node(data);

	// Do a normal BST insert
	root = BSTInsert(root, pt);

	// fix Red Black Tree violations
	fixViolation(root, pt);
}

bool RBTree::search(int value, Node*& node)
{
	if(node->data == value){
		count_search++;
		return true;
	}else if(node->data > value and node->left != nullptr) {
		count_search +=2;
		return search(value, node->left);
	}else if(node->data < value and node->right != nullptr){
		count_search += 3;
		return search(value, node->right);
	}else
		return false;
}

bool RBTree::search(int value)
{
	if(root == nullptr)
		return false;
	else if(root->data == value) {
		count_search++;
		return true;
	}else if(root->data > value and root->left != nullptr){
		count_search +=2;
		return search(value, root->left);
	}else if(root->data < value and root->right != nullptr){
		count_search +=3;
		return search(value, root->right);
	}else
		return false;

}

int RBTree::depth(int value, Node*& node)
{
	int value_left, value_right;
	value_left = value_right = value;
	value++;
	if(node->left != NULL)
		value_left = depth(value, node->left);
	if(node->right !=NULL)
		value_right = depth(value, node->right);

	if(value_right > value_left)
		return value_right;
	else
		return value_left;
}

int RBTree::depth()
{
	int value, value_left, value_right;
	value = value_left = value_left = 0;
	value++;
	if(root == nullptr)
		return 0;
	if(root->left != nullptr)
		value_left = depth(value, root->left);
	if(root->right !=nullptr)
		value_right = depth(value, root->right);

	if(value_right > value_left)
		return value_right;
	else
		return value_left;
}

// Function to do inorder and level order traversals
void RBTree::inorder()	 { inorderHelper(root);}
void RBTree::levelOrder() { levelOrderHelper(root); }

// Driver Code
int main()
{
	RBTree *tree = new RBTree();
	vector<string> file_names = {"50","100","200","300","500","750","1000","1500","2000","3000",
		"5000","7500","10000","12500","15000","20000","25000","30000","40000","50000","75000","100000","125000","150000","175000","200000","225000","250000"};

	ofstream filecsv("red_black.csv");

	filecsv << "Arquivo, Tempo de contrucao, Numero de comparacoes (construcao), Tempo de busca, Numero de operacoes (busca), Profundidade\n";

	for(int i=0; i<file_names.size(); i++)
	{
		float mean_construction = 0.0;
		float mean_search = 0.0;
		int depth = 0;
		for(int j=0; j<5; j++)
		{
			count_search = 0;
			count_construction = 0;
			depth = 0;
			tree = new RBTree();
			string path = "./Construcao/" + file_names[i] + ".txt";
			string value_string;
			ifstream file_construction(path);
			float time = 0.0;
			while (getline (file_construction, value_string,' ')) {
				int value = atoi(value_string.c_str());

				auto start = high_resolution_clock::now();
				tree->insert(value);
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				time += duration.count();
			}
			mean_construction += time;
			file_construction.close();

			path = "./Consulta/" + file_names[i] + ".txt";
			ifstream file_search(path);
			time = 0.0;
			auto start = high_resolution_clock::now();
			while (getline (file_search, value_string,' ')) {
				int value = atoi(value_string.c_str());

				tree->search(value);
			}
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			mean_search += duration.count();
			depth = tree->depth();
			tree->deleteTree();
			file_search.close();
		}
		std::cout << "----------------------------------------------" << endl;
		std::cout << file_names[i] << ".txt" << endl;
		std::cout << "Tempo medio de construção: " << mean_construction/5 << endl;
		std::cout << "Comparações na construção: " << count_construction << endl;
		std::cout << "Tempo medio de consulta: " << mean_search/5 << endl;
		std::cout << "Comparações na consulta: " << count_search << endl;
		std::cout << "Profundidade: " << depth << endl;

		filecsv  << file_names[i] << "," << mean_construction/5 << "," 
			<< count_construction << "," << mean_search/5 << "," << count_search 
			<< "," << depth << "\n";
	}
	filecsv.close();

	return 0;
}
