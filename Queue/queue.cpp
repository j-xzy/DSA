#include <cstdio>

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

int main() {

}