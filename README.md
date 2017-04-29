# Class_Matrix_improved_by_openMP
class Matrix imporved by openMP
Implemented multipy threads to  significantly improve the algebra calculation.

to use it, just #include<matrix.h>

Eg.

#include"matrix.h"

#include<iostream>

using std::cout;

using std::cin;

using std::endl;

int main(){

	static double a[4][5] = { { 0.2368,0.2471,0.2568,1.2671,1 },
	
	{ 1.1161,0.1254,0.1397,0.1490,1 },
	
	{ 0.1582,1.1675,0.1768,0.1871,1 },
	
	{ 0.1968,0.2071,1.2168,0.2271,1 } };
	
	matrix aa(4, 5);
	
	for (int i = 0; i < 4; ++i) {
	
		for (int j = 0; j < 5; ++j) {
		
			aa[i][j] = a[i][j];
			
		}
		
	}
	
	cout << "aa is: " << endl;
	
	cout<<aa << endl;
	
	matrix temp(aa[0],5);
		
	cout<<"the first row is: " <<endl;
	
	cout<<temp<<endl;
	
	matrix r=cov(aa);
	
	cout<<"covariance matrix of aa is: "<<endl;
	
	cout<<r<<endl;
	
	cout<<"aa(1,1) is: "<<r.getdata(1,1)<<endl;
	
	matrix bb=aa;
	
	bb.T();
	
	cout<<"aa*aa' is: "<<endl;
	
	cout<<aa*bb<<endl;
	
	return 0;

}

Result:

aa is: 

|     0.2368     0.2471     0.2568     1.2671     1.0000 |

|     1.1161     0.1254     0.1397     0.1490     1.0000 |

|     0.1582     1.1675     0.1768     0.1871     1.0000 |

|     0.1968     0.2071     1.2168     0.2271     1.0000 |




the first row is:


 |     0.2368     0.2471     0.2568     1.2671     1.0000 |




covariance matrix of aa is: 


|     0.2448    -0.0005    -0.0024    -0.0038 |

|    -0.0005     0.2557     0.0036     0.0021 |

|    -0.0024     0.0036     0.2519    -0.0003 |

|    -0.0038     0.0021    -0.0003     0.2480 |


aa(1,1) is:  0.255685


aa*aa' is: 


|     2.7886     1.5200     1.6084     1.6980 |

|     1.5200     2.3031     1.3755     1.4494 |

|     1.6084     1.3755     2.4543     1.5305 |

|     1.6980     1.4494     1.5305     2.6138 |


