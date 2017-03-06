// List.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef int Rank;
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
	ListNodePosi(T) insertAsSucc(T const& e) {};
};

template<typename T> class  List
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//��ģ��ͷ�ڱ���β�ڱ�
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

	//���ƽڵ�
	void copyNodes(ListNodePosi(T) p, int n) {
		init();
		while (n--)
		{
			insertAsLast(p->data); p = p->succ;
		}
	}

public:
	//���캯��
	List() { init(); };//Ĭ��

   //��������
	~List() {
		clear(); delete header; delete trailer;
	};
	//��ģ
	Rank size()const { return _size; }
	//�п�
	bool empty() const { return _size <= 0; }
	//���أ�֧��ѭ�ȷ���
	T& operator[] (Rank r)const {
		ListNodePosi(T) p = first();//���׽ڵ����
		while (0 < r--) p = p->succ;
		return p->data;
	}
	ListNodePosi(T) first()const { return header->succ; }
	ListNodePosi(T) last()const { return trailer->pred; }

	//����p��ǰ��
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e) {
		_size++;
		return p->insertAsPred(e);
	}

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

	//���ҽڵ�
	ListNodePosi(T) find(T const&e, int n, ListNodePosi(T) p)const {
		while (0 < n--) {
			if (e == (p = p->pred)->data)
				return p;
		}
		return NULL;
	}

	//Ψһ��
	int deduplicate() {
		if (_size < 2)
			return 0;
		int oldSize = _size;//��¼ԭʼ��ģ
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

