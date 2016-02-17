//////////////////////////////////////////////////////////////////
///
/// (C) 2007: ScalingWeb.com
///
/// Author: Anton Fedoruk <afedoruk@scalingweb.com>
///
//////////////////////////////////////////////////////////////////

#include "BTree.h"
#include "StreamBTreeController.h"
#include "JFileStream.h"
#include <iostream>

// For PerformanceTimer
#include "windows.h"

class PerformanceTimer
{
public:

	PerformanceTimer():tick( 0 )
	{
		SetThreadAffinityMask( GetCurrentThread(), 1 );
		Sleep( 0 );
		sample();
	}

	void sample()
	{
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);

		this->deltaTicks = (tick.QuadPart - this->tick);
		if( this->deltaTicks < 0 )
			this->deltaTicks = 0;
		this->tick = tick.QuadPart;
	}

	float deltaTime() const
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		return float(double(this->deltaTicks) / double(freq.QuadPart));
	}

	void print()
	{
		sample();
		printf( "Time:   %8.2f ms\n", deltaTime()*1000.0f );
	}

	__int64 tick;
	__int64 deltaTicks;
};

int main()
{
	typedef BTreeAlgorithms< 1000, guint, guint, StreamBTreeController > BTree;

	BTree bt;
	QString path = "d:/btree.data";
	QString plus = path + ".j";
	JFileStream fstor( path );
	JFileStream jstor( path + ".j" );
	if ( !bt.open( &fstor, 1024*1024*5, &jstor ) )
	{
		return -1;
	}

	srand( ( unsigned int ) time( 0 ) );

	PerformanceTimer pt;

	const guint iters = 1000000;
	typedef std::map< guint, guint > stdmap;
	stdmap smap;
	for ( guint i = 0; i < iters; i++ )
	{
		guint val = rand()*rand()*rand();
		smap[ val ] = val/7*45;
	}

	std::cout << "size: " << smap.size() << std::endl;

	std::cout << "Map Creation ";
	pt.print();

	for ( stdmap::iterator iter = smap.begin(); iter != smap.end(); iter++ )
	{
		if ( !bt.add( iter->first, iter->second ) )
		{
			std::cout << "error!" << std::endl;
		}
	}

	std::cout << "Adding Elements ";
	pt.print();

	for ( stdmap::iterator iter = smap.begin(); iter != smap.end(); iter++ )
	{
		guint data = 0;
		if ( !bt.find( iter->first, data ) )
		{
			std::cout << "error 1!" << std::endl;
		}

		if ( iter->second != data )
		{
			std::cout << "error 2!" << std::endl;
		}
	}

	bt.close();
	std::cout << "Searching Elements ";
	pt.print();

	JFileStream fstor1( path );
	JFileStream jstor1( path + ".j" );
	if ( !bt.open( &fstor1, 1024*1024*5, &jstor1 ) ) return -1;

	for ( stdmap::iterator iter = smap.begin(); iter != smap.end(); iter++ )
	{
		guint data = 0;
		if ( !bt.find( iter->first, data ) )
		{
			std::cout << "error 3!" << std::endl;
		}

		if ( iter->second != data )
		{
			std::cout << "error 4!" << std::endl;
		}
	}

	bt.close();

	std::cout << "Searching Elements After Reopen ";
	pt.print();

	std::cout << "ok" << std::endl;
	std::cin.get();
	return 0;
}
