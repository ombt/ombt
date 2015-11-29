#include <stdio.h>
#include <iostream>

using namespace std;

template <class DT, int DTSZ> class HalfSizeDataTypePolicy;

template <> 
class HalfSizeDataTypePolicy<unsigned long long, 8 > 
{
public:
	static const unsigned long long BitShift = 32;
	static const unsigned long long BitMask = 0xffffffff;
	static const unsigned long long Base = 0x100000000ull;
	static const unsigned long long MaxDigit = 0xffffffff;
	typedef unsigned long DataType;
};

template <> 
class HalfSizeDataTypePolicy<unsigned long, 4 > 
{
public:
	static const unsigned long BitShift = 16;
	static const unsigned long BitMask = 0x0000ffff;
	static const unsigned long Base = 0x00010000;
	static const unsigned long MaxDigit = 0x0000ffff;
	typedef unsigned short DataType;
};

template <> 
class HalfSizeDataTypePolicy<unsigned short, 2 > 
{
public:
	static const unsigned short BitShift = 8;
	static const unsigned short BitMask = 0x00ff;
	static const unsigned short Base = 0x0100;
	static const unsigned short MaxDigit = 0x00ff;
	typedef unsigned char DataType;
};

template <> 
class HalfSizeDataTypePolicy<unsigned int, 4 > 
{
public:
	static const unsigned int BitShift = 16;
	static const unsigned int BitMask = 0x0000ffff;
	static const unsigned int Base = 0x00010000;
	static const unsigned int MaxDigit = 0x0000ffff;
	typedef unsigned short DataType;
};

template <> 
class HalfSizeDataTypePolicy<unsigned int, 2 > 
{
public:
	static const unsigned int BitShift = 8;
	static const unsigned int BitMask = 0x00ff;
	static const unsigned int Base = 0x0100;
	static const unsigned int MaxDigit = 0x00ff;
	typedef unsigned char DataType;
};

#define PrintIt(EXPR) \
	cerr << #EXPR << " = " << EXPR << endl;

int
main(int, char**)
{
	cerr << endl;
	PrintIt((HalfSizeDataTypePolicy<unsigned long long, sizeof(unsigned long long)>::BitShift));
	PrintIt((HalfSizeDataTypePolicy<unsigned long long, sizeof(unsigned long long)>::BitMask));
	PrintIt((HalfSizeDataTypePolicy<unsigned long long, sizeof(unsigned long long)>::Base));
	PrintIt((HalfSizeDataTypePolicy<unsigned long long, sizeof(unsigned long long)>::MaxDigit));

	//cerr << endl;
	//PrintIt((HalfSizeDataTypePolicy<unsigned long, sizeof(unsigned long)>::BitShift));
	//PrintIt((HalfSizeDataTypePolicy<unsigned long, sizeof(unsigned long)>::BitMask));
	//PrintIt((HalfSizeDataTypePolicy<unsigned long, sizeof(unsigned long)>::Base));
	//PrintIt((HalfSizeDataTypePolicy<unsigned long, sizeof(unsigned long)>::MaxDigit));

	cerr << endl;
	PrintIt((HalfSizeDataTypePolicy<unsigned short, sizeof(unsigned short)>::BitShift));
	PrintIt((HalfSizeDataTypePolicy<unsigned short, sizeof(unsigned short)>::BitMask));
	PrintIt((HalfSizeDataTypePolicy<unsigned short, sizeof(unsigned short)>::Base));
	PrintIt((HalfSizeDataTypePolicy<unsigned short, sizeof(unsigned short)>::MaxDigit));

	cerr << endl;
	PrintIt((HalfSizeDataTypePolicy<unsigned int, sizeof(unsigned int)>::BitShift));
	PrintIt((HalfSizeDataTypePolicy<unsigned int, sizeof(unsigned int)>::BitMask));
	PrintIt((HalfSizeDataTypePolicy<unsigned int, sizeof(unsigned int)>::Base));
	PrintIt((HalfSizeDataTypePolicy<unsigned int, sizeof(unsigned int)>::MaxDigit));
	return 0;
}
