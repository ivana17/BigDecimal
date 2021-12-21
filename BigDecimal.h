#ifndef _BIGDECIMAL_H_
#define _BIGDECIMAL_H_
#include <iostream>
using namespace std;
#define MAX(a, b) ((a > b) ? a : b) 
#define MIN(a, b) ((a < b) ? a : b)

class BigDecimal{
private:
	char sign;
	int* whole;
	int numWhole;
	int* fraction;
	int numFraction;

	friend int arrayLength(int *);
	friend int* zeroClear(int*, int);
	friend int* zeroClearBeginning(int*, int);
	friend int* funcAdd(int*, int, int*, int, int&, bool);
	friend int* funcSub(int*, int, int*, int, int&, bool);
	friend char* intToChr(int*, int*, char);
	BigDecimal& operator= (const BigDecimal&);
	
public:
	BigDecimal(char* num);
	BigDecimal(int* whole, int* fraction);
	BigDecimal shl(int n);
	BigDecimal shr(int n);
	BigDecimal rmd(int* n);
	bool equals(BigDecimal* num);
	BigDecimal abs();
	bool greater(BigDecimal* num);
	bool less(BigDecimal* num);
	friend ostream& operator<<(ostream&, const BigDecimal&);
	BigDecimal add(BigDecimal*);
	BigDecimal sub(BigDecimal*);
	~BigDecimal();
	BigDecimal(const BigDecimal&);
	friend void test();
	friend class TreeNode;
	friend class RSearchTree;
	//dopuna
	BigDecimal operator<< (int n);
	BigDecimal operator>> (int n);
	BigDecimal operator>>= (int& n);
	bool operator== (const BigDecimal& num);
	bool operator!= (const BigDecimal& num);
	BigDecimal operator- ();
	BigDecimal operator+ ();
	bool operator> (const BigDecimal& num);
	bool operator< (const BigDecimal& num);
	bool operator>= (const BigDecimal& num);
	bool operator<= (const BigDecimal& num);
	BigDecimal operator+ (const BigDecimal&);
	BigDecimal operator- (const BigDecimal&);
	friend ostream& operator<<(ostream&, const BigDecimal&);
};

#endif