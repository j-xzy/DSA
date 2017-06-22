// LCS.cpp : main project file.

#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;
using namespace System;


string cls1(string A, string B){
	if (A.length() == 0 || B.length() == 0){
		return "";
	}
	else if (A.back() == B.back()){
		return cls1(A.substr(0, A.length() - 1), B.substr(0, B.length() - 1)) + A.substr(A.length() - 1, 1);
	}
	else{
		if (cls1(A.substr(0, A.length() - 1), B.substr(0, B.length())).length() > cls1(A.substr(0, A.length()), B.substr(0, B.length() - 1)).length()){
			return cls1(A.substr(0, A.length() - 1), B.substr(0, B.length()));
		}
		else
		{
			return cls1(A.substr(0, A.length()), B.substr(0, B.length() - 1));
		}
	}
}

 void reserve(int *A,int l,int h){
	 if (h > l){
		 int tmp = A[h];
		 A[h] = A[l];
		 A[l] = tmp;
		 reserve(A, l + 1, h - 1);
	 }
}

int main(array<System::String ^> ^args)
{

}

