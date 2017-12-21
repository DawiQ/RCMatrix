#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class WrongDim{};
class bad_alloc2{};
class IndexOutOfRange{};
class BadIndex{};

class CMatrix{
	private:
		struct rcmatrix;
		rcmatrix* macierz;
	public:
		class Range{};

		CMatrix( CMatrix& source );
		~CMatrix();
		CMatrix( unsigned int row, unsigned int col, double diag, double pose );

		class Cref1;
		class Cref2;
		Cref1 operator[](unsigned int row);

		friend ostream& operator<<(ostream& object, const CMatrix& source);

		CMatrix& operator=(const CMatrix& source);
		CMatrix operator*( const CMatrix& source) const;
		CMatrix( fstream& fileStream );

		void write( int, int, double );
		double read( int i, int j );
};

struct CMatrix::rcmatrix{
	double** m;
  	unsigned int row, col, n;

	rcmatrix( unsigned int nrow, unsigned int ncol){
		n=1;
		row = nrow;
		col = ncol;

		m = new double*[row];
		unsigned int i;

		try{
			for( i = 0; i < row; ++i)
				m[i] = new double[col];	
		}catch( bad_alloc ){
			unsigned int j;
			for( j = 0; j < i; j++ )
				delete [] m[j];
	
			delete [] m;
			throw;
		}
		
	};
	  
	~rcmatrix(){
		unsigned int i = 0;
		for( i = 0; i < row; i++ )
			delete [] m[i];

		delete [] m;
	};

	rcmatrix* detach(){
		if(n==1)
			return this;
		
		rcmatrix* t = new rcmatrix( row, col );
		unsigned int i, j;
	
		for( i = 0; i < row; i++ )
			for( j = 0; j < col; j++ )
				t -> m[i][j] = m[i][j];
		
		n--;

		return t;
	};

};

class CMatrix::Cref1{
public:	
	friend class CMatrix;
	CMatrix& macierz;

	unsigned int row;

	Cref1( CMatrix& matrix, unsigned int nrow): macierz(matrix), row(nrow){};

	CMatrix::Cref2 operator[](unsigned int col);
};

class CMatrix::Cref2{
public:
	friend class CMatrix;
	CMatrix& macierz;

	unsigned int row, col;

	Cref2( CMatrix& matrix, unsigned int nrow, unsigned int ncol): macierz(matrix), row(nrow), col(ncol){};

	operator double() const{
		return macierz.read( row, col );
	}

	CMatrix::Cref2 operator=( double value ){
		macierz.write( row, col, value );
		return *this;
	}
};

CMatrix::Cref2 CMatrix::Cref1::operator[](unsigned int col){
	if( macierz.macierz->row <= row )	
		throw BadIndex();

	return CMatrix::Cref2(macierz, row, col);
}

CMatrix::Cref1 CMatrix::operator[](unsigned int row){
	return Cref1( *this, row );
}

CMatrix::CMatrix( unsigned int row = 0, unsigned int col = 0, double diag = 0.0, double pose = 0.0 ){	
	macierz = new rcmatrix( row, col );

	unsigned i, j;

	for( i = 0; i < row; i++ )
		for( j = 0; j < col; j++ )
			if( i == j )
				macierz -> m[i][j] = diag;
			else
				macierz -> m[i][j] = pose;
		
}

CMatrix::CMatrix( CMatrix& source ){
	source.macierz->n++;
	macierz = source.macierz;
}

CMatrix::~CMatrix(){
	if( --macierz->n == 0 )
		delete macierz;
}

ostream& operator<<(ostream& object, const CMatrix& source){
	unsigned int i, j;

	for( i = 0; i < source.macierz -> row; i++ ){
		for( j = 0; j < source.macierz -> col; j++ )
			object << source.macierz -> m[i][j] << " ";
		cout << endl;
	}
	
	return object;
}


CMatrix& CMatrix::operator=(const CMatrix& source){
	source.macierz -> n++;
	if( --macierz -> n == 0 )
		delete macierz;

	macierz = source.macierz;
	return *this;
}


CMatrix CMatrix::operator*( const CMatrix& source) const{
	
	if( macierz -> col != source.macierz->row )
		throw WrongDim();

	CMatrix multiplied( macierz -> row, source.macierz->col, 0.0, 0.0 );

	double sum = 0.0;
	unsigned int i, j, k;

	for( i = 0; i < macierz->row; i++ )
		for( j = 0; j < source.macierz->col; j++ ){
			for( k = 0; k < source.macierz->row; k++ )
				sum += macierz->m[i][k] * source.macierz->m[k][j];

			multiplied.macierz->m[i][j] = sum;
			sum = 0.0;
		}

	return multiplied;
}


// Na wejsciu dostaje od razu uchwyt do pliku
// Nie musze martwic sie o zamkniecie strumienia
CMatrix::CMatrix( fstream& fileStream ){
	unsigned int row = 0, col = 0, i = 0, j = 0;

	fileStream >> row; // zczytuje ilosc wierszy
	fileStream >> col; // zczytuje ilosc kolumn

	macierz = new rcmatrix( row, col );

	// zapelniam tablice danymi z pliku
	for( i = 0; i < row; i++ )
		for( j = 0; j < col; j++ )
			fileStream >> macierz->m[i][j];
}

void CMatrix::write( int i, int j, double value ){
	macierz = macierz -> detach();
	macierz -> m[i][j] = value;
}

double CMatrix::read( int i, int j ){
	return macierz -> m[i][j];
}
