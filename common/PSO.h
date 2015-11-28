/*
** Base PSO
*/
#pragma once
#include "SUT.h"
#include "Configuration.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>

class PSO
{
public:
	PSO(){};
	PSO( SUT* s )
	{
		sut = s ;
	};
	~PSO()
	{
		clear_PSOResult();
	};

public:
	SUT* sut ;
	Configuration config ;

	// final covering array generated
	vector<int*> PSO_Result ;
	int PSO_SIZE ;
	int PSO_TIME ;

public:
	virtual int* Evolve() { return 0; } ;
	// basic setting for CPSO, C-CLPSO, C-DMSPSO 
	virtual void SetConfig( int par , int ite , double w , double f ) {};
	// setting for TVAC and APSO
	virtual void SetConfig(int par, int ite) {};
	// basci setting for DPSO, D - CLPSO, D - DMSPSO
	virtual void SetConfig(int par, int ite, double w, double f, double p1, double p2, double p3) {};
	// setting for D-TVAC and D-APSO
	virtual void SetConfig( int par , int ite , double p1 , double p2, double p3 ) {};
	void PSOEvolve();
	void writeThisResult(string filename, string info);

public:
	double HammingDist( const int *test );
	void copy( vector<int*> A , vector<int*> &B , int prasize );

private:
	void clear_PSOResult();
};