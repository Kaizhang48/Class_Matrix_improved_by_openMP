//written by KAIZHANG
#ifndef MATRIX
#define MATRIX
#include <iostream>
#include <cmath>
#include <stdlib.h>
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
	friend matrix mean(matrix& );
	friend matrix inv(matrix a);
	friend matrix operator* (const matrix& A,const matrix& B);
	friend matrix operator*( const matrix& A, const double &B);
	friend matrix operator+ ( const matrix& A,  const matrix &B);
	friend matrix operator- ( const matrix& A,  const matrix& B);
	friend ostream& operator <<(std::ostream &os, const matrix &m);
public:
	double*& operator[](int t);
	matrix() = default;
	matrix(double* const a,const int&n);
	matrix(const int&a, const int& b,const double& c=0);
	matrix(const matrix &copyfrom);
	matrix(matrix &&movefrom);
	~matrix();
	int getrow()const;
	int getcol()const;
	double getdata(const int&i,const int&j)const;
	void operator=(const matrix& assignfrom);
	void operator=(matrix&& moveassignfrom);
	vector<int> size() const ;
	matrix& T();

private:
	int row;
	int col;
	double** data;
	void mfree();
};

matrix::matrix(const int&a, const int& b,const double& c) :row(a), col(b) {
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
matrix::matrix(const matrix &copyfrom)
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
matrix::matrix(matrix &&movefrom) {
  if (movefrom.data != data) {
    data = movefrom.data;
    row = movefrom.row;
    col = movefrom.col;
    movefrom.data = nullptr;
    movefrom.row = 0;
    movefrom.col = 0;
  }
}
void matrix::operator=(const matrix& assignfrom) {
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

void matrix::operator=(matrix&& moveassignfrom) {
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

matrix::~matrix()
{
   mfree();
 }

 int matrix::getrow()const{
   return row;
 }

 int matrix::getcol()const{
   return col;
 }

 double matrix::getdata(const int&i,const int&j)const{
   return data[i][j];
 }

 vector<int> matrix::size() const {
   return vector<int>{row, col};
 }

 matrix& matrix::T() {
   double** tempdata = new double*[row];
#pragma omp parallel
   {
#pragma omp for
     for (int i = 0; i < row; ++i) {
       //printf("I am Thread %d\n", omp_get_thread_num());
       tempdata[i] = new double[col];
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
         tempdata[i][j] = data[i][j];
       }
     }
   }
   int tempr = col;
   int tempc = row;
   mfree();
   row = tempr;
   col = tempc;
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
     for (int i = 0; i < row; ++i) {
       for (int j = 0; j < col; ++j) {
         data[i][j] = tempdata[j][i];
       }
     }
   }
#pragma omp parallel
   {
#pragma omp for
     for (int i = 0; i < col; ++i) {
       delete[] tempdata[i];
     }
   }
   delete[] tempdata;
   return *this;
 }

 matrix operator*( matrix& A, const double &B) {
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
 	return std::move(result);
 }

 matrix operator*( const double &B, matrix& A){
 	return std::move(A*B);
 }

 matrix operator+ ( const matrix& A,  const matrix &B) {
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
 					c[i][j] += B.getdata(i,j);
 				}
 			}
 	}
 	return std::move(c);
 }
 matrix operator- ( matrix& A,  matrix& B) {
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
 				c[i][j] -= B.getdata(i,j);
 			}
 		}
 	}
 	return std::move(c);
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
 			sprintf(buf, "%10.4lf ", data);
 			os << buf;
 		}
 		os << "|\n";
 	}
 	os << "\n\n";
 	return os;
 }

 void removemean(matrix& a){
 	matrix b=mean(a);
 #pragma omp parallel
 {
 #pragma omp for
 	for (int i=0;i<a.getrow();++i){
 		for(int j=0;j<a.getcol();++j){
 			a[i][j]-=b[i][0];
 		}
 	}
 }
 }

 matrix cov(matrix a){
 	removemean(a);
 	matrix result(a.getrow(), a.getrow());
 #pragma omp parallel
 {
 #pragma omp for
 	for (int i = 0; i < a.getrow(); ++i) {
 		for (int j = 0; j <= i; ++j) {
 			matrix tempi(a[i], a.getcol());
 			matrix tempj(a[j], a.getcol());
 			result[i][j] = result[j][i] = (tempi*tempj.T())[0][0]/(a.getcol()-1);
 		}
 	}
 }
 	return std::move(result);
 }
 
#endif
