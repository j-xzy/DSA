// List.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef int Rank;
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
		ListNodePosi(T) x = new ListNode(e, this,succ);
		succ->pred = x;
		succ = x;
		return x;
	};
};

template<typename T> class  List
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//规模，头哨兵，尾哨兵
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

	//复制节点
	void copyNodes(ListNodePosi(T) p, int n) {
		init();
		while (n--)
		{
			insertAsLast(p->data); p = p->succ;
		}
	}

public:
	//构造函数
	List() { init(); };//默认

   //析构函数
	~List() {
		clear(); delete header; delete trailer;
	};
	//规模
	Rank size()const { return _size; }
	//判空
	bool empty() const { return _size <= 0; }
	//重载，支持循秩访问
	T& operator[] (Rank r)const {
		ListNodePosi(T) p = first();//从首节点出发
		while (0 < r--) p = p->succ;
		return p->data;
	}
	ListNodePosi(T) first()const { return header->succ; }
	ListNodePosi(T) last()const { return trailer->pred; }

	//插入p的前驱
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e) {
		_size++;
		return p->insertAsPred(e);
	}
	//插入p的后继
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e) {
		_size++;
		return p->insertAsSucc(e);

	}

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

	//查找节点
	ListNodePosi(T) find(T const&e, int n, ListNodePosi(T) p)const {
		while (0 < n--) {
			if (e == (p = p->pred)->data)
				return p;
		}
		return NULL;
	}

	//唯一化
	int deduplicate() {
		if (_size < 2)
			return 0;
		int oldSize = _size;//记录原始规模
		ListNodePosi(T) p = header; Rank r = 0;
		while (trailer != (p = p->succ))
		{
			ListNodePosi(T) q = find(p->data, r, p);
			q ? remove(q) : r++;
		}
		return oldSize - _size;
	}

	//有序列表唯一化
	int uniquify() {
		if (_size < 2)return 0;
		int oldSize = _size;
		ListNodePosi(T) p = first(); ListNodePosi(T) q;
		while (trailer != (q = p->succ))
		{
			if (p->data != q->data) p = p->succ;
			else
			{
				remove(q);
			}
		}
		return oldSize - _size;
	}

	//有序列表查找（p的n个真前驱中）,找到不大于e的最后者
	ListNodePosi(T) search(T const&e, int n, ListNodePosi(T) p) const
	{
		while (0 <= n--)
		{
			if (((p = p->pred)->data) <= e)
			{
				break;
			}
		}
		return p;
	}

	//查找最大值(起始位置P的连续n个元素)
	ListNodePosi(T) selectMax(ListNodePosi(T)p, int n) {
		ListNodePosi(T)maxPos = p;
		while (1 < n--)
		{
			if ((maxPos->data) < ((p = p->succ)->data))
			{
				maxPos = p;
			}
		}
		return maxPos;
	}

	//选择排序(起始位置P的连续n个元素)
	void selectionSort(ListNodePosi(T) p,int n) {
		ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
		for (int i = 0; i < n; i++) {
			tail = tail->succ;
		}
		while (1<n)
		{
			//insertB(tail, remove(selectMax(head->succ, n))); insertB,remove需要new和delete，性能不好
			//直接交换数据
	        ListNodePosi(T) max = selectMax(head->succ, n);
			T dataMax = max->data;
			max->data = (tail->pred)->data;
			(tail->pred)->data = dataMax;
			tail = tail->pred; n--;
		}
	}

	//对列表中起始于位置p的连续n个元素做插入排序
	void insertionSort(ListNodePosi(T) p,int n) {
		for (int r = 0;r <n ; r++)
		{
			insertA(search(p->data, r, p), p->data);
			p = p->succ; remove(p->pred);
		}
	}
};

int main()
{
	List<int> list;
	list.insertB(list.first(), 4);
	list.insertB(list.first(), 4);
	list.insertB(list.first(), 0);
	list.insertB(list.first(), 0);
	list.insertB(list.first(), 3);
	list.insertB(list.first(), 3);

	list.insertionSort(list.first(), list.size());
	for (int i = 0; i < list.size(); i++)
	{
		printf("%d\n", list[i]);
	}

 }

