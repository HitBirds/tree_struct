#include "rb_tree_node.h"

template<typename T>
My_RB_Tree_Node<T>::My_RB_Tree_Node(T data_in):data(data_in),color_tag(RED),father_node(nullptr),left_child(nullptr),right_child(nullptr)
{
	data = data_in;
	color_tag = RED;
}

template<typename T>
My_RB_Tree_Node<T>::~My_RB_Tree_Node() {
	delete left_child;
	left_child = nullptr;
	delete right_child;
	right_child = nullptr;
	father_node = nullptr;
}