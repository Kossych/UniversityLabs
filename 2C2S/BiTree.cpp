#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>

using namespace std;

class Node{
    int key;
    Node *left, *right;
    public:
    Node(int k=0, Node*l=NULL,Node*r=NULL){key=k; left=l; right= r;}
    void print();
    friend class BinaryTree;
    friend class FTree;
};

class BinaryTree{
    Node *root;
    public:

    void Tree(){root=NULL;}

    BinaryTree(int n){root=BTree(n);}
    BinaryTree(const BinaryTree&);

    Node *BTree(int n);
    Node *CopyTree(Node *q);
    Node *GetRoot(){return root;}
    void PrintTree(Node *q, int k);

    int LNR(int*);
    void inarray(Node*,int *,int &);

    int min_key();
    int min_r(Node *q);
};

void Node::print(){
    if(left==nullptr && right==nullptr){
        cout<<key<<" ";
        return;
    }
    left->print();
    right->print();
}

Node* BinaryTree::BTree(int n){
    if(n==0) return NULL;
    Node *q=new Node(rand()%100);
    int n1=n/2;
    q->left=BTree(n1);
    q->right=BTree(n-1-n1);
    return q;
}

BinaryTree::BinaryTree(const BinaryTree &BT){
    if(BT.root==NULL) root=NULL;
    else{
        root=new Node(BT.root->key);
        root->left=CopyTree(BT.root->left);
        root->right=CopyTree(BT.root->right);
    }
}

Node* BinaryTree::CopyTree(Node *q){
    if(q==NULL) return NULL;
    Node *p=new Node(q->key);
    p->left=CopyTree(q->left);
    p->right=CopyTree(q->right);
    return p;
}

void BinaryTree::PrintTree(Node *q, int k){
    if(q==NULL) return;
    PrintTree(q->left,k+3);
    for(int i=0;i<k;i++) cout<<" ";
    cout.width(2);
    cout<<q->key<<endl;
    PrintTree(q->right,k+3);
}

int BinaryTree::LNR(int *a){
    int k=0;
    inarray(root,a,k);
    return k;
}

void BinaryTree::inarray(Node *q, int *a, int &k){
    if(q==NULL) return;
    inarray(q->left,a,k);
    a[k++]=q->key;
    inarray(q->right,a,k);
}

int BinaryTree::min_key(){
    if(root==NULL){ cout<<"Дерево пустое"; return -1;}
    return min_r(root);
}

int BinaryTree::min_r(Node *q){
    int m,m1;
    m=q->key;
    if(q->key){ m1=min_r(q->left); if(m1<m) m=m1;}
    if(q->right){ m1=min_r(q->right); if(m1<m) m=m1;}
    return m;
}


int main(){
    srand(time(0));
    BinaryTree A(10);
    A.PrintTree(A.GetRoot(),0);
}