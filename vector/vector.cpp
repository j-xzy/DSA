//vector.cpp : main project file.
#include "stdafx.h"
#include <vector>

#include <iostream>
using std::cout;
using std::endl;
using namespace System;


typedef int Rank;
#define DEFAULT_CAPACITY 3

class Fib1 { //Fibonacci数列类
private:
	int f, g; //f = fib(k - 1), g = fib(k)。均为int型，很快就会数值溢出
public:
	Fib1(int n) //初始化为不小于n的最小Fibonacci项
	{
		f = 1; g = 0;
		while (g < n) {
			next();
		}
	} //fib(-1), fib(0)，O(log_phi(n))时间
	int get()
	{
		return g;
	} //获取当前Fibonacci项，O(1)时间
	int next()
	{
		g += f; f = g - f;
		return g;
	} //转至下一Fibonacci项，O(1)时间
	int prev()
	{
		f = g - f; g -= f;
		return g;
	} //转至上一Fibonacci项，O(1)时间

};

class Fib {
private:
	int _f, _g;
public:
	Fib(int n) {
		_f = 1; _g = 0;
		while (_g < n)
		{
			next();
		}
	}

	int get() {
		return _g;
	}

	void next() {
		_g = _g + _f;
		_f = _g - _f;
		//return _g;
	}

	void pre() {
		_f = _g - _f;
		_g = _g - _f;
		//return _g;
	}
};

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
		if (_size << 2>_capacity)return;
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
	~myVector()
	{
		delete[]_elem;
	}

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
			n += (_elem[i - 1]>_elem[i]);
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
			while (++lo<hi)
			{
				if (_elem[lo - 1]>_elem[lo]) {
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
			while (++lo<hi)
			{
				if (_elem[lo - 1]>_elem[lo]) {
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
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
			if (j < lb && (k >= lc || B[j] < C[k])) {
				A[i++] = B[j++];
			}
			if (k < lc && (j >= lb || C[k] <= B[j])) {
				A[i++] = C[k++];
			}
		}
	}

};

template<typename T>
struct Increase
{
	virtual void operator()(T &e) { e++; }
};


int main(array<System::String ^> ^args)
{
	myVector<int> a;
	int b[5];
	for (int i = 0; i < 5; i++) {
		printf("%d", b[i] = i * 2);
	}
	a.binSearch(b, 3, 0, 5);

}