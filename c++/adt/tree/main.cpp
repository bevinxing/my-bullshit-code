#include <iostream>
#include "Tree.h"
#include "Node.h"

using namespace std;

/*
 *          0(0)
 *     5(1)     8(2)
 * 2(3)  6(4) 9(5)   7(6)
 */
int main() {
    Tree *pTree = new Tree();

    Node *pNode1 = new Node();
    Node *pNode2 = new Node();
    Node *pNode3 = new Node();
    Node *pNode4 = new Node();
    Node *pNode5 = new Node();
    Node *pNode6 = new Node();

    pNode1->index = 1;
    pNode2->index = 2;
    pNode3->index = 3;
    pNode4->index = 4;
    pNode5->index = 5;
    pNode6->index = 6;

    pNode1->data = 5;
    pNode2->data = 8;
    pNode3->data = 2;
    pNode4->data = 6;
    pNode5->data = 9;
    pNode6->data = 7;

    pTree->AddNode(0, 0, pNode1);
    pTree->AddNode(0, 1, pNode2);
    pTree->AddNode(1, 0, pNode3);
    pTree->AddNode(1, 1, pNode4);
    pTree->AddNode(2, 0, pNode5);
    pTree->AddNode(2, 1, pNode6);

    pTree->PreorderTraversal();
    cout << endl;
    pTree->InorderTraversal();
    cout << endl;
    pTree->PostorderTraversal();

    //pTree->DeleteNode(2, NULL);
    //pTree->DeleteNode(1, NULL);
    pTree->DeleteNode(5, NULL);
    pTree->PreorderTraversal();


    delete pTree;
    return 0;
}