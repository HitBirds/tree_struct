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
* 左旋 P,R不为空
* 只是更改R所指向节点里指针的指向，不更改指针R本身的值
*/
template<class T>
void RB_Tree<T>::Left_Rotate(My_RB_Tree_Node<T>* r_node)
{
	/* r_node不为空,r_node->father不为空,对R和P进行旋转 */
	
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
* 右旋 P,L不为空
* 只是更改L所指向节点里指针的指向，不更改指针L本身的值
*/
template<class T>
void RB_Tree<T>::Right_Rotate(My_RB_Tree_Node<T>* l_node)
{
	/* l_node不为空,l_node->father不为空,对L和P进行旋转 */
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
* 根据维基百科 https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
* 先查找插入位置
* 如果存在相等值就不用插入
*/
template<class T>
int RB_Tree<T>::Insert_Node(T insert_data)
{
	//寻找插入位置
	My_RB_Tree_Node<T>* pre(nullptr), *cur(nullptr);
	cur = root;
	while (cur != nullptr)
	{
		//值已存在，不操作，返回-1
		if (insert_data == cur->data)return -1;
		pre = cur;
		if (insert_data < cur->data)cur = cur->left_child;
		else cur = cur->right_child;
	}
	//先按照待插入节点是红色来插入，然后再调整
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


//插入时导致不平衡的调整 case1 N是根
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

//case 2 N的P是黑色时 直接插入不影响 红黑树的条件4，5
template<class T>
void RB_Tree<T>::Insert_Case2(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node->color_tag == BLACK)
	{
		return;
	}
	return Insert_Case3(n_node, insert_data);
}

//case 3 N的P是红色时
template<class T>
void RB_Tree<T>::Insert_Case3(My_RB_Tree_Node<T>* n_node)
{
	My_RB_Tree_Node<T>* U = Find_Node_U(n_node);
	My_RB_Tree_Node<T>* G = Find_Node_G(n_node);
	//3.1叔父是红色，调整P,U和G，把G当N入循环
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

//3.2叔父是黑色的或者为空（可能N的兄弟是黑，或者兄弟是空，所以U是黑或者空）
template<class T>
void RB_Tree<T>::Insert_Case4(My_RB_Tree_Node<T>* n_node)
{
	//3.2.1若N是P的右子 左旋N,P 变 N为P,  P为N的左子，  转3.2.2
	if (n_node == n_node->father_node->right_child)Left_Rotate(n_node);
	return Insert_Case5(n_node->left_child);
}

//case  3.2.2 N,P红,U黑(空) N为左子
template<class T>
void RB_Tree<T>::Insert_Case5(My_RB_Tree_Node<T>* n_node)
{
	//右旋P,G,对调颜色
	Right_Rotate(n_node->father_node);
	n_node->father_node->color_tag = BLACK;
	n_node->father_node->right_child->color_tag = RED;
	return;
}


/*
* 根据维基百科 https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
* 先查找删除位置进行 直接前驱和后继的替换
*/
template<class T>
int RB_Tree<T>::Delete_Node(T delete_data)
{
	//寻找删除位置
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
		//删除pre,进行‘直接前驱’替换调整
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

//中序遍历 输入root* 和 vector*，结果vector里保存了中序遍历的节点顺序
template<class T>
void RB_Tree<T>::Mid_Traversal(My_RB_Tree_Node<T>* root, vector<My_RB_Tree_Node<T>*>* mid_vec)
{
	if (root == nullptr)return;
	Mid_Traversal(root->left_child,mid_vec);
	mid_vec->push_back(root);
	Mid_Traversal(root->right_child, mid_vec);
	return void;
}


//删除时导致不平衡的调整
template<class T>
void RB_Tree<T>::Delete_Case1(My_RB_Tree_Node<T>* n_node)
{

}


//寻找G
template<class T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_G(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr)return current_node->father_node->father_node;
	return nullptr;
}

//寻找U
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

//删除时寻找儿子的叔父节点S
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

//清空节点数据
template<class T>
void RB_Tree<T>::erase_Node(My_RB_Tree_Node<T>* current_node)
{
	current_node->data = NULL;
	current_node->color_tag = Null;
	current_node->father_child = nullptr;
	current_node->left_child = nullptr;
	current_node->right_child = nullptr;
}