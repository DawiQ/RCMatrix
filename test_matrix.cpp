#include <iostream>
#include "matrix.h"

using namespace std;

int main()
{
	try
	{
		cout << endl;

	  	CMatrix A1(2,5,1.0);			//Create matrix A1 = [	1.0  0.0  0.0  0.0  0.0
							//			0.0  1.0  0.0  0.0  0.0  ]
		cout << A1 << endl;
	
		CMatrix A2(5,3,0.0,6.3);		//Create matrix A1 = [	0.0  6.3  6.3 
							//			6.3  0.0  6.3 
							//			6.3  6.3  0.0
							//			6.3  6.3  6.3
							//			6.3  6.3  6.3  ]
		cout << A2 << endl;
		
		CMatrix S = A1 * A2;			//Multiply A1 by A2
		cout << S << endl;
	
		fstream f1;
		f1.open("matrix.dat", fstream::in);
		CMatrix B(f1);				//Read the matrix data from file matrix.dat
		f1.close();				//First two values in this file specify the matrix dimensions
		cout << B << endl;
		
		S = B;						//Assign B to S
		S[0][0] = 1.4;					//Modify S
		cout << "S[0][0]=" << S[0][0] << endl;		//Verify S
		cout << "B[0][0]=" << B[0][0] << endl;		//Verify B
	}
	catch(IndexOutOfRange&)
	{
		cout << "Index Out of Range" << endl;
	}
	catch(WrongDim&)
	{
		cout << "Wrong Matrix Dimensions" << endl;
	}
	catch(bad_alloc)
	{
		cout << "Out of Memory" << endl;
	}
};
