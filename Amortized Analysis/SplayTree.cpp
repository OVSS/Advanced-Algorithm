#include <iostream>
#include <ctime> 
#include <queue>
using namespace std;

typedef struct NOED
{
	int value;
	int level;
	struct NOED* rightChild;
	struct NOED* leftChild;
}Node;

/*important Node* &root1*/
void meld(Node* &root1, Node* &root2){
	if(!root1){
		root1=root2;
		return;
	}
	if(!root2)
		return;
	if(root1->value > root2->value){
		Node* p=root1;
		root1=root2;
		root2=p;
	}
	
 	meld(root1->rightChild,root2);

	Node* q = root1->rightChild;
	root1->rightChild = root1->leftChild;
	root1->leftChild = q;
}

void meldNode(Node* &root, int value){
	Node* node= new Node;
	node->value=value;
	node->leftChild=NULL;
	node->rightChild=NULL;
	meld(root,node);
}

Node* createTree(int startValue){
	Node* root= new Node;
	root->value=startValue;
	root->leftChild=NULL;
	root->rightChild=NULL;
	return root;
}

void printTree(Node* root){
	queue<Node*> que;
	que.push(root);
	root->level=0;
	while(!que.empty()){
		Node* node=que.front();
		que.pop();
		cout<<node->value<<" "<<node->level<<" "<<endl;
		if(node->leftChild){
			node->leftChild->level=node->level+1;
			que.push(node->leftChild);
		}
		if(node->rightChild){
			node->rightChild->level=node->level+1;
			que.push(node->rightChild);
		}

	}
}

int main(int argc, char const *argv[])
{
	Node *root=createTree(43);
	srand((unsigned)time(0)); 
	for(int i=0;i<1000;i++)
		meldNode(root,rand()%100);

	printTree(root);
	return 0;
}