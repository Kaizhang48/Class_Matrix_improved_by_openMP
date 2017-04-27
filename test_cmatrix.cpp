// test_cmatrix.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include<vector>
#include "omp.h" 
using std::vector;
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::endl;
using std::out_of_range;

class matrix {
	friend matrix inv(matrix a);
	friend matrix operator* (const matrix& A, const matrix& B);
	friend matrix operator*(const matrix& A, double &B);
	friend matrix operator+ (const matrix& A, const matrix &B);
	friend matrix operator- (const matrix& A, const matrix& B);
	friend istream& operator >> (std::istream &is, matrix &m);
	friend ostream& operator <<(std::ostream &os, const matrix &m);
public:
	double*& operator[](int t) {
		double* &r = data[t];
		return r;
	}
	matrix() = default;
	matrix(const int&a, const int& b,const double& c=0) :row(a), col(b) {
		data = new double*[row];
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < row; ++i)
				data[i] = new double[col];
		}
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < row; ++i)
			{
				for (int j = 0; j < col; ++j)
				{
					data[i][j] = c;
				}
			}
		}
	
	}
	matrix(const matrix &copyfrom)
	{
		//cout << "this is copy constructor" <<endl;
		row = copyfrom.row;
		col = copyfrom.col;
		data = new double*[row];
#pragma omp parallel
		{
#pragma omp for 
			for (int i = 0; i < row; ++i) {
				//printf("I am Thread %d\n", omp_get_thread_num());
				data[i] = new double[col];
			}
		}
#pragma omp parallel
		{
#pragma omp for 
			for (int i = 0; i < row; ++i)
			{
				//printf("I am Thread %d\n", omp_get_thread_num());
				for (int j = 0; j < col; ++j)
				{
					data[i][j] = copyfrom.data[i][j];
				}
			}
		}
	}
	matrix(matrix &&movefrom) {
		if (movefrom.data != data) {
			data = movefrom.data;
			row = movefrom.row;
			col = movefrom.col;
			movefrom.data = nullptr;
			movefrom.row = 0;
			movefrom.col = 0;
		}
	}
	~matrix() {
		mfree();
	}
	void operator=(const matrix& assignfrom) {
		if (assignfrom.data != data) {
			if (data != nullptr) {
				mfree();
			}
			row = assignfrom.row;
			col = assignfrom.col;
			data = new double*[row];
#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < row; ++i)
					data[i] = new double[col];
			}
#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < row; ++i)
				{
					for (int j = 0; j < col; ++j)
					{
						data[i][j] = assignfrom.data[i][j];
					}
				}
			}
		}
	}
	void operator=(matrix&& moveassignfrom) {
		if (moveassignfrom.data != data) {
			if (data != nullptr) {
				mfree();
			}
			data = moveassignfrom.data;
			row = moveassignfrom.row;
			col = moveassignfrom.col;
			moveassignfrom.data = nullptr;
			moveassignfrom.row = 0;
			moveassignfrom.col = 0;
		}
	}
	/*matrix inv()
	{
		int *is = new int[row];
		int *js = new int[row];
		int i, j, k;
		double d, p;
		for (k = 0; k < row; k++)
		{
			d = 0.0;
			for (i = k; i <= row - 1; i++)
				for (j = k; j <= row - 1; j++)
				{
					p = fabs(data[i][j]);
					if (p>d) { d = p; is[k] = i; js[k] = j; }
				}
			if (0.0 == d)
			{
				free(is); free(js); 
				throw out_of_range("can not be inversed");
			}
			if (is[k] != k)
				for (j = 0; j <= row - 1; j++)
				{
					p = data[k][j];
					data[k][j] = data[is[k]][j];
					data[is[k]][j] = p;
				}
			if (js[k] != k)
				for (i = 0; i <= row - 1; i++)
				{
					p = data[i][k];
					data[i][k] = data[i][js[k]];
					data[i][js[k]] = p;
				}
			data[k][k] = 1.0 / data[k][k];
			for (j = 0; j <= row - 1; j++)
				if (j != k)
				{
					data[k][j] *= data[k][k];
				}
			for (i = 0; i <= row - 1; i++)
				if (i != k)
					for (j = 0; j <= row - 1; j++)
						if (j != k)
						{
							data[i][j] -= data[i][k] * data[k][j];
						}
			for (i = 0; i <= row - 1; i++)
				if (i != k)
				{
					data[i][k] = -data[i][k] * data[k][k];
				}
		}
		for (k = row - 1; k >= 0; k--)
		{
			if (js[k] != k)
				for (j = 0; j <= row - 1; j++)
				{
					p = data[k][j];
					data[k][j] = data[js[k]][j];
					data[js[k]][j] = p;
				}
			if (is[k] != k)
				for (i = 0; i <= row - 1; i++)
				{
					p = data[i][k];
					data[i][k] = data[i][is[k]];
					data[i][is[k]] = p;
				}
		}
		free(is); free(js);
		return *this;
	}*/
	vector<int> size() const {
		return vector<int>{row, col};
	}
private:
	int row;
	int col;
	double** data;
	void mfree() {
		if (data != nullptr) {
			for (int i = 0; i < row; ++i)
				delete[] data[i];
			delete[] data;
		}
		row = 0;
		col = 0;
	}
};
matrix operator*(const matrix& A, const matrix& B) {
	auto szA = A.size();
	auto szB = B.size();
	if (szA[0] != szB[1] || szA[1] != szB[0]) {
		throw out_of_range("size of two matrixs does not match!");
	}
	matrix result(szA[0], szB[1]);
#pragma omp parallel  
	{
#pragma omp for 

		for (int i = 0; i < szA[0]; ++i)
		{
			//printf("I am Thread %d\n", omp_get_thread_num());
			for (int j = 0; j < szB[1]; ++j)
			{
				for (int k = 0; k < szA[0]; k++)
				{
#pragma omp critical  
					result.data[i][j] += A.data[i][k]
						* B.data[k][j];
				}
			}
		}
	}
	return result;
}
matrix operator*(const matrix& A, const double &B) {
	auto sz = A.size();
	matrix result = A;
#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < sz[0]; ++i) {
			for (int j = 0; j < sz[1]; ++j) {
				result[i][j] *= B;
			}
		}
	}
	return result;
}
matrix operator+ (const matrix& A, matrix &B) {
	auto szA = A.size();
	auto szB = B.size();
	if (szA[0] != szB[0] || szA[1] != szB[1]) {
		throw out_of_range("size of two matrixs does not match!");
	}
	matrix c = A;
#pragma omp parallel 
	{
#pragma omp for  
			for (int i = 0; i < szA[0]; ++i) {
				//printf("I am Thread %d\n", omp_get_thread_num());
				for (int j = 0; j < szA[1]; ++j) {
					c[i][j] += B[i][j];
				}
			}
	}
	return c;
}
matrix operator- (const matrix& A, matrix& B) {
	auto szA = A.size();
	auto szB = B.size();
	if (szA[0] != szB[0] || szA[1] != szB[1]) {
		throw out_of_range("size of two matrixs does not match!");
	}
	matrix c = A;
#pragma omp parallel 
	{
#pragma omp for  
		for (int i = 0; i < szA[0]; ++i) {
			//printf("I am Thread %d\n", omp_get_thread_num());
			for (int j = 0; j < szA[1]; ++j) {
				c[i][j] -= B[i][j];
			}
		}
	}
	return c;
}
istream& operator >> (istream &is, matrix &m)
{
	for (int i = 0; i < m.row; i++)
	{
		for (int j = 0; j < m.col; j++)
		{
			std::cout << "Input For Row: " << i + 1 << " Col: " << j + 1
				<< " = ";
			is >> m.data[i][j];
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	return is;
}
ostream& operator <<(std::ostream &os, const matrix &m)
{
	for (int i = 0; i < m.row; i++)
	{
		os << " | ";
		for (int j = 0; j < m.col; j++)
		{
			char buf[32];
			double data = m.data[i][j];
			if (m.data[i][j] > -0.00001 && m.data[i][j] < 0.00001)
				data = 0;
			sprintf_s(buf, "%10.2lf ", data);
			os << buf;
		}
		os << "|\n";
	}
	os << "\n\n";
	return os;
}
/*int matrixInversion(double **a, int n)
{
	int *is = new int[n];
	int *js = new int[n];
	int i, j, k;
	double d, p;
	for (k = 0; k < n; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
			for (j = k; j <= n - 1; j++)
			{
				p = fabs(a[i][j]);
				if (p>d) { d = p; is[k] = i; js[k] = j; }
			}
		if (0.0 == d)
		{
			free(is); free(js); printf("err**not inv\n");
			return(0);
		}
		if (is[k] != k)
			for (j = 0; j <= n - 1; j++)
			{
				p = a[k][j];
				a[k][j] = a[is[k]][j];
				a[is[k]][j] = p;
			}
		if (js[k] != k)
			for (i = 0; i <= n - 1; i++)
			{
				p = a[i][k];
				a[i][k] = a[i][js[k]];
				a[i][js[k]] = p;
			}
		a[k][k] = 1.0 / a[k][k];
		for (j = 0; j <= n - 1; j++)
			if (j != k)
			{
				a[k][j] *= a[k][k];
			}
		for (i = 0; i <= n - 1; i++)
			if (i != k)
				for (j = 0; j <= n - 1; j++)
					if (j != k)
					{
						a[i][j] -= a[i][k] * a[k][j];
					}
		for (i = 0; i <= n - 1; i++)
			if (i != k)
			{
				a[i][k] = -a[i][k] * a[k][k];
			}
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
			for (j = 0; j <= n - 1; j++)
			{
				p = a[k][j];
				a[k][j] = a[js[k]][j];
				a[js[k]][j] = p;
			}
		if (is[k] != k)
			for (i = 0; i <= n - 1; i++)
			{
				p = a[i][k];
				a[i][k] = a[i][is[k]];
				a[i][is[k]] = p;
			}
	}
	free(is); free(js);
	return(1);
}*/
matrix inv(matrix b)
{
	double**&a = b.data;
	int& n = b.row;
	int *is = new int[n];
	int *js = new int[n];
	int i, j, k;
	double d, p;
	for (k = 0; k < n; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
			for (j = k; j <= n - 1; j++)
			{
				p = fabs(a[i][j]);
				if (p>d) { d = p; is[k] = i; js[k] = j; }
			}
		if (0.0 == d)
		{
			free(is); free(js);
			throw out_of_range("can not be inversed !");
		}
		if (is[k] != k)
			for (j = 0; j <= n - 1; j++)
			{
				p = a[k][j];
				a[k][j] = a[is[k]][j];
				a[is[k]][j] = p;
			}
		if (js[k] != k)
			for (i = 0; i <= n - 1; i++)
			{
				p = a[i][k];
				a[i][k] = a[i][js[k]];
				a[i][js[k]] = p;
			}
		a[k][k] = 1.0 / a[k][k];
		for (j = 0; j <= n - 1; j++)
			if (j != k)
			{
				a[k][j] *= a[k][k];
			}
		for (i = 0; i <= n - 1; i++)
			if (i != k)
				for (j = 0; j <= n - 1; j++)
					if (j != k)
					{
						a[i][j] -= a[i][k] * a[k][j];
					}
		for (i = 0; i <= n - 1; i++)
			if (i != k)
			{
				a[i][k] = -a[i][k] * a[k][k];
			}
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
			for (j = 0; j <= n - 1; j++)
			{
				p = a[k][j];
				a[k][j] = a[js[k]][j];
				a[js[k]][j] = p;
			}
		if (is[k] != k)
			for (i = 0; i <= n - 1; i++)
			{
				p = a[i][k];
				a[i][k] = a[i][is[k]];
				a[i][is[k]] = p;
			}
	}
	free(is); free(js);
	return std::move(b);
}

int main()
{
	static double a[4][4] = { { 0.2368,0.2471,0.2568,1.2671 },
	{ 1.1161,0.1254,0.1397,0.1490 },
	{ 0.1582,1.1675,0.1768,0.1871 },
	{ 0.1968,0.2071,1.2168,0.2271 } };
	matrix aa(4, 4);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			aa[i][j] = a[i][j];
		}
	}
	cout << "aa is: " << endl;
	cout<<aa << endl;
	matrix b = inv(aa);
	cout << "aa^(-1) is: " << endl;
	cout<<b << endl;
	cout << "aa*aa^(-1) is: " << endl;
	cout<<aa*b << endl;
	system("PAUSE");
	return 0;
}




