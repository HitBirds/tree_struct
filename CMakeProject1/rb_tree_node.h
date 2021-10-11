#pragma once
/*
 *������ڵ���
 * ��Ա��ģ�����ݣ���ָ�룬����ָ�룬�Һ���ָ�룬��ɫ
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