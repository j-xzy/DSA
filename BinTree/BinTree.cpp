#include <cstdio>
#include <cmath>
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height:-1)//�ڵ�߶�(������ڵ�߶�Ϊ-1Լ����ͳһ)

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
		return x->height = 1 + fmax(stature(x->lc),stature(x->rc));
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
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const &e) {
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
};

int main() {
	BinTree<int>tree;
    BinNodePosi(int)root=tree.insertAsRoot(42);
	tree.insertAsRC(root, 65);
	BinNodePosi(int) r1=tree.insertAsLC(root, 62);
	tree.insertAsLC(r1, 2);
}