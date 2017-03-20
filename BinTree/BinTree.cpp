#include <cstdio>
#include <cmath>
#include <iostream>
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height:-1)//节点高度(与空数节点高度为-1约定相统一)
using namespace std;

typedef int Rank;
#define DEFAULT_CAPACITY 3

#define ListNodePosi(T) ListNode<T>*
template<typename T> struct ListNode
{
	//成员
	T data;//数值
	ListNodePosi(T) pred;//前驱
	ListNodePosi(T) succ;//后驱
						 //构造函数
	ListNode() {}//针对header和trailer的构造
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
	{
		pred = p;
		succ = s;
		data = e;
	}//默认构造函数
	 //操作接口
	ListNodePosi(T) insertAsPred(T const& e) {
		ListNodePosi(T) x = new ListNode(e, pred, this);
		pred->succ = x;
		pred = x;
		return x;
	};
	ListNodePosi(T) insertAsSucc(T const& e) {
		ListNodePosi(T) x = new ListNode(e, this, succ);
		succ->pred = x;
		succ = x;
		return x;
	};
};

template <typename T>class Queue {
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T)trailer;

	//插入末节点
	ListNodePosi(T) insertAsLast(T const&e) {
		_size++; return trailer->insertAsPred(e);
	}

	//删除节点
	T remove(ListNodePosi(T) p) {
		T e = p->data;
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		delete p; _size--;
		return e;
	}

	ListNodePosi(T) first()const { return header->succ; }
protected:
	void init() {
		header = new ListNode<T>;
		trailer = new ListNode<T>;
		header->succ = trailer; header->pred = NULL;
		trailer->pred = header; trailer->succ = NULL;
		_size = 0;
	};

	//清空列表
	int clear() {
		int oldSize = _size;
		while (0 < _size)
		{
			remove(header->succ);
		}
		return oldSize;
	}
public:
	Queue() {
		init();
	}
	~Queue() {
		clear();
		delete header; delete trailer;
	}
	//入队:尾部入队
	void enqueue(T const&e) {
		insertAsLast(e);
	}
	//出队:头部出队
	T dequeue() {
		return remove(first());
	}
	//队首
	T& front() {
		return first()->data;
	}
	//规模
	int size() {
		return _size;
	}
	//判空
	bool empty() {
		return _size == 0;
	}
};

//栈
template <typename T> class Stack
{
protected:
	Rank _size; int _capacity; T*_elem;

public:
	//构造函数
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//析构函数
	~Stack() {
		delete[]_elem;
	}

	//入栈
	void push(T const& e) {
		insert(size(), e);
	}

	//出栈
	T pop() {
		return remove(size() - 1);
	}

	//取顶
	T& top() {
		return _elem[_size - 1];
	}

	//返回栈大小
	Rank size() const {
		return _size;
	}

	bool isEmpty() {
		return _size == 0;
	}

private:

	Rank insert(Rank r, T const& e) {
		expand();
		for (int i = _size; i > r; i--) {
			_elem[i] = _elem[i - 1];
		}
		_elem[r] = e; _size++;
		return r;
	}

	void expand() {
		if (_size < _capacity)return;
		_capacity = _capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : _capacity;
		T*oldElem = _elem; _elem = new T[_capacity <<= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[] oldElem;
	}

	int remove(Rank lo, Rank hi) {
		if (lo == hi)return 0;
		while (hi < _size)
		{
			_elem[lo++] = _elem[hi++];
		}
		_size = lo;
		shrink();
		return hi - lo;//返回被删除元素的数目
	}

	T remove(Rank r) {
		T e = _elem[r];
		remove(r, r + 1);
		return e;
	}

	void shrink() {
		if (_capacity < DEFAULT_CAPACITY << 1)return;
		if (_size << 2 > _capacity)return;
		T *oldElem = _elem; _elem = new T[_capacity >>= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[]oldElem;
	}
};

//二叉树结点
template <typename T>struct BinNode
{
	T data;//数值
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//父节点，左右孩子节点
	int height;//高度（通用）
	//构造函数
	BinNode() :
		parent(NULL), lc(NULL), lr(NULL), height(0) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0) :
		data(e), parent(p), lc(lc), rc(rc), height(h) {}

	/****操作接口****/

	//统计当前节点后代总数
	int size() {
		int s = 1;//计入本身
		if (lc) s += lc->size();//递归计入左子树规模
		if (rc) s += rc->size();//递归计入右子树规模
		return s;
	}
	//将e作为当前节点的左孩子插入新的节点
	BinNodePosi(T) insertAsLC(T const&e) {
		return lc = new BinNode(e, this);
	}
	//将e作为当前节点的右孩子插入新的节点
	BinNodePosi(T) insertAsRC(T const&e) {
		return rc = new BinNode(e, this);
	}
};

//二叉树
template<typename T> class BinTree {
protected:
	int _size;//规模
	BinNodePosi(T) _root;//根节点

	//更新节点X的高度
	int updateHeight(BinNodePosi(T) x) {
		return x->height = 1 + fmax(stature(x->lc), stature(x->rc));
	}

	//更新高度
	void updateHeightAbove(BinNodePosi(T) x) {
		while (x)
		{
			updateHeight(x);
			x = x->parent;//从x出发，覆盖历代祖先
		}
	}

public:
	//将e作为根节点插入空的二叉树
	BinNodePosi(T) insertAsRoot(T const &e) {
		_size = 1;
		return _root = new BinNode<T>(e);
	}

	//将e作为x的右孩子插入
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e) {
		_size++;
		x->insertAsRC(e);
		updateHeightAbove(x);
		return x->rc;
	}

	//将e作为x的左孩子插入
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e) {
		_size++;
		x->insertAsLC(e);
		updateHeightAbove(x);
		return x->lc;
	}

	//先序遍历(递归版)
	void pre_traverse(BinNodePosi(T) x) {
		if (!x)return;
		cout << x->data << "\n";
		pre_traverse(x->lc);
		pre_traverse(x->rc);
	}

	//先序遍历(迭代栈版1)
	void pre_trav1(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		if (x)	stack.push(x);
		while (!stack.isEmpty())
		{
			x = stack.pop();
			cout << x->data << "\n";
			if (x->rc) stack.push(x->rc);//推入右子树
			if (x->lc) stack.push(x->lc);//推入左子树
		}
	}

	//先序遍历(迭代栈版2)
	void pre_trav2(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		while (true)
		{
			while (x)//沿着左侧分支访问
			{
				cout << x->data << "\n";
				stack.push(x->rc);
				x = x->lc;
			}
			if (stack.isEmpty())break;
			x = stack.pop();
		}
	}

	//中序遍历(递归版)
	void in_traverse(BinNodePosi(T) x) {
		if (!x)return;
		in_traverse(x->lc);
		cout << x->data << "\n";
		in_traverse(x->rc);
	}

	//中序遍历(迭代栈版)
	void in_trav(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		while (true)
		{
			while (x)// 沿左分支深入，但并不访问
			{
				stack.push(x);
				x = x->lc;
			}
			if (stack.isEmpty())break;
			x = stack.pop();
			cout << x->data << "\n";
			x = x->rc;
		}
	}

	//层次遍历
	void travLevel(BinNodePosi(T)x) {
		Queue<BinNodePosi(T)> que;
		que.enqueue(x);
		while (!que.empty())
		{
			x = que.dequeue();
			cout << x->data << "\n";
			if (x->lc)  que.enqueue(x->lc);
			if (x->rc)	que.enqueue(x->rc);
		}
	}

};

int main() {
	BinTree<int>tree;
	BinNodePosi(int)root = tree.insertAsRoot(42);
	tree.insertAsRC(root, 65);
	BinNodePosi(int) r1 = tree.insertAsLC(root, 62);
	tree.insertAsLC(r1, 2);
	tree.travLevel(root);
}