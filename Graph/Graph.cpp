#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

//������
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

//������
typedef int Rank;
#define DEFAULT_CAPACITY 3
template <typename T> class myVector
{
protected:
	Rank _size; int _capacity; T*_elem;

	//������������A[lo, hi)
	void myVector<T>::copyFrom(T const*A, Rank lo, Rank hi) {
		_elem = new T[_capacity = 2 * (hi - lo)];
		size = 0;
		while (lo < hi)
		{
			_elem[_size++] = A[lo++];
		}
	}

	//�����ռ䲻��ʱ����
	void myVector<T>::expand() {
		if (_size < _capacity)return;
		_capacity = _capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : _capacity;
		T*oldElem = _elem; _elem = new T[_capacity <<= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[] oldElem;
	}

	//װ�����ӹ�Сѹ��������ռ�ռ�
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
	//���캯��
	myVector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)//����Ϊc����ģΪs������Ԫ�س�ʼΪv
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//��������
	//~myVector()
	//{
	//	delete[]_elem;
	//}

	/**ֻ�����ʽӿ�**/

	//���ش�С
	Rank size() const { return _size; }

	//�����±������������������������ʽ���ø�Ԫ��
	T& operator[] (Rank r) const {
		return _elem[r];
	};

	//��e��Ϊ��ΪrԪ�ز���
	Rank insert(Rank r, T const& e) {
		expand();
		for (int i = _size; i > r; i--) {
			_elem[i] = _elem[i - 1];
		}
		_elem[r] = e; _size++;
		return r;
	}

	//ɾ������[lo,hi)
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

	//ɾ����������Ϊr��Ԫ��
	T remove(Rank r) {
		T e = _elem[r];
		remove(r, r + 1);
		return e;
	}

	//�����ж��Ԫ��ʱ���������ֵ
	Rank find(T const &e, Rank lo, Rank hi) {
		while ((lo < hi--) && (e != _elem[hi]));
		return hi;
	}

	//ɾ���ظ���Ԫ��
	int deduplicate() {
		int oldsize = _size;
		Rank i = 1;
		while (i < _size)
		{
			(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
		}
		return oldsize - _size;
	}

	template <typename VST> //Ԫ�����͡�������
	void traverse(VST& visit) //���������������
	{
		for (int i = 0; i < _size; i++) visit(_elem[i]);
	} //��������

	  //������������������Ԫ�ضԵ�����
	int disordered()const {
		int n = 0;
		for (int i = 1; i < _size; i++) {
			n += (_elem[i - 1] > _elem[i]);
		}
		return n;
	}

	//ɾ���ظ�Ԫ��
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

	//������������
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
		Fib1 fib(hi - lo); //��O(log_phi(n = hi - lo)ʱ�䴴��Fib����
		while (lo < hi) { //ÿ����������Ҫ�����αȽ��жϣ���������֧
			while (hi - lo < fib.get())
				fib.prev(); //ͨ����ǰ˳����ң���̯O(1)����������������Σ�
			Rank mi = lo + fib.get() - 1; //ȷ������Fib(k) - 1�����
			if (e < A[mi]) hi = mi; //����ǰ���[lo, mi)��������
			else if (A[mi] < e) lo = mi + 1; //�������(mi, hi)��������
			else                return mi; //��mi������

		} //�ɹ����ҿ�����ǰ��ֹ
		return -1; //����ʧ��

	} //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�ʧ��ʱ���򵥵ط���-1��������ָʾʧ�ܵ�λ��

	static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
		Fib fib(hi - lo);
		while (lo < hi)
		{
			while ((hi - lo) < fib.get())
			{
				fib.pre();
			}
			Rank mi = lo + fib.get() - 1;
			if (e < A[mi]) hi = mi; //����ǰ���[lo, mi)��������
			else if (A[mi] < e) lo = mi + 1; //�������(mi, hi)��������
			else      return mi; //��mi������
		}
		return -1;
	}

	//�����������ҷ��ز�����e���������
	static Rank binSearch2(T *A, T const &e, Rank lo, Rank hi) {
		while (lo < hi)
		{
			int mi = (lo + hi) >> 1;
			e < A[mi] ? hi = mi : lo = mi + 1; //[lo,mi)��(mi,hi)
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

	//�鲢����
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

//����״̬
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;

//���ڱ�����������������
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;

//�������
template <typename Tv> struct Vertex
{
	//���ݡ����ȡ���ȡ�״̬
	Tv data; int inDegree, outDegree; VStatus status;
	//ʱ���ǩ
	int dTime, fTime;
	//�ڱ������еĸ��ڵ㡢���ȼ�
	int parent; int priority;
	//�����¶���
	Vertex(Tv const&d = (Tv)0) :
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}//�ݲ�����Ȩ�����
};

//�߶���
template<typename Te>struct Edge
{
	//���ݡ�Ȩ�ء�����
	Te data; int weight; EType type;
	//����
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}
};

//���ڽӾ�����ʽʵ�ֵ�ͼ
template<typename Tv, typename Te>
class GraphMatrix
{
private:
	int n, e;//���㡢�߸���
	//���㼯(����)
	myVector<Vertex<Tv>>V;
	//�߼�(�ٽӾ���)
	myVector<myVector<Edge<Te>*>>E;

	//���ж��㣬�ߵĸ�����Ϣ��λ
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

	//�������
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
					type(v, u) = TREE; parent(u) = v;//��������
				}
				else
				{
					type(v, u) = CROSS;//������
				}
			}
			status(v) = VISITED;
		}
	}

	//�������
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
	//����
	GraphMatrix() {
		n = e = 0;
	}
	//����
	~GraphMatrix() {
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++) {
				delete E[j][k];
			}
		}
	}

	/*****�����д*****/
#pragma region �����д
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

	/****�ڵ��ö��****/
#pragma region �ڵ��ö��

	//�ڵ��ö��(����ö�����ھ�j����ת����һ���ھ�)
	int nextNbr(int i, int j) {
		while ((-1 < j) && !exists(i, --j));
		return j;
	}
	//�׸��ھ�
	int firstNbr(int i) {
		return nextNbr(i, n);//�ڱ�
	}
#pragma endregion

	/*****�ߵĶ�д*****/
#pragma region �ߵĶ�д
	//��(i,j)�Ƿ����
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

	//�ߵĲ���
	void insert(Te const& edge, int w, int i, int j) {
		if (exists(i, j))return;
		E[i][j] = new Edge<Te>(edge, w);
		e++;//���±���
		V[i].outDegree++;
		V[j].inDegree++;
	}

	//�ߵ�ɾ��
	Te remove(int i, int j) {
		Te eBak = edge(i, j);
		delete E[i][j];
		E[i][j] = NULL;
		e--;
		V[i].outDegree--;
		V[j].inDegree--;
		return eBak;
	}

	//�������(���ر��)
	int insert(Tv const& vertex) {
		for (int j = 0; j < n; j++)
		{
			E[j].insert(n, NULL);
		}
		n++;
		E.insert(n-1, myVector<Edge<Te>*>(n, n, NULL));
		return V.insert(n-1, Vertex<Tv>(vertex));
	}

	//����ɾ��
	Tv remove(int i) {
		for (int j = 0; j < n; j++) {//ɾ�����г���
			if (exists(i, j)) {
				delete E[i][j];
				V[j].inDegree--;
			}
		}
		E.remove(i); n--;//ɾ����i��
		Tv vBak = vertex(i); V.remove(i);
		for (int j = 0; j < n; j++) {//ɾ��������߼���i��
			if (Edge<Te>*e = E[j].remove(i)) {
				delete e;
				V[j].outDegree--;
			}
		}
		return vBak;
	}

	//�����������
	void bfs(int s) {//sΪ��ʼ����
		reset();
		int clock = 0; int v = s;
		do {
			if (UNDISCOVERED == status(v))
			{
				BFS(v, clock);
			}
		} while (s != (v = (++v%n)));
	}

	//�����������
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