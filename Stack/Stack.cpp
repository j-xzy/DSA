#include <cstdio>
#include <ctype.h>
#include <string>
#include <iostream>

typedef int Rank;
#define DEFAULT_CAPACITY 3
using namespace std;
//ջ
template <typename T> class Stack
{
protected:
	Rank _size; int _capacity; T*_elem;

public:
	//���캯��
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}

	//��������
	~Stack() {
		delete[]_elem;
	}

	//��ջ
	void push(T const& e) {
		insert(size(), e);
	}

	//��ջ
	T pop() {
		return remove(size() - 1);
	}

	//ȡ��
	T& top() {
		return _elem[_size - 1];
	}

	//����ջ��С
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
		return hi - lo;//���ر�ɾ��Ԫ�ص���Ŀ
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

//����ת��
void convert(Stack<char>& S, __int64 n, int base) {
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0) {
		S.push(digit[n%base]);
		n /= base;
	}
}

//����ƥ��
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

//ջ��ϴ���n����
void stackIsWash(int *B, int n) {
	Stack<int>S;
	int m = 0;
	S.push(m++);
	printf("%s", "push\n");
	for (int i = 0; i < n; i++)
	{
		if (!S.isEmpty() && S.top() > B[i]) {
			printf("%s", "NO\n");
			break;
		}
		while (m < n&&S.top() != B[i])
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

/********************************��׺���ʽ��ֵ*******************************/

//���ȼ���
const char pri[9][9] = {
	//    +   -   *   /   ^   !   (   )  \0   (��ǰ�����)
/* + */{ '>','>','<','<','<','<','<','>','>' },
/* - */{ '>','>','<','<','<','<','<','>','>' },
/* * */{ '>','>','>','>','<','<','<','>','>' },
/* / */{ '>','>','>','>','<','<','<','>','>' },
/* ^ */{ '>','>','>','>','>','<','<','>','>' },
/* ! */{ '>','>','>','>','>','>',' ','>','>' },
/* ( */{ '<','<','<','<','<','<','<','=',' ' },
/* ) */{ ' ',' ',' ',' ',' ',' ',' ',' ',' ' },
/* \0*/{ '<','<','<','<','<','<','<',' ','=' }
//(ջ�������)
};

//��������
void readNumber(char *&p, Stack<float> &stk) {
	stk.push((float)(*p - '0'));
	while (isdigit(*(++p)))
	{
		stk.push(stk.pop() * 10 + (*p - '0'));
	}
	if ('.' != *p)
	{
		return;
	}
	float fraciton = 1;
	while (isdigit(*(++p)))
	{
		stk.push(stk.pop() + (*p - '0')*(fraciton /= 10));
	}
}

//��������ó����
int optr2rank(char op) {
	switch (op)
	{
	case '+':return 0;
	case '-':return 1;
	case '*':return 2;
	case '/':return 3;
	case '^':return 4;
	case '!':return 5;
	case '(':return 6;
	case ')':return 7;
	case '\0': return 8;
	default:exit(-1);
	}
}

//�Ƚ�����������ȼ�
char orderBetween(char op1, char op2) {
	return pri[optr2rank(op1)][optr2rank(op2)];
}

//����
float calcu(float b) {
	if (b < 1.01)
	{
		return 1;
	}
	return b*(calcu(b - 1));
}

float calcu(float a, char op, float b) {
	switch (op)
	{
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a*b;
	case '/':return a / b;
	case '^': {
		float result = 1;
		for (int i = 0; i < b; i++)
		{
			result = result*a;
		}
		return result;
	}
	default:exit(-1);
	}
}

//��׺���ʽ��ֵ
float evalute(char *S,string *RPN)
{
	Stack<float> opnd; Stack<char> optr;//������ջ�������ջ
	optr.push('\0');
	while (!optr.isEmpty()) {
		if (isdigit(*S)) {
			readNumber(S, opnd);
			*(RPN++) = to_string(opnd.top());
		}
		else {
			switch (orderBetween(optr.top(), *S))
			{
			case '<'://ջ����������ȼ�����ʱ
				optr.push(*S); S++;//�����Ƴ٣���ǰ�������ջ
				break;
			case '='://���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
				optr.pop(); S++;
				break;
			case '>': {//ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
				char op = optr.pop(); //ջ���������ջ
				*(RPN++) = op;
				if ('!' == op) {
					float pOpnd = opnd.pop();
					opnd.push(calcu(pOpnd));
				}
				else {
					float pOpnd2 = opnd.pop(); float pOpnd1 = opnd.pop();
					opnd.push(calcu(pOpnd1, op, pOpnd2));
				}
				break;
			}
			default:exit(-1);
			}
		}
	}
	return opnd.pop();
}


void main() {
	char a = 2;
	char S[100];
	string rpn[100];
	int length = 0;
	cout << "������ʽ\n";
	gets_s(S);
	length = strlen(S);
	S[length] = '\0';
	float result = evalute(S,rpn);
	cout << result;
}
