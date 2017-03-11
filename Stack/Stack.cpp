#include <cstdio>

typedef int Rank;
#define DEFAULT_CAPACITY 3


//栈
template <typename T> class Stack
{
protected:
	Rank _size; int _capacity; T*_elem;
	
public:
	//构造函数
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//析构函数
	~Stack() {
		delete[]_elem;
	}

	//入栈
	void push(T const& e) {
		insert(size(), e);
	}

	//出栈
	T pop() {
		return remove(size() - 1);
	}

	//取顶
	T& top() {
		return _elem[_size - 1];
	}

	//返回栈大小
	Rank size() const {
		return _size;
	}

	bool isEmpty() {
		return _size == 0;
	}

private:

	Rank insert(Rank r, T const& e) {
		expand();
		for (int i = _size; i > r; i--) {
			_elem[i] = _elem[i - 1];
		}
		_elem[r] = e; _size++;
		return r;
	}

	void expand() {
		if (_size < _capacity)return;
		_capacity = _capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : _capacity;
		T*oldElem = _elem; _elem = new T[_capacity <<= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[] oldElem;
	}

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

	T remove(Rank r) {
		T e = _elem[r];
		remove(r, r + 1);
		return e;
	}

	void shrink() {
		if (_capacity < DEFAULT_CAPACITY << 1)return;
		if (_size << 2 > _capacity)return;
		T *oldElem = _elem; _elem = new T[_capacity >>= 1];
		for (int i = 0; i < _size; i++) {
			_elem[i] = oldElem[i];
		}
		delete[]oldElem;
	}
};

//进制转换
void convert(Stack<char>& S, __int64 n, int base) {
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0) {
		S.push(digit[n%base]);
		n /= base;
	}
}

//括号匹配
bool paren(const char exp[], int lo, int hi) {
	Stack<char> stack;
	for (int i = lo; i < hi; i++)
	{
		if ('(' == exp[i] || '{' == exp[i]) {
			stack.push(exp[i]);
		}
		else if (!stack.isEmpty()) {
			if ((stack.top() == '{'&&exp[i] == '}') || (stack.top() == '('&&exp[i] == ')')) {
				stack.pop();
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return stack.isEmpty();
}

//栈混洗甄别n个数
void stackIsWash(int *B, int n) {
	Stack<int>S;
	int m = 0;
	S.push(m++);
	printf("%s", "push\n");
	for (int i = 0; i < n; i++)
	{
		if (!S.isEmpty()&&S.top() > B[i]) {
			printf("%s", "NO\n");
			break;
		}
		while (m<n&&S.top()!=B[i])
		{
			printf("%s", "push\n");
			S.push(m++);
		}
		if (S.top() == B[i])
		{
			printf("%s", "pop\n");
			S.pop();
		}
	}
}

void main() {
	int B[] = {2,0,1,3,4};
	stackIsWash(B, 5);
}
