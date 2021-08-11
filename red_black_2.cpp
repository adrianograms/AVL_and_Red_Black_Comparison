#include <bits/stdc++.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

typedef struct RBNode{
	
	struct RBNode* left;
	struct RBNode* right;
	struct RBNode* parent;
	int info;
	bool color; //True Red, False Black
}Node;

struct RBNode* createNode(int data){
	
	struct RBNode* temp=(struct RBNode*)malloc(sizeof(struct RBNode));
	temp->left=NULL;
	temp->right=NULL;
	temp->parent=NULL;
	temp->info=data;
	temp->color=true;
}

void swap_col(struct RBNode* g,struct RBNode* p){
	
	char c=g->color;
	g->color=p->color;
	p->color=c;
}

struct RBNode* leftRotate(struct RBNode* root,struct RBNode* g){
	
	struct RBNode* p=g->right;
	struct RBNode* temp=p->left;
	
	p->left=g;
	g->right=temp;
	if(temp){
		temp->parent=g;
	}
	p->parent=g->parent;
	if(g->parent==NULL){
		root=p;
	}
	else if(g==g->parent->right){
		g->parent->right=p;
	}
	else{
		g->parent->left=p;
	}
	g->parent=p;
	return root;
	
}

struct RBNode* rightRotate(struct RBNode* root,struct RBNode* g){
	
	struct RBNode* p=g->left;
	struct RBNode* temp=p->right;
	
	p->right=g;
	g->left=temp;
	if(temp){
		temp->parent=g;
	}
	p->parent=g->parent;
	if(g->parent==NULL){
		root=p;
	}
	else if(g==g->parent->right){
		g->parent->right=p;
	}
	else{
		g->parent->left=p;
	}
	g->parent=p;
	return root;
}

struct RBNode* BSTInsert(struct RBNode* root,struct RBNode* current){
	
	if(root){
		
		if(current->info>root->info){
			
			root->right=BSTInsert(root->right,current);
			root->right->parent=root;
			return root;
		}
		else{
			root->left=BSTInsert(root->left,current);
			root->left->parent=root;
			return root;
		}
	}
	else{
		
		root=current;
		return root;
	}
}

struct RBNode* undoViolation(struct RBNode* root,struct RBNode* curr){
	
	struct RBNode* par=curr->parent;
	struct RBNode* g_parent=NULL;
	struct RBNode* uncle=NULL;
	
	if(!(par)){
		curr->color=false;
		return root;
	}
	else if(!par->color){
		return root;
	}
	else{
		g_parent=par->parent;
		if(par==g_parent->right){
			
			uncle=g_parent->left;
			
			if(!(uncle) || !uncle->color){
				
				if(curr==par->right){
					root=leftRotate(root,g_parent);
					swap_col(g_parent,par);
					return root;
				}
				else{
					root=rightRotate(root,par);
					root=leftRotate(root,g_parent);
					swap_col(g_parent,curr);
					return root;
				}
				
			}
			else{
				uncle->color=false;
				par->color=false;
				g_parent->color=true;
				curr=g_parent;
				root=undoViolation(root,curr);
				return root;
			}
		}
		else{
			uncle=g_parent->right;
			
			if(!(uncle)|| !uncle->color){
				
				if(curr==par->left){
					root=rightRotate(root,g_parent);
					swap_col(g_parent,par);
					return root;
				}
				else{
					root=leftRotate(root,par);
					root=rightRotate(root,g_parent);
					swap_col(g_parent,par);
					return root;
				}
			}
			else{
				uncle->color=false;
				par->color=false;
				g_parent->color=true;
				curr=g_parent;
				root=undoViolation(root,curr);
				return root;
			}
		}
	}
	
}

struct RBNode* insertion(struct RBNode* root,int data){
	
	struct RBNode* x=createNode(data);
	root=BSTInsert(root,x);
	root=undoViolation(root,x);
	return root;
}

void printTree(struct RBNode* root){
	if(root){
		printTree(root->left);
		printf("%d->%c ",root->info,root->color);
		printTree(root->right);
	}
}

void deleteTree(Node *node)
{
    if(node == NULL)
        return;
    if(node->left != NULL)
		deleteTree(node->left);
	if(node->right != NULL)
		deleteTree(node->right);
	delete node;
}

bool search(int value, Node* node)
{
    if(node == nullptr)
        return false;
	if(node->info == value)
		return true;
	else if(node->info > value and node->left != nullptr)
		return search(value, node->left);
	else if(node->info < value and node->right != nullptr)
		return search(value, node->right);
	else
		return false;
}

int depth(Node *node, int depth_value)
{
    if(node == nullptr)
        return depth_value;
    else
    {
        int value_left = depth(node->left, depth_value+1);
        int value_right = depth(node->right, depth_value+1);
        if(value_left > value_right)
            return value_left;
        else
            return value_right;
    }
}

int main(){
	
	Node* tree = NULL;
	vector<string> file_names = {"50","100","200","300","500","750","1000","1500","2000","3000",
		"5000","7500","10000","12500","15000","20000","25000","30000","40000","50000","75000","100000","125000","150000","175000","200000","225000","250000"};

	for(int i=0; i<file_names.size(); i++)
	{
		float mean_construction = 0.0;
		float mean_search = 0.0;
		for(int j=0; j<5; j++)
		{
			string path = "./Construcao/" + file_names[i] + ".txt";
			string value_string;
			ifstream file_construction(path);
			float time = 0.0;
            auto start = high_resolution_clock::now();
			while (getline (file_construction, value_string,' ')) {
				int value = atoi(value_string.c_str());

				tree = insertion(tree, value);
				
			}
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
			mean_construction += duration.count();
			file_construction.close();

			path = "./Consulta/" + file_names[i] + ".txt";
			ifstream file_search(path);
			time = 0.0;
			start = high_resolution_clock::now();
			while (getline (file_search, value_string,' ')) {
				int value = atoi(value_string.c_str());

				search(value, tree);
			}
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);
			mean_search += duration.count();
			//deleteTree(tree);
			file_search.close();
		}
		cout << "----------------------------------------------" << endl;
		cout << file_names[i] << ".txt" << endl;
		cout << "Tempo medio de construção: " << mean_construction/5 << endl;
		cout << "Tempo medio de consulta: " << mean_search/5 << endl;
	}
}