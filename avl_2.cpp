//An empty binary tree is an AVL Tree//
//A non-empty binary tree is an AVL Tree if the two subtrees of each node are AVL trees and balance-factor of each node is 0,1 or-1.//
//An AVL tree is a balanced binary tree//
//A AVL Search Tree is Binary Search Tree satisfying above properties of AVL Tree//
#include<bits/stdc++.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

typedef struct AVLNode{
	
	struct AVLNode* left;
	struct AVLNode* right;
	int info;
}Node;

int max(int a,int b){
	
	if(a>=b){
		return a;
	}
	else{
		return b;
	}
}

int height(struct AVLNode* r){
	
	if(r){
		
		int h=1;
		int h_left=height(r->left);
		int h_right=height(r->right);
		h=h+max(h_left,h_right);
		return h;
	}
	else{
		return 0;
	}
}


int get_bal(struct AVLNode* r){
	
	int bal;
	bal=height(r->left)-height(r->right);
	return bal;
}

struct AVLNode* rrRotate(struct AVLNode* A,struct AVLNode* B){
	
	struct AVLNode* temp;
	temp=B->left;
	B->left=A;
	A->right=temp;
	return B;
}

struct AVLNode* llRotate(struct AVLNode* A,struct AVLNode* B){
	struct AVLNode* temp;
	temp=B->right;
	B->right=A;
	A->left=temp;
	return B;
}

struct AVLNode* rlRotate(struct AVLNode* A,struct AVLNode* B){
	
	A->right=llRotate(B,B->left);
	A=rrRotate(A,A->right);
	return A;
}

struct AVLNode* lrRotate(struct AVLNode* A,struct AVLNode* B){
	
	A->left=rrRotate(B,B->right);
	A=llRotate(A,A->left);
	return A;
	
}

struct AVLNode* createNode(int data){
	
	struct AVLNode* temp;
	temp= new Node();
	temp->left=NULL;
	temp->right=NULL;
	temp->info=data;
	return temp;
}

struct AVLNode* insert(struct AVLNode* r,int data){
	
	if(r!=NULL){
		if(data>r->info){
			r->right=insert(r->right,data);
		}
		else{
			r->left=insert(r->left,data);
		}
		
		int balance=get_bal(r);
		
		if(balance<-1 && get_bal(r->right)<=0){
		  
			r=rrRotate(r,r->right);
			return r;
		}
		else if(balance>1 && get_bal(r->left)>=0){ 
		
			r=llRotate(r,r->left);
			return r;
		}
		else if(balance<-1 && get_bal(r->right)>0){ 
		     r=rlRotate(r,r->right);
		     return r;
		}
		
		else if(balance>1 && get_bal(r->left)<0){                          
			r=lrRotate(r,r->left);
			return r;
		}
		
		else{
			return r;
		}
	}
	
	else{
		r=createNode(data);
		return r;
	}
}

void inOrder(struct AVLNode* r){
	
	if(r){
	inOrder(r->left);
	printf("%d ",r->info);
	inOrder(r->right);
     }
}

void deleteTree(Node *&node)
{
    if(node == NULL)
        return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}

bool search(int value, Node*& node)
{
    if(node == NULL)
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

int main(){
	
	Node* tree = nullptr;
	vector<string> file_names = {"50","100","200","300","500","750","1000","1500","2000","3000",
		"5000","7500","10000","12500","15000","20000","25000","30000","40000","50000","75000","100000","125000","150000","175000","200000","225000","250000"};

	for(int i=0; i<file_names.size(); i++)
	{
		float mean_construction = 0.0;
		float mean_search = 0.0;
		for(int j=0; j<5; j++)
		{
            tree = NULL;
			string path = "./Construcao/" + file_names[i] + ".txt";
			string value_string;
			ifstream file_construction(path);
			float time = 0.0;
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
			time = 0.0;
			start = high_resolution_clock::now();
			while (getline (file_search, value_string,' ')) {
				int value = atoi(value_string.c_str());

				bool result = search(value, tree);
			}
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);
			mean_search += duration.count();
			deleteTree(tree);
			file_search.close();
		}
		cout << "----------------------------------------------" << endl;
		cout << file_names[i] << ".txt" << endl;
		cout << "Tempo medio de construção: " << mean_construction/5 << endl;
		cout << "Tempo medio de consulta: " << mean_search/5 << endl;
	}
}