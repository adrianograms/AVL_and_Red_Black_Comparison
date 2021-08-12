// C++ program to insert a node in AVL tree
#include<bits/stdc++.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

int count_construction = 0;
int count_search = 0;
 
// An AVL tree node
class Node
{
    public:
    int key;
    Node *left;
    Node *right;
    int height;
};
 
// A utility function to get maximum
// of two integers
int max(int a, int b);
 
// A utility function to get the
// height of the tree
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
// A utility function to get maximum
// of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}
 
/* Helper function that allocates a
   new node with the given key and
   NULL left and right pointers. */
Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially
                      // added at leaf
    return(node);
}
 
// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;
 
    // Return new root
    return x;
}
 
// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    // Update heights
    x->height = max(height(x->left),   
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
 
// Recursive function to insert a key
// in the subtree rooted with node and
// returns the new root of the subtree.
Node* insert(Node* node, int key)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
 
    if (key < node->key){
        count_construction++;
        node->left = insert(node->left, key);
    }else if (key > node->key){
        count_construction +=2;
        node->right = insert(node->right, key);
    }else // Equal keys are not allowed in BST
        return node;
 
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                        height(node->right));
 
    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}
 
// A utility function to print preorder
// traversal of the tree.
// The function also prints height
// of every node
void preOrder(Node *root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void deleteTree(Node *node)
{
    if(node == NULL)
        return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}

bool search(int value, Node* node)
{
	if(node->key == value){
        count_search++;
		return true;
	}else if(node->key > value and node->left != nullptr) {
        count_search += 2;
		return search(value, node->left);
    }else if(node->key < value and node->right != nullptr){
        count_search += 3;
		return search(value, node->right);
    }else
		return false;
}

int fdepth(int value, Node* node)
{
    int value_left, value_right;
	value_left = value_right = value;
	value++;
	if(node->left != NULL)
		value_left = fdepth(value, node->left);
	if(node->right !=NULL)
		value_right = fdepth(value, node->right);

	if(value_right > value_left)
		return value_right;
	else
		return value_left;
}
// Driver Code
int main()
{

    Node* tree = NULL;
	vector<string> file_names = {"50","100","200","300","500","750","1000","1500","2000","3000",
		"5000","7500","10000","12500","15000","20000","25000","30000","40000","50000","75000","100000","125000","150000","175000","200000","225000","250000"};

    ofstream filecsv("avl.csv");

	filecsv << "Arquivo, Tempo de contrucao, Numero de comparacoes (construcao), Tempo de busca, Numero de operacoes (busca), Profundidade\n";

	for(int i=0; i<file_names.size(); i++)
	{
		float mean_construction = 0.0;
		float mean_search = 0.0;
        int depth = 0;
		for(int j=0; j<5; j++)
		{
            count_construction = 0;
            count_search = 0;
            depth = 0;
            tree = NULL;
			string path = "./Construcao/" + file_names[i] + ".txt";
			string value_string;
			ifstream file_construction(path);
            auto start = high_resolution_clock::now();
			while (getline (file_construction, value_string,' ')) {
				int value = atoi(value_string.c_str());

				tree = insert(tree, value);
			}
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            mean_construction += duration.count();
			file_construction.close();

			path = "./Consulta/" + file_names[i] + ".txt";
			ifstream file_search(path);
			start = high_resolution_clock::now();
			while (getline (file_search, value_string,' ')) {
				int value = atoi(value_string.c_str());

                search(value, tree);
			}
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);
			mean_search += duration.count();
            depth = fdepth(0, tree);
			deleteTree(tree);
			file_search.close();
		}
		cout << "----------------------------------------------" << endl;
		cout << file_names[i] << ".txt" << endl;
		cout << "Tempo medio de construção: " << mean_construction/5 << endl;
		cout << "Comparações na construção: " << count_construction << endl;
		cout << "Tempo medio de consulta: " << mean_search/5 << endl;
		cout << "Comparações na consulta: " << count_search << endl;
        cout << "Profundidade: " << depth << endl;

        filecsv  << file_names[i] << "," << mean_construction/5 << "," 
			<< count_construction << "," << mean_search/5 << "," << count_search 
			<< "," << depth << "\n";
	}
    filecsv.close();
     
    return 0;
}
 
// This code is contributed by
// rathbhupendra