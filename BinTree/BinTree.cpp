#include <cstdio>
#include <cmath>
#include <iostream>
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height:-1)//�ڵ�߶�(������ڵ�߶�Ϊ-1Լ����ͳһ)
using namespace std;

typedef int Rank;
#define DEFAULT_CAPACITY 3

#define ListNodePosi(T) ListNode<T>*
template<typename T> struct ListNode
{
	//��Ա
	T data;//��ֵ
	ListNodePosi(T) pred;//ǰ��
	ListNodePosi(T) succ;//����
						 //���캯��
	ListNode() {}//���header��trailer�Ĺ���
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
	{
		pred = p;
		succ = s;
		data = e;
	}//Ĭ�Ϲ��캯��
	 //�����ӿ�
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

	//����ĩ�ڵ�
	ListNodePosi(T) insertAsLast(T const&e) {
		_size++; return trailer->insertAsPred(e);
	}

	//ɾ���ڵ�
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

	//����б�
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
	//���:β�����
	void enqueue(T const&e) {
		insertAsLast(e);
	}
	//����:ͷ������
	T dequeue() {
		return remove(first());
	}
	//����
	T& front() {
		return first()->data;
	}
	//��ģ
	int size() {
		return _size;
	}
	//�п�
	bool empty() {
		return _size == 0;
	}
};

//ջ
template <typename T> class Stack
{
protected:
	Rank _size; int _capacity; T*_elem;

public:
	//���캯��
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//��������
	~Stack() {
		delete[]_elem;
	}

	//��ջ
	void push(T const& e) {
		insert(size(), e);
	}

	//��ջ
	T pop() {
		return remove(size() - 1);
	}

	//ȡ��
	T& top() {
		return _elem[_size - 1];
	}

	//����ջ��С
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
		return hi - lo;//���ر�ɾ��Ԫ�ص���Ŀ
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

//���������
template <typename T>struct BinNode
{
	T data;//��ֵ
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//���ڵ㣬���Һ��ӽڵ�
	int height;//�߶ȣ�ͨ�ã�
	//���캯��
	BinNode() :
		parent(NULL), lc(NULL), lr(NULL), height(0) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0) :
		data(e), parent(p), lc(lc), rc(rc), height(h) {}

	/****�����ӿ�****/

	//ͳ�Ƶ�ǰ�ڵ�������
	int size() {
		int s = 1;//���뱾��
		if (lc) s += lc->size();//�ݹ������������ģ
		if (rc) s += rc->size();//�ݹ������������ģ
		return s;
	}
	//��e��Ϊ��ǰ�ڵ�����Ӳ����µĽڵ�
	BinNodePosi(T) insertAsLC(T const&e) {
		return lc = new BinNode(e, this);
	}
	//��e��Ϊ��ǰ�ڵ���Һ��Ӳ����µĽڵ�
	BinNodePosi(T) insertAsRC(T const&e) {
		return rc = new BinNode(e, this);
	}
};

//������
template<typename T> class BinTree {
protected:
	int _size;//��ģ
	BinNodePosi(T) _root;//���ڵ�

	//���½ڵ�X�ĸ߶�
	int updateHeight(BinNodePosi(T) x) {
		return x->height = 1 + fmax(stature(x->lc), stature(x->rc));
	}

	//���¸߶�
	void updateHeightAbove(BinNodePosi(T) x) {
		while (x)
		{
			updateHeight(x);
			x = x->parent;//��x������������������
		}
	}

public:
	//��e��Ϊ���ڵ����յĶ�����
	BinNodePosi(T) insertAsRoot(T const &e) {
		_size = 1;
		return _root = new BinNode<T>(e);
	}

	//��e��Ϊx���Һ��Ӳ���
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e) {
		_size++;
		x->insertAsRC(e);
		updateHeightAbove(x);
		return x->rc;
	}

	//��e��Ϊx�����Ӳ���
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e) {
		_size++;
		x->insertAsLC(e);
		updateHeightAbove(x);
		return x->lc;
	}

	//�������(�ݹ��)
	void pre_traverse(BinNodePosi(T) x) {
		if (!x)return;
		cout << x->data << "\n";
		pre_traverse(x->lc);
		pre_traverse(x->rc);
	}

	//�������(����ջ��1)
	void pre_trav1(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		if (x)	stack.push(x);
		while (!stack.isEmpty())
		{
			x = stack.pop();
			cout << x->data << "\n";
			if (x->rc) stack.push(x->rc);//����������
			if (x->lc) stack.push(x->lc);//����������
		}
	}

	//�������(����ջ��2)
	void pre_trav2(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		while (true)
		{
			while (x)//��������֧����
			{
				cout << x->data << "\n";
				stack.push(x->rc);
				x = x->lc;
			}
			if (stack.isEmpty())break;
			x = stack.pop();
		}
	}

	//�������(�ݹ��)
	void in_traverse(BinNodePosi(T) x) {
		if (!x)return;
		in_traverse(x->lc);
		cout << x->data << "\n";
		in_traverse(x->rc);
	}

	//�������(����ջ��)
	void in_trav(BinNodePosi(T) x) {
		Stack<BinNodePosi(T)> stack;
		while (true)
		{
			while (x)// �����֧���룬����������
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

	//��α���
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