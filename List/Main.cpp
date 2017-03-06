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
	ListNodePosi(T) insertAsSucc(T const& e) {};
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
};

int main()
{
	List<int> list;
	list.insertB(list.first(), 0);
	list.insertB(list.first(), 0);
	list.insertB(list.first(), 3);
	list.insertB(list.first(), 3);
	list.insertB(list.first(), 4);
	list.insertB(list.first(), 4);
	int n = list.deduplicate();
}

