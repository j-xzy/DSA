#include <cstdio>
#include <cmath>
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height:-1)//节点高度(与空数节点高度为-1约定相统一)

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
		return x->height = 1 + fmax(stature(x->lc),stature(x->rc));
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
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const &e) {
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
};

int main() {
	BinTree<int>tree;
    BinNodePosi(int)root=tree.insertAsRoot(42);
	tree.insertAsRC(root, 65);
	BinNodePosi(int) r1=tree.insertAsLC(root, 62);
	tree.insertAsLC(r1, 2);
}