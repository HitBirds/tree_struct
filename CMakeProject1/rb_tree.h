#pragma once
#include <vector>
using namespace std;
#include "rb_tree_node.h"
#include "rb_tree_node.cpp"
/*
 *封装RB_NODE的树结构
 *数据成员:root
 *操作:初始化，析构，左旋转，右旋转，插入节点，删除节点，
 *中序遍历，清空节点数据,查找结点
*/
template<typename T>
class RB_Tree
{
public:
	My_RB_Tree_Node<T>* root;
	RB_Tree(T root_data);
	~RB_Tree();
	void Left_Rotate(My_RB_Tree_Node<T>* r_node);
	void Right_Rotate(My_RB_Tree_Node<T>* l_node);
	int Insert_Node(T insert_data);
	int Delete_Node(T delete_data);
	My_RB_Tree_Node<T>* Search_Node(T sr_data);
	//中序遍历 输入root* 和 vector*，结果vector里保存了中序遍历的节点顺序，也可以自己用二级指针来实现vector
    void Mid_Traversal(vector<My_RB_Tree_Node<T>*>* mid_vec){Mid_Traversal(mid_vec, root);}
    void Mid_Traversal(vector<My_RB_Tree_Node<T>*>* mid_vec, My_RB_Tree_Node<T>* root);
    //打印树
    void print_Tree();
private:
	//插入时导致不平衡的调整
	int Insert_Case1(My_RB_Tree_Node<T>* n_node);
	int Insert_Case2(My_RB_Tree_Node<T>* n_node);
	int Insert_Case3(My_RB_Tree_Node<T>* n_node);
	int Insert_Case4(My_RB_Tree_Node<T>* n_node);
	int Insert_Case5(My_RB_Tree_Node<T>* n_node);
	//删除时导致不平衡的调整
	int Delete_Case1(My_RB_Tree_Node<T>* p_node);
	int Delete_Case2(My_RB_Tree_Node<T>* p_node);
	int Delete_Case3(My_RB_Tree_Node<T>* p_node);
	int Delete_Case4(My_RB_Tree_Node<T>* p_node);
	int Delete_Case5(My_RB_Tree_Node<T>* p_node);
	int Delete_Case6(My_RB_Tree_Node<T>* p_node);
	int Delete_Case7(My_RB_Tree_Node<T>* p_node);
	int Delete_Case8(My_RB_Tree_Node<T>* p_node);
	int Delete_Case9(My_RB_Tree_Node<T>* p_node);
	int Delete_Case10(My_RB_Tree_Node<T>* p_node);
	
    //判断黑 隐式内联
    bool is_Black(My_RB_Tree_Node<T>* c)
    {
        return c == nullptr || c->color_tag == BLACK;
    }

	//寻找G
	My_RB_Tree_Node<T>* Find_Node_G(My_RB_Tree_Node<T>* current_node);
	//寻找U
	My_RB_Tree_Node<T>* Find_Node_U(My_RB_Tree_Node<T>* current_node);
	//删除时寻找兄弟节点S
    My_RB_Tree_Node<T>* Find_Node_S_byN(My_RB_Tree_Node<T>* current_node);
    My_RB_Tree_Node<T>* Find_Node_S_byP(My_RB_Tree_Node<T>* p_node);

	//清空节点数据
	void erase_Node(My_RB_Tree_Node<T>* current_node);


};
