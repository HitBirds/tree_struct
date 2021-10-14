#include <iostream>
#include <vector>
#include <queue>
#include "rb_tree.h"

using namespace std;


template<typename T>
RB_Tree<T>::RB_Tree(T root_data)
{
	if (root_data != NULL) {
		this->root = new My_RB_Tree_Node<T>(root_data);
		root->color_tag = BLACK;
	}
	else root = nullptr;
}

template<typename T>
RB_Tree<T>::~RB_Tree()
{
	delete(root);
	root = nullptr;
}

/*
* 左旋 P,R不为空
* 只是更改R和P所指向节点里指针的指向和成员值，不更改指针R或P本身的指向
* R指向的空间里保存的是旧的*P的内容
*/
template<typename T>
void RB_Tree<T>::Left_Rotate(My_RB_Tree_Node<T>* r_node)
{
	/* r_node不为空,r_node->father不为空,对R和P进行旋转 */
	
	if (r_node == nullptr || r_node->father_node == nullptr)return;
	My_RB_Tree_Node<T>* p = r_node->father_node;
	T tmp = p->data;
    RB_COLOR tempc = p->color_tag;
	p->data = p->right_child->data;
    p->color_tag = p->right_child->color_tag;
	p->right_child->data = tmp;
    p->right_child->color_tag = tempc;

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
* 只是更改L所指向节点里指针的指向和成员值，不更改指针L或P本身的指向
* L指向的空间里保存的是旧的*P的内容
*/
template<typename T>
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
template<typename T>
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
    return Insert_Case1(N);
}


//插入时导致不平衡的调整 case1 N是根
template<typename T>
int RB_Tree<T>::Insert_Case1(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node == nullptr)
	{
		this->root->color_tag = BLACK;
		return 1;
	}
    return Insert_Case2(n_node);
}

//case 2 N的P是黑色时 直接插入不影响 红黑树的条件4，5
template<typename T>
int RB_Tree<T>::Insert_Case2(My_RB_Tree_Node<T>* n_node)
{
	if (n_node->father_node->color_tag == BLACK)
	{
		return 1;
	}
    return Insert_Case3(n_node);
}

//case 3 N的P是红色时
template<typename T>
int RB_Tree<T>::Insert_Case3(My_RB_Tree_Node<T>* n_node)
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
template<typename T>
int RB_Tree<T>::Insert_Case4(My_RB_Tree_Node<T>* n_node)
{
    //3.2.1若N是P的右子 左旋N,P 变 N为P,  P为G的左子，  转3.2.2
    if (n_node == n_node->father_node->right_child && n_node->father_node == n_node->father_node->father_node->left_child)Left_Rotate(n_node);
    else if(n_node == n_node->father_node->left_child && n_node->father_node == n_node->father_node->father_node->right_child)Right_Rotate(n_node);
    return Insert_Case5(n_node);
}

//case  3.2.2 N,P红,U黑(空) 分P为左子或右
template<typename T>
int RB_Tree<T>::Insert_Case5(My_RB_Tree_Node<T>* n_node)
{
	//右旋P,G,对调颜色
    if(n_node->father_node == n_node->father_node->father_node->left_child){
        Right_Rotate(n_node->father_node);
        n_node->father_node->color_tag = BLACK;
        n_node->father_node->right_child->color_tag = RED;
    }else{
        Left_Rotate(n_node->father_node);
        n_node->father_node->color_tag = BLACK;
        n_node->father_node->left_child->color_tag = RED;
    }
	return 1;
}


/*
* 根据维基百科 https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
* 先查找删除位置进行 直接前驱和后继的替换
*/
template<typename T>
int RB_Tree<T>::Delete_Node(T delete_data)
{
	//寻找删除位置
	My_RB_Tree_Node<T> *cur(nullptr);
	
	int res(0);
	cur = Search_Node(delete_data);
	if (cur == nullptr)res = -1;
	else
	{
		//delete case1
		res = Delete_Case1(cur);
	}
	return res;
}


//case1 待删除节点P有两个儿子(儿子指非叶子节点，注意:wiki红黑树定义3，NIL节点是叶子)
template<typename T>
int RB_Tree<T>::Delete_Case1(My_RB_Tree_Node<T>* p_node)
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
		//这里用直接后继来替换 //有两个儿子怎么可能会没有直接后继?所以不用判断it != mid_vec.end()
		T tmp = p_node->data;
		p_node->data = (*it)->data;
		//p_node指向直接后继
		p_node = *it;
		p_node->data = tmp;
	}
	//替换完毕,转case2
	return Delete_Case2(p_node);
}


//待删除节点没有儿子(只有两个叶子(nil))则任选1个叶子看作儿子节点(假儿子)
//CASE2 待删除结点有1个儿子
template<typename T>
int RB_Tree<T>::Delete_Case2(My_RB_Tree_Node<T>* p_node)
{
    //2-1  当待删除结点为红色时(根据红黑树定义可知,其父子都不为红色,且其子为nil)直接用儿子结点替换
    if(p_node != nullptr && p_node->color_tag == RED)
    {
        auto g=p_node->father_node;
        if(p_node == g->left_child)g->left_child = p_node->left_child;
        else g->right_child = p_node->left_child;
        erase_Node(p_node);
        delete p_node;
        p_node = nullptr;
		return 1;
    }
    else return Delete_Case3(p_node);
}

//2-2  待删除结点是黑色
//-1 但它儿子是红色(红色必有两个空叶子) 把儿子改成黑色顶替待删除结点
template<typename T>
int RB_Tree<T>::Delete_Case3(My_RB_Tree_Node<T>* p_node)
{
	if(p_node != nullptr && p_node->color_tag == BLACK)
	{ 
		My_RB_Tree_Node<T>* c = nullptr;
		if( p_node->left_child != nullptr && p_node->left_child->color_tag == RED)
		{
			c = p_node->left_child;
			p_node->left_child = c->left_child;
      
			p_node->data = c->data;
			erase_Node(c);
			delete c;
			c = nullptr;
			return 1;
		}
		else if(p_node->right_child != nullptr && p_node->right_child->color_tag == RED)
		{
			c = p_node->right_child;
			p_node->right_child = c->right_child;
      
			p_node->data = c->data;
			erase_Node(c);
			delete c;
			c = nullptr;
			return 1;
		}
		return 0;
	}
    else return Delete_Case4(p_node);
}

//2-2
//-2  待删除结点是黑色,它儿子也是黑色(根据红黑树的定义它的两个儿子必为叶子)
template<typename T>
int RB_Tree<T>::Delete_Case4(My_RB_Tree_Node<T>* p_node)
{
	if (p_node != nullptr && p_node->color_tag == BLACK) {
		auto G = p_node->father_node;
		//把P用它的孩子替换,它的儿子为N
		if (G != nullptr)
		{
			if (p_node == G->left_node)G->left_node = p_node->left_child;//其实left_child === nullptr
			else G->right_node = p_node->left_child;
		}
		erase_Node(p_node);
		delete p_node;
		p_node = nullptr;
		return Delete_Case5(G);
	}
	return 0;
}

//2-2-2 P黑N黑替换后
//-1 若N是根
template<typename T>
int RB_Tree<T>::Delete_Case5(My_RB_Tree_Node<T>* p_node)
{
    if(p_node == nullptr)return 1;
    else Delete_Case6(p_node);
}

//替换后,N不是根,则P非空,根据P和S的颜色分情况讨论 BB BR RB
//2-2-2
//-2 P:B S:R  ->BR
//必然:SL:B SR:B
template<typename T>
int RB_Tree<T>::Delete_Case6(My_RB_Tree_Node<T>* p_node)
{
	if(p_node != nullptr && p_node->color_tag == BLACK){
		My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
		if(S != nullptr && S->color_tag == RED)
		{
			if(p_node->right_child == S)Left_Rotate(S);
			else Right_Rotate(S);
			//我的旋转算法使得调用后 原来的P指针指向保存S节点的值，S指针指向保存P点的值 保存S节点的值是红色，保存P值的节点是黑色
			S->color_tag = RED;
			p_node->color_tag = BLACK;
			p_node = S;
		}
	}
	//由于N多了一个兄弟 SL和SR都是孩子,N的P变成了红色,S变成了SL Black孩子 ,BR变成了 RB 的情况,按照wiki上的转情况4,5,6(对应这里的8,9,10)
	return Delete_Case7(p_node);
}

//2-2-2
//-3 P:B S:B  ->BB
//-1 假设SL,SR为BB(两个叶子) 注意：2-2-2-3-2~4 SL,SR为RB BR RR三种情况还没讨论
template<typename T>
int RB_Tree<T>::Delete_Case7(My_RB_Tree_Node<T>* p_node)
{
    if(p_node != nullptr && p_node->color_tag == BLACK){
        My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
        if(S != nullptr && S->color_tag == BLACK && is_Black(S->left_child) && is_Black(S->right_child))
        {
            S->color_tag = RED;
            //转BR 再平衡P
            return Delete_Case5(p_node->father_node);
        }
    }
    return Delete_Case8(p_node);
}

//2-2-2
//-4 P:R S:B -> RB
//-1 假设SL,SR为BB(两个叶子)
template<typename T>
int RB_Tree<T>::Delete_Case8(My_RB_Tree_Node<T>* p_node)
{
	if (p_node != nullptr && p_node->color_tag == RED)
	{
		My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
		if (S != nullptr && S->color_tag == BLACK && is_Black(S->left_child) && is_Black(S->right_child))
		{
			S->color_tag = RED;
			p_node->color_tag = BLACK;
			//已经平衡了
			return 1;
		}
	}
	return Delete_Case9(p_node);
}

//在PS为 RB或BB的情况下 考虑SL,SR
//2-2-2
//-4 P:R S:B -> RB   ||   -3 P:B S:B -> BB
//-2 假设SL,SR为RB的组合(其中B必为黑空叶子) 分(R/B)->(右)B->(RB) || (R/B)->(左)B->(BR) 把这两个都调整为(R/B)->B->R->(B)
template<typename T>
int RB_Tree<T>::Delete_Case9(My_RB_Tree_Node<T>* p_node)
{
	if (p_node != nullptr)
	{
		My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
		if (S != nullptr && S->color_tag == BLACK)
		{
			if (S == S->father_node->right_child && !is_Black(S->left_child) && is_Black(S->right_child))
			{
				S->left_child->color_tag = BLACK;
				S->color_tag = RED;
				Right_Rotate(S->left_child);
				//我的旋转算法使得P永远是P 此处S为逻辑上的P
			}
			else if (S == S->father_node->left_child && is_Black(S->left_child) && !is_Black(S->right_child))
			{
				S->right_child->color_tag = BLACK;
				S->color_tag = RED;
				Left_Rotate(S->right_child);
			}
			return Delete_Case10(p_node);
		}
	}
}


//[!2-2-2-3-2~4 SL,SR为RR还没讨论]
//现在有 (R/B)->B->R->(B) 或 (R/B)->B->(RR)
template<typename T>
int RB_Tree<T>::Delete_Case10(My_RB_Tree_Node<T>* p_node)
{
	if (p_node != nullptr)
	{
		My_RB_Tree_Node<T>* S = Find_Node_S_byP(p_node);
		if (S != nullptr)
		{
			S->color_tag = p_node->color_tag;
			p_node->color_tag = BLACK;
		}
		if (S == p_node->left_child)
		{
			S->left_child->color_tag = BLACK;
			Right_Rotate(S);
		}
		else {
			S->right_child->color_tag = BLACK;
			Left_Rotate(S);
		}
	}
	return 1;
}


template<typename T>
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
template<typename T>
void RB_Tree<T>::Mid_Traversal(vector<My_RB_Tree_Node<T>*>* mid_vec, My_RB_Tree_Node<T>* root)
{
	if (root == nullptr)return;
	Mid_Traversal( mid_vec, root->left_child);
	mid_vec->push_back(root);
	Mid_Traversal( mid_vec, root->right_child);
    return;
}


//寻找G
template<typename T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_G(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr)return current_node->father_node->father_node;
	return nullptr;
}

//寻找U
template<typename T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_U(My_RB_Tree_Node<T>* current_node)
{
	if (current_node != nullptr && current_node->father_node != nullptr && current_node->father_node->father_node != nullptr)
	{
		if (current_node->father_node == current_node->father_node->father_node->left_child)return current_node->father_node->father_node->right_child;
		else return current_node->father_node->father_node->left_child;
	}
	return nullptr;
}

//删除时寻找S
template<typename T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_S_byP(My_RB_Tree_Node<T>* current_node)
{
    if (current_node != nullptr)
	{
        if(current_node->left_child == nullptr)return current_node->right_child;
        else return current_node->left_child;
    }
	return nullptr;
}

template<typename T>
My_RB_Tree_Node<T>* RB_Tree<T>::Find_Node_S_byN(My_RB_Tree_Node<T>* current_node)
{
    if (current_node != nullptr && current_node->father_node != nullptr)
    {
        if (current_node == current_node->father_node->left_child)return current_node->father_node->right_child;
        else return current_node->father_node->left_child;
    }
    return nullptr;
}

//清空节点数据
template<typename T>
void RB_Tree<T>::erase_Node(My_RB_Tree_Node<T>* current_node)
{
    if(current_node == nullptr)return;
	current_node->data = NULL;
	current_node->color_tag = Null;
	current_node->father_child = nullptr;
	current_node->left_child = nullptr;
	current_node->right_child = nullptr;
}

template<typename T>
void RB_Tree<T>::print_Tree()
{
    int prev_cnt(0),cur_cnt(0);
    queue<My_RB_Tree_Node<T>*> q;
    if(root != nullptr){
        q.push(root);
        prev_cnt += 1;
    }
    while(!q.empty())
    {
        My_RB_Tree_Node<T>* cur=q.front();
        q.pop();
		if (cur == nullptr)cout << 'n';
		else {
			if (cur->color_tag == BLACK)cout << 'B';
			else cout << 'R';
			cout << cur->data;
			q.push(cur->left_child);
			++cur_cnt;
			q.push(cur->right_child);
			++cur_cnt;
		}
        prev_cnt-=1;
        if(prev_cnt == 0)
        {
            cout<<endl;
            prev_cnt = cur_cnt;
            cur_cnt = 0;
        }
        else{
            cout<<'\t';
        }
    }
}
