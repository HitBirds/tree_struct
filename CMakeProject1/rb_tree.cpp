#include <vector>
using namespace std;
#include "rb_tree.h"
template<class T>
RB_Tree<T>::RB_Tree(T root_data)
{
	this->root = new My_RB_Tree_Node(root_data);
}

template<class T>
RB_Tree<T>::~RB_Tree()
{
	delete(root);
	root = nullptr;
}

/*
* ���� P,R��Ϊ��
* ֻ�Ǹ���R��ָ��ڵ���ָ���ָ�򣬲�����ָ��R�����ֵ
*/
template<class T>
void RB_Tree<T>::Left_Rotate(My_RB_Tree_Node<T>* r_node)
{
	/* r_node��Ϊ��,r_node->father��Ϊ��,��R��P������ת */
	
	if (r_node == nullptr || r_node->father_node == nullptr)return;
	My_RB_Tree_Node<T>* p = r_node->father_node;
	T tmp = p->data;
	p->data = p->right_child->data;
	p->right_child->data = tmp;

	My_RB_Tree_Node<T>* pl= p->left_child;
	p->left_child = p->right_child;
	p->right_child = p->right_child->right_child;
	if (p->right_child != nullptr)p->right_child->father_node = p;

	p->left_child->right_child = p->left_child->left_child;
	if(p->left_child->right_child != nullptr)p->left_child->right_child->father_node = p->left_child;
	p->left_child->left_child = pl;
	if (p->left_child->left_child != nullptr)p->left_child->left_child->father_node = p->left_child;
}

/*
* ���� P,L��Ϊ��
* ֻ�Ǹ���L��ָ��ڵ���ָ���ָ�򣬲�����ָ��L�����ֵ
*/
template<class T>
void RB_Tree<T>::Right_Rotate(My_RB_Tree_Node<T>* l_node)
{
	/* l_node��Ϊ��,l_node->father��Ϊ��,��L��P������ת */
	if (l_node == nullptr || l_node->father_node == nullptr)return;
	My_RB_Tree_Node<T>* p = l_node->father_node;
	T tmp = p->data;
	p->data = p->left_child->data;
	p->left_child->data = tmp;

	My_RB_Tree_Node<T>* pr = p->right_child;
	p->right_child = p->left_child;
	p->left_child = p->left_child->left_child;
	if (p->left_child != nullptr)p->left_child->father_node = p;

	p->right_child->left_child = p->right_child->right_child;
	if (p->right_child->left_child != nullptr)p->right_child->left_child->father_node = p->right_child;
	p->right_child->right_child = pr;
	if (p->right_child->right_child != nullptr)p->right_child->right_child->father_node = p->right_child;	
}

/*
* ����ά���ٿ� https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
* �Ȳ��Ҳ���λ��
* ����������ֵ�Ͳ��ò���
*/
template<class T>
int RB_Tree<T>::Insert_Node(T insert_data)
{
	//Ѱ�Ҳ���λ��
	My_RB_Tree_Node<T>* pre(nullptr), *cur(nullptr);
	cur = root;
	while (cur != nullptr)
	{
		//ֵ�Ѵ��ڣ�������������-1
		if (insert_data == cur->data)return -1;
		pre = cur;
		if (insert_data < cur->data)cur = cur->left_child;
		else cur = cur->right_child;
	}
	//�Ȱ��մ�����ڵ��Ǻ�ɫ�����룬Ȼ���ٵ���
	My_RB_Tree_Node<T>* N = new My_RB_Tree_Node<T>(insert_data);
	N->color_tag = RED;
	if (pre == nullptr)
	{
		this->root = N;
	}
	else
	{
		if (insert_data < pre->data)
		{
			pre->left_child = N;
			pre->left_child->father_node = pre;
		}
		else
		{
			pre->right_child = N;
			pre->right_child->father_node = pre;
		}
	}
	return Insert_Case1(N,&insert_data);
}


//����ʱ���²�ƽ��ĵ��� case1 N�Ǹ�
template<class T>
void RB_Tree<T>::Insert_Case1(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node == nullptr)
	{
		this->root->color_tag = BLACK;
		return;
	}
	return Insert_Case2(n_node, insert_data);
}

//case 2 N��P�Ǻ�ɫʱ ֱ�Ӳ��벻Ӱ�� �����������4��5
template<class T>
void RB_Tree<T>::Insert_Case2(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node->color_tag == BLACK)
	{
		return;
	}
	return Insert_Case3(n_node, insert_data);
}

//case 3 N��P�Ǻ�ɫʱ
template<class T>
void RB_Tree<T>::Insert_Case3(My_RB_Tree_Node<T>* n_node)
{
	My_RB_Tree_Node<T>* U = Find_Node_U(n_node);
	My_RB_Tree_Node<T>* G = Find_Node_G(n_node);
	//3.1�常�Ǻ�ɫ������P,U��G����G��N��ѭ��
	if (U != nullptr && U->color_tag == RED)
	{
		n_node->father_node->color_tag = BLACK;
		U->color_tag = BLACK;
		G->color_tag = RED;
		return Insert_Case1(G);
	}
	else
	{
		return Insert_Case4(n_node);
	}
}

//3.2�常�Ǻ�ɫ�Ļ���Ϊ�գ�����N���ֵ��Ǻڣ������ֵ��ǿգ�����U�Ǻڻ��߿գ�
template<class T>
void RB_Tree<T>::Insert_Case4(My_RB_Tree_Node<T>* n_node)
{
	//3.2.1��N��P������ ����N,P �� NΪP,  PΪN�����ӣ�  ת3.2.2
	if (n_node == n_node->father_node->right_child)Left_Rotate(n_node);
	return Insert_Case5(n_node->left_child);
}

//case  3.2.2 N,P��,U��(��) NΪ����
template<class T>
void RB_Tree<T>::Insert_Case5(My_RB_Tree_Node<T>* n_node)
{
	//����P,G,�Ե���ɫ
	Right_Rotate(n_node->father_node);
	n_node->father_node->color_tag = BLACK;
	n_node->father_node->right_child->color_tag = RED;
	return;
}


/*
* ����ά���ٿ� https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
* �Ȳ���ɾ��λ�ý��� ֱ��ǰ���ͺ�̵��滻
*/
template<class T>
int RB_Tree<T>::Delete_Node(T delete_data)
{
	//Ѱ��ɾ��λ��
	My_RB_Tree_Node<T>* pre(nullptr), *cur(nullptr);
	cur = root;
	while (cur != nullptr)
	{
		pre = cur;
		if (insert_data == cur->data)break;
		if (insert_data < cur->data)cur = cur->left_child;
		else cur = cur->right_child;
	}
	if (pre == nullptr)return -1;
	else
	{
		//ɾ��pre,���С�ֱ��ǰ�����滻����
	}
	return 0;
}

template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Search_Node(T sr_data)
{
	My_RB_Tree_Node<T>* cur = root;
	while (cur != nullptr)
	{
		if (cur->data == sr_data)return cur;
		if (sr_data < cur->data)cur = cur->left_child;
		else cur = cur->right_child;
	}
	return cur;
}

//������� ����root* �� vector*�����vector�ﱣ������������Ľڵ�˳��
template<class T>
void RB_Tree<T>::Mid_Traversal(My_RB_Tree_Node<T>* root, vector<My_RB_Tree_Node<T>*>* mid_vec)
{
	if (root == nullptr)return;
	Mid_Traversal(root->left_child,mid_vec);
	mid_vec->push_back(root);
	Mid_Traversal(root->right_child, mid_vec);
	return void;
}


//ɾ��ʱ���²�ƽ��ĵ���
template<class T>
void RB_Tree<T>::Delete_Case1(My_RB_Tree_Node<T>* n_node)
{

}


//Ѱ��G
template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_G(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr)return current_node->father_node->father_node;
	return nullptr;
}

//Ѱ��U
template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_U(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr && current_node->father_node->father_node != nullptr)
	{
		if (current_node->father_node == current_node->father_node->father_node->left_child)return current_node->father_node->father_node->right_child;
		else return current_node->father_node->father_node->left_child;
	}
	return nullptr;
}

//ɾ��ʱѰ�Ҷ��ӵ��常�ڵ�S
template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_S(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr)
	{
		if (current_node == current_node->father_node->left_child)return current_node->father_node->right_child;
		else return current_node->father_node->left_child;
	}
	return nullptr;
}

//��սڵ�����
template<class T>
void RB_Tree<T>::erase_Node(My_RB_Tree_Node<T>* current_node)
{
	current_node->data = NULL;
	current_node->color_tag = Null;
	current_node->father_child = nullptr;
	current_node->left_child = nullptr;
	current_node->right_child = nullptr;
}