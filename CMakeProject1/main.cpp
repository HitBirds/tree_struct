// CMakeProject1.cpp: 定义应用程序的入口点。
//
#include "main.h"
#include "stdlib.h"
#include "rb_tree_node.h"
#include "rb_tree.h"
#include <iostream>
using namespace std;

int main()
{
    RB_Tree<int> test_tree;
    /*for(int i=0;i<=766;++i)
    {
        test_tree.Insert_Node(rand()%1000-500);
    }
    vector<My_RB_Tree_Node<int>*> mid_vec;
    test_tree.Mid_Traversal(&mid_vec);
    auto it = mid_vec.begin();
    while(it != mid_vec.end())
    {
        cout<<(*it)->data<<'\b';
    }
    test_tree.print_Tree();*/
	return 0;
}
