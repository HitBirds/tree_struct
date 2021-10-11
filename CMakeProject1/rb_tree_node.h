#pragma once
/*
 *红黑树节点类
 * 成员：模板数据，父指针，左孩子指针，右孩子指针，颜色
*/
enum RB_COLOR { Null, RED = 1, BLACK };

template<class T>
class My_RB_Tree_Node {

public:
	My_RB_Tree_Node(T data_in);
	~My_RB_Tree_Node();
	T data;
	My_RB_Tree_Node* father_node, left_child, right_child;
	RB_COLOR color_tag;
};