// CMakeProject1.cpp: 定义应用程序的入口点。
//
#include "main.h"
#include "stdlib.h"
#include "rb_tree.h"
#include "rb_tree.cpp"
#include <iostream>
using namespace std;

int main()
{
    RB_Tree< int > test_tree(33);
    int cnt = 0,res = 0;
    for(int i=0;i<=550;++i)
    {
        res = test_tree.Insert_Node(rand()%1000-500);
        if (res != 1)++cnt;
    }
    cout <<"插入了" << cnt << "个重复值" << endl;
    vector<My_RB_Tree_Node<int>*> mid_vec;
    test_tree.Mid_Traversal(&mid_vec);
    auto it = mid_vec.begin();
    while(it != mid_vec.end())
    {
        cout<<(*it)->data<<'\t';
        it++;
    }
    cout << endl;
    test_tree.print_Tree();
    cnt = 0;
    for (int i = 0; i <= 550; ++i)
    {
        int tt = rand() % 1000 - 500;
        res = test_tree.Delete_Node(tt);
        if (res == 1) {
            ++cnt; cout << "删除了" << tt << endl;
        }
    }
    cout << "删除了" << cnt << "个值" << endl;
    
    test_tree.print_Tree();
    cout << endl;
	return 0;
}
