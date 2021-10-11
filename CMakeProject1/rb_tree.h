#pragma once
#include <vector>
#include "rb_tree_node.h"
/*
 *��װRB_NODE�����ṹ
 *���ݳ�Ա:root
 *����:��ʼ��������������ת������ת������ڵ㣬ɾ���ڵ㣬
 *�����������սڵ�����,���ҽ��
*/
template<class T>
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
	//������� ����root* �� vector*�����vector�ﱣ������������Ľڵ�˳��Ҳ�����Լ��ö���ָ����ʵ��vector
	void Mid_Traversal(My_RB_Tree_Node<T>* root,vector<My_RB_Tree_Node<T>*>* mid_vec);
private:
	//����ʱ���²�ƽ��ĵ���
	void Insert_Case1(My_RB_Tree_Node<T>* p_node, const T& insert_data);
	void Insert_Case2(My_RB_Tree_Node<T>* p_node, const T& insert_data);
	void Insert_Case3(My_RB_Tree_Node<T>* p_node, const T& insert_data);
	void Insert_Case4(My_RB_Tree_Node<T>* p_node, const T& insert_data);
	void Insert_Case5(My_RB_Tree_Node<T>* p_node, const T& insert_data);
	//ɾ��ʱ���²�ƽ��ĵ���
	void Fix_Tree_Delete(My_RB_Tree_Node<T>* current_node);

	
	//Ѱ��G
	My_RB_Tree_Node<T>* Find_Node_G(My_RB_Tree_Node<T>* current_node);
	//Ѱ��U
	My_RB_Tree_Node<T>* Find_Node_U(My_RB_Tree_Node<T>* current_node);
	//ɾ��ʱѰ���ֵܽڵ�S
	My_RB_Tree_Node<T>* Find_Node_S(My_RB_Tree_Node<T>* current_node);

	//��սڵ�����
	void erase_Node(My_RB_Tree_Node<T>* current_node);
};
