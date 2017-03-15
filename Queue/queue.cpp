#include <cstdio>

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

int main() {

}