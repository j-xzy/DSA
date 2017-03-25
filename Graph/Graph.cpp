#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

//队列类
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

//向量类
typedef int Rank;
#define DEFAULT_CAPACITY 3
template <typename T> class myVector
{
protected:
	Rank _size; int _capacity; T*_elem;

	//复制数组区间A[lo, hi)
	void myVector<T>::copyFrom(T const*A, Rank lo, Rank hi) {
		_elem = new T[_capacity = 2 * (hi - lo)];
		size = 0;
		while (lo < hi)
		{
			_elem[_size++] = A[lo++];
		}
	}

	//向量空间不足时扩容
	void myVector<T>::expand() {
		if (_size < _capacity)return;
		_capacity = _capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : _capacity;
		T*oldElem = _elem; _elem = new T[_capacity <<= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[] oldElem;
	}

	//装填因子过小压缩向量所占空间
	void shrink() {
		if (_capacity < DEFAULT_CAPACITY << 1)return;
		if (_size << 2 > _capacity)return;
		T *oldElem = _elem; _elem = new T[_capacity >>= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[]oldElem;
	}

public:
	//构造函数
	myVector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)//容量为c、规模为s、所有元素初始为v
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//析构函数
	//~myVector()
	//{
	//	delete[]_elem;
	//}

	/**只读访问接口**/

	//返回大小
	Rank size() const { return _size; }

	//重载下标操作符，可以类似于数组形式引用各元素
	T& operator[] (Rank r) const {
		return _elem[r];
	};

	//将e作为秩为r元素插入
	Rank insert(Rank r, T const& e) {
		expand();
		for (int i = _size; i > r; i--) {
			_elem[i] = _elem[i - 1];
		}
		_elem[r] = e; _size++;
		return r;
	}

	//删除区间[lo,hi)
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

	//删除向量中秩为r的元素
	T remove(Rank r) {
		T e = _elem[r];
		remove(r, r + 1);
		return e;
	}

	//在命中多个元素时返回秩最大值
	Rank find(T const &e, Rank lo, Rank hi) {
		while ((lo < hi--) && (e != _elem[hi]));
		return hi;
	}

	//删除重复的元素
	int deduplicate() {
		int oldsize = _size;
		Rank i = 1;
		while (i < _size)
		{
			(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
		}
		return oldsize - _size;
	}

	template <typename VST> //元素类型、操作器
	void traverse(VST& visit) //借助函数对象机制
	{
		for (int i = 0; i < _size; i++) visit(_elem[i]);
	} //遍历向量

	  //返回向量中逆序相邻元素对的总数
	int disordered()const {
		int n = 0;
		for (int i = 1; i < _size; i++) {
			n += (_elem[i - 1] > _elem[i]);
		}
		return n;
	}

	//删除重复元素
	int uniquify() {
		Rank i = 0; j = 0;
		while (++j < _size)
		{
			if (_elem[i] != _elem[j]) {
				_elem[++i] = _elem[j];
			}
		}
		_size = ++i; shrink();
		return j - i;
	}

	//有序向量查找
	static Rank binSearch(T *A, T const &e, Rank lo, Rank hi) {
		while (lo < hi)
		{
			Rank mi = (lo + hi) >> 1;
			if (e < A[mi]) {
				hi = mi;
			}
			else if (A[mi] < e) {
				lo = mi + 1;
			}
			else
			{
				return mi;
			}
		}
		return -1;
	}

	static Rank recurbinSearch(T *A, T const &e, Rank lo, Rank hi) {
		if (A[(lo + hi) >> 1] == e) {
			return (lo + hi) >> 1;
		}
		else if (e < A[(lo + hi) >> 1])
		{
			return recurbinSearch(A, e, lo, (lo + hi) >> 1);
		}
		else if (A[(lo + hi) >> 1] < e) {

			return recurbinSearch(A, e, (((lo + hi) >> 1) + 1), hi);
		}
		else
		{
			return -1;
		}
	}

	static Rank fibSearch1(T* A, T const& e, Rank lo, Rank hi) {
		Fib1 fib(hi - lo); //用O(log_phi(n = hi - lo)时间创建Fib数列
		while (lo < hi) { //每步迭代可能要做两次比较判断，有三个分支
			while (hi - lo < fib.get())
				fib.prev(); //通过向前顺序查找（分摊O(1)）——至多迭代几次？
			Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
			if (e < A[mi]) hi = mi; //深入前半段[lo, mi)继续查找
			else if (A[mi] < e) lo = mi + 1; //深入后半段(mi, hi)继续查找
			else                return mi; //在mi处命中

		} //成功查找可以提前终止
		return -1; //查找失败

	} //有多个命中元素时，不能保证返回秩最大者；失败时，简单地返回-1，而不能指示失败的位置

	static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
		Fib fib(hi - lo);
		while (lo < hi)
		{
			while ((hi - lo) < fib.get())
			{
				fib.pre();
			}
			Rank mi = lo + fib.get() - 1;
			if (e < A[mi]) hi = mi; //深入前半段[lo, mi)继续查找
			else if (A[mi] < e) lo = mi + 1; //深入后半段(mi, hi)继续查找
			else      return mi; //在mi处命中
		}
		return -1;
	}

	//有序向量查找返回不大于e的秩最大者
	static Rank binSearch2(T *A, T const &e, Rank lo, Rank hi) {
		while (lo < hi)
		{
			int mi = (lo + hi) >> 1;
			e < A[mi] ? hi = mi : lo = mi + 1; //[lo,mi)、(mi,hi)
		}
		return --lo;
	}

	void bubbleSort1(Rank lo, Rank hi) {
		bool sorted = false;
		int loo = lo;
		while (!sorted)
		{
			sorted = true;
			while (++lo < hi)
			{
				if (_elem[lo - 1] > _elem[lo]) {
					T temp = _elem[lo - 1];
					_elem[lo - 1] = _elem[lo];
					_elem[lo] = temp;
					sorted = false;
				}
			}
			hi--;
			lo = loo;
		}
	}

	void bubbleSort2(Rank lo, Rank hi) {
		int last = lo + 1;
		int initLo = lo;
		while (last > lo)
		{
			last = lo;
			while (++lo < hi)
			{
				if (_elem[lo - 1] > _elem[lo]) {
					T temp = _elem[lo - 1];
					_elem[lo - 1] = _elem[lo];
					_elem[lo] = temp;
					last = lo;
				}
			}
			lo = initLo;
			hi = last;
		}
	}

	//归并排序
	void mergeSort(Rank lo, Rank hi) {
		if (hi - lo < 2)return;
		int mi = (lo + hi) >> 1;
		mergeSort(lo, mi);
		mergeSort(mi, hi);
		merge(lo, mi, hi);
	}

private:
	void merge(Rank lo, Rank mi, Rank hi) {
		T *A = _elem + lo;
		int lb = mi - lo;
		T *B = new T[lb];
		for (Rank i = 0; i < lb; B[i] = A[i++]);
		int lc = hi - mi; T *C = _elem + mi;
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lb);) {
			if (j < lb && (k >= lc || B[j] < C[k])) {
				A[i++] = B[j++];
			}
			if (k < lc && (j >= lb || C[k] <= B[j])) {
				A[i++] = C[k++];
			}
		}
	}
};

//顶点状态
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;

//边在遍历树中所属的类型
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;

//顶点对象
template <typename Tv> struct Vertex
{
	//数据、出度、入度、状态
	Tv data; int inDegree, outDegree; VStatus status;
	//时间标签
	int dTime, fTime;
	//在遍历树中的父节点、优先级
	int parent; int priority;
	//构造新顶点
	Vertex(Tv const&d = (Tv)0) :
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}//暂不考虑权重溢出
};

//边对象
template<typename Te>struct Edge
{
	//数据、权重、类型
	Te data; int weight; EType type;
	//构造
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}
};

//以邻接矩阵形式实现的图
template<typename Tv, typename Te>
class GraphMatrix
{
private:
	int n, e;//顶点、边个数
	//顶点集(向量)
	myVector<Vertex<Tv>>V;
	//边集(临接矩阵)
	myVector<myVector<Edge<Te>*>>E;

	//所有定点，边的辅助信息复位
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; j++) {
				if (exists(i, j)) {
					type(i, j) = UNDETERMINED;
				}
			}
		}
	}

	//广度优先
	void BFS(int v, int& clock) {
		Queue<int> Q; status(v) = DISCOVERED;
		Q.enqueue(v);
		while (!Q.empty())
		{
			int v = Q.dequeue();
			dTime(v) = ++clock;
			for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
			{
				if (UNDISCOVERED == status(u))
				{
					status(u) = DISCOVERED;
					Q.enqueue(u);
					type(v, u) = TREE; parent(u) = v;//引入树边
				}
				else
				{
					type(v, u) = CROSS;//归入跨边
				}
			}
			status(v) = VISITED;
		}
	}

	//深度优先
	void DFS(int v,int& clock) {
		dTime(v) = ++clock; status(v) = DISCOVERED;
		cout << vertex(v);
		for (int u = firstNbr(v); -1 < u;u=nextNbr(v,u)) {
			switch (status(u))
			{
			case UNDISCOVERED:
				type(v, u) = TREE; parent(u) = v; DFS(u, clock);
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD; break;
				break;
			default:
				type(v, u) = dTime(v) < dTime(u) ? FORWARD: CROSS;
				break;
			}
		}
		status(v) = VISITED; fTime(v) = ++clock;
	}


public:
	//构造
	GraphMatrix() {
		n = e = 0;
	}
	//析构
	~GraphMatrix() {
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++) {
				delete E[j][k];
			}
		}
	}

	/*****顶点读写*****/
#pragma region 顶点读写
	Tv &vertex(int i) {
		return V[i].data;
	}
	int inDegree(int i) {
		return V[i].inDegree;
	}
	int outDegree(int i) {
		return V[i].outDegree;
	}
	VStatus &status(int i) {
		return V[i].status;
	}
	int& dTime(int i) {
		return V[i].dTime;
	}
	int& fTime(int i) {
		return V[i].fTime;
	}
	int& parent(int i) {
		return V[i].parent;
	}
	int& priority(int i) {
		return V[i].priority;
	}
#pragma endregion

	/****邻点的枚举****/
#pragma region 邻点的枚举

	//邻点的枚举(若已枚举至邻居j，则转向下一个邻居)
	int nextNbr(int i, int j) {
		while ((-1 < j) && !exists(i, --j));
		return j;
	}
	//首个邻居
	int firstNbr(int i) {
		return nextNbr(i, n);//哨兵
	}
#pragma endregion

	/*****边的读写*****/
#pragma region 边的读写
	//边(i,j)是否存在
	bool exists(int i, int j) {
		return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
	}
	Te& edge(int i, int j) {
		return E[i][j]->data;
	}
	EType& type(int i, int j) {
		return E[i][j]->type;
	}
	int& weight(int i, int j) {
		return E[i][j].weight;
	}
#pragma endregion

	//边的插入
	void insert(Te const& edge, int w, int i, int j) {
		if (exists(i, j))return;
		E[i][j] = new Edge<Te>(edge, w);
		e++;//更新边数
		V[i].outDegree++;
		V[j].inDegree++;
	}

	//边的删除
	Te remove(int i, int j) {
		Te eBak = edge(i, j);
		delete E[i][j];
		E[i][j] = NULL;
		e--;
		V[i].outDegree--;
		V[j].inDegree--;
		return eBak;
	}

	//顶点插入(返回编号)
	int insert(Tv const& vertex) {
		for (int j = 0; j < n; j++)
		{
			E[j].insert(n, NULL);
		}
		n++;
		E.insert(n-1, myVector<Edge<Te>*>(n, n, NULL));
		return V.insert(n-1, Vertex<Tv>(vertex));
	}

	//顶点删除
	Tv remove(int i) {
		for (int j = 0; j < n; j++) {//删除所有出边
			if (exists(i, j)) {
				delete E[i][j];
				V[j].inDegree--;
			}
		}
		E.remove(i); n--;//删除第i行
		Tv vBak = vertex(i); V.remove(i);
		for (int j = 0; j < n; j++) {//删除所有入边及第i列
			if (Edge<Te>*e = E[j].remove(i)) {
				delete e;
				V[j].outDegree--;
			}
		}
		return vBak;
	}

	//广度优先搜索
	void bfs(int s) {//s为起始顶点
		reset();
		int clock = 0; int v = s;
		do {
			if (UNDISCOVERED == status(v))
			{
				BFS(v, clock);
			}
		} while (s != (v = (++v%n)));
	}

	//深度优先搜索
	void dfs(int s) {
		reset();
		int clock = 0; int v = s;
		do {
			if (UNDISCOVERED==status(v))
			{
				DFS(v, clock);
			}
		} while (s != (v = (++v%n)));
	}
};


int main() {
	GraphMatrix<char, int>graph;
}