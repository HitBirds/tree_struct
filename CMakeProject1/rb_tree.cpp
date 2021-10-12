#include <vector>
using namespace std;
#include "rb_tree.h"
template<class T>
RB_Tree<T>::RB_Tree(T root_data)
{
    this->root = new My_RB_Tree_Node<T>(root_data);
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
    return Insert_Case1(N);
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
    return Insert_Case2(n_node);
}

//case 2 N��P�Ǻ�ɫʱ ֱ�Ӳ��벻Ӱ�� �����������4��5
template<class T>
void RB_Tree<T>::Insert_Case2(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node->color_tag == BLACK)
	{
		return;
	}
    return Insert_Case3(n_node);
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
	My_RB_Tree_Node<T> *cur(nullptr);
	
	int res(0);
	cur = Search_Node(delete_data);
	if (cur == nullptr)res = -1;
	else
	{
		//delete case1
		Delete_Case1(cur);
	}
	return res;
}

//case1 ��ɾ���ڵ�P����������(����ָ��Ҷ�ӽڵ㣬ע��:wiki���������3��NIL�ڵ���Ҷ��)
template<class T>
void RB_Tree<T>::Delete_Case1(My_RB_Tree_Node<T>* p_node)
{
	if (p_node->left_child != nullptr && p_node->right_child != nullptr)
	{
		vector<My_RB_Tree_Node<T>*> mid_vec;
		Mid_Traversal(this->root, &mid_vec);
		auto it = mid_vec.front();
		while (it != mid_vec.end())
		{
			if (*it == p_node)break;
			++it;
		}
		//������ֱ�Ӻ�����滻 //������������ô���ܻ�û��ֱ�Ӻ��?���Բ����ж�it != mid_vec.end()
		T tmp = p_node->data;
		p_node->data = (*it)->data;
		//p_nodeָ��ֱ�Ӻ��
		p_node = *it;
		p_node->data = tmp;
	}
	//�滻���,תcase2
	return Delete_Case2(p_node);
}


//��ɾ���ڵ�û�ж���(ֻ������Ҷ��(nil))����ѡ1��Ҷ�ӿ������ӽڵ�(�ٶ���)
//CASE2 ��ɾ�������1������
template<class T>
void RB_Tree<T>::Delete_Case2(My_RB_Tree_Node<T>* p_node)
{
    //2-1  ����ɾ�����Ϊ��ɫʱ(���ݺ���������֪,�丸�Ӷ���Ϊ��ɫ,������Ϊnil)ֱ���ö��ӽ���滻
    if(p_node == nullptr)return;
    if(p_node->color_tag == RED)
    {
        auto g=p_node->father_node;
        if(p_node == g->left_child)g->left_child = p_node->left_child;
        else g->right_child = p_node->left_child;
        erase_Node(p_node);
        delete p_node;
        p_node = nullptr;
    }
    else return Delete_Case3(p_node);
}

//2-2  ��ɾ������Ǻ�ɫ
//-1 ���������Ǻ�ɫ(��ɫ����������Ҷ��) �Ѷ��Ӹĳɺ�ɫ�����ɾ�����
template<class T>
void RB_Tree<T>::Delete_Case3(My_RB_Tree_Node<T>* p_node)
{
    if(p_node == nullptr || p_node->color_tag != BLACK)return;
    My_RB_Tree_Node<T>* c = nullptr;
    if(p_node->left_child != nullptr && p_node->left_child->color_tag == RED)
    {
        c = p_node->left_child;
        p_node->left_child = c->left_child;
        delete c->right_child;//���ڰ�ȫ���Ƕ���
        c->right_child = nullptr;
        p_node->data = c->data;
        erase_Node(c);
        delete c;
        c = nullptr;
    }
    else if(p_node->right_child != nullptr && p_node->right_child->color_tag == RED)
    {
        c = p_node->right_child;
        p_node->right_child = c->right_child;
        delete c->left_child;//���ڰ�ȫ���Ƕ���
        c->left_child = nullptr;
        p_node->data = c->data;
        erase_Node(c);
        delete c;
        c = nullptr;
    }
    return Delete_Case4(p_node);
}

//2-2
//-2  ��ɾ������Ǻ�ɫ,������Ҳ�Ǻ�ɫ(���ݺ�����Ķ��������������ӱ�ΪҶ��)
template<class T>
void RB_Tree<T>::Delete_Case4(My_RB_Tree_Node<T>* p_node)
{
    if(p_node == nullptr)return;
    auto G = p_node->father_node;
    //��P�����ĺ����滻,���Ķ���ΪN
    if(G != nullptr)
    {
        if(p_node == G->left_node)G->left_node = p_node->left_child;//��ʵleft_child === nullptr
        else G->right_node = p_node->left_child;
        G->left_child->father_node = G;
        G->right_child->father_node = G;
    }
    erase_Node(p_node);
    delete p_node;
    p_node = nullptr;
    return Delete_Case5(G);
}

//2-2-2
//-1 P��N���滻��,��N�Ǹ�
template<class T>
void RB_Tree<T>::Delete_Case5(My_RB_Tree_Node<T>* p_node)
{
    if(p_node == nullptr)return;
    else Delete_Case6(p_node);
}

//�滻��,N���Ǹ�,��P�ǿ�,����P��S����ɫ��������� BB BR RB
//2-2-2
//-2 P:B S:R
//��Ȼ:SL:B SR:B
template<class T>
void RB_Tree<T>::Delete_Case6(My_RB_Tree_Node<T>* p_node)
{
   My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
   if(S != nullptr && S->color_tag == RED)
   {
       S->color_tag = BLACK;
       p_node->color_tag = RED;
       if(p_node->right_child == S)Left_Rotate(S);
       else Right_Rotate(S);
   }
   //����N����һ���ֵ� SL��SR������1������,����wiki�ϵ�ת���4,5,6(��Ӧ�����8,9,10)
   Delete_Case7(p_node);
}

//2-2-2
//-3 P:B S:B
//����SL,SRΪBB
template<class T>
void RB_Tree<T>::Delete_Case7(My_RB_Tree_Node<T>* p_node)
{
    if(p_node->color_tag == BLACK){
        My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
        if(S != nullptr && is_Black(S->left_child) && is_Black(S->right_child))
        {
            S->color_tag = RED;
            //rebalance P
            Delete_Case5(p_node->father_node);
        }
    }
    return Delete_Case8(p_node);
}

template<class T>
void RB_Tree<T>::Delete_Case8(My_RB_Tree_Node<T>* p_node)
{
    if(p_node == nullptr)return;
    else Delete_Case6(p_node);
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
	Mid_Traversal(root->left_child, mid_vec);
	mid_vec->push_back(root);
	Mid_Traversal(root->right_child, mid_vec);
    return;
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

//ɾ��ʱѰ��S
template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_S_byP(My_RB_Tree_Node<T>* current_node)
{
    if (current_node != nullptr)
	{
        if(current_node->left_child == nullptr)return current_node->right_child;
        else return current_node->left_child;
    }
	return nullptr;
}

template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_S_byN(My_RB_Tree_Node<T>* current_node)
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
    if(current_node == nullptr)return;
	current_node->data = NULL;
	current_node->color_tag = Null;
	current_node->father_child = nullptr;
	current_node->left_child = nullptr;
	current_node->right_child = nullptr;
}
