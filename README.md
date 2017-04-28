# class_Matrix_improved_by_openMP
class Matrix imporved by openMP
Implemented multipy threads to improve the algebra calculation significantly.

to use it, just #include<matrix.h>

Eg.＜/br＞
#include"matrix.h"＜/br＞
#include<iostream>＜/br＞
using std::cout;＜/br＞
using std::cin;＜/br＞
using std::endl;＜/br＞

int main()＜/br＞
{＜/br＞
	static double a[4][5] = { { 0.2368,0.2471,0.2568,1.2671,1 };＜/br＞
	{ 1.1161,0.1254,0.1397,0.1490,1 },＜/br＞
	{ 0.1582,1.1675,0.1768,0.1871,1 },＜/br＞
	{ 0.1968,0.2071,1.2168,0.2271,1 } };＜/br＞
	matrix aa(4, 5);＜/br＞
	for (int i = 0; i < 4; ++i) {＜/br＞
		for (int j = 0; j < 5; ++j) {＜/br＞
			aa[i][j] = a[i][j];＜/br＞
		}＜/br＞
	}＜/br＞
	cout << "aa is: " << endl;＜/br＞
	cout<<aa << endl;＜/br＞
	matrix temp(aa[0],5);＜/br＞
	matrix tempi(aa[0],5);＜/br＞
	cout<<temp<<endl;＜/br＞
	cout<<tempi<<endl;＜/br＞
	matrix r=cov(aa);＜/br＞
	cout<<r<<endl;＜/br＞
	cout<<r.getdata(1,1)<<endl;＜/br＞
	matrix bb=aa;＜/br＞
	bb.T();＜/br＞
	cout<<aa*bb<<endl;＜/br＞
	return 0;＜/br＞
}＜/br＞

Result:＜/br＞ 
aa is: ＜/br＞
 |     0.2368     0.2471     0.2568     1.2671     1.0000 |＜/br＞
 |     1.1161     0.1254     0.1397     0.1490     1.0000 |＜/br＞
 |     0.1582     1.1675     0.1768     0.1871     1.0000 |＜/br＞
 |     0.1968     0.2071     1.2168     0.2271     1.0000 |＜/br＞


＜/br＞
 |     0.2368     0.2471     0.2568     1.2671     1.0000 |＜/br＞

＜/br＞

 |     0.2368     0.2471     0.2568     1.2671     1.0000 |＜/br＞



 |     0.2448    -0.0005    -0.0024    -0.0038 |＜/br＞
 |    -0.0005     0.2557     0.0036     0.0021 |＜/br＞
 |    -0.0024     0.0036     0.2519    -0.0003 |＜/br＞
 |    -0.0038     0.0021    -0.0003     0.2480 |＜/br＞



0.255685＜/br＞
＜/br＞
 |     2.7886     1.5200     1.6084     1.6980 |＜/br＞
 |     1.5200     2.3031     1.3755     1.4494 |＜/br＞
 |     1.6084     1.3755     2.4543     1.5305 |＜/br＞
 |     1.6980     1.4494     1.5305     2.6138 |＜/br＞




