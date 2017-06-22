//vector.cpp : main project file.
#include "stdafx.h"
#include <vector>

#include <iostream>
using std::cout;
using std::endl;
using namespace System;


typedef int Rank;
#define DEFAULT_CAPACITY 3

class Fib1 { //Fibonacci������
private:
	int f, g; //f = fib(k - 1), g = fib(k)����Ϊint�ͣ��ܿ�ͻ���ֵ���
public:
	Fib1(int n) //��ʼ��Ϊ��С��n����СFibonacci��
	{
		f = 1; g = 0;
		while (g < n) {
			next();
		}
	} //fib(-1), fib(0)��O(log_phi(n))ʱ��
	int get()
	{
		return g;
	} //��ȡ��ǰFibonacci�O(1)ʱ��
	int next()
	{
		g += f; f = g - f;
		return g;
	} //ת����һFibonacci�O(1)ʱ��
	int prev()
	{
		f = g - f; g -= f;
		return g;
	} //ת����һFibonacci�O(1)ʱ��

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
		if (_size << 2>_capacity)return;
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
	~myVector()
	{
		delete[]_elem;
	}

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
			n += (_elem[i - 1]>_elem[i]);
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