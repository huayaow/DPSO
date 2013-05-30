/*
** Base PSO
*/
#pragma once
#include"SUT.h"
#include"Configuration.h"

class PSO
{
public:
	PSO()
	{
	};
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

	// ◊Ó÷’≤‚ ‘”√¿˝ºØ
	vector<int*> PSO_Result ;
	int PSO_SIZE ;
	int PSO_TIME ;

public:
	virtual int* Evolve() { return NULL; } ;
	virtual void SetConfig( int par , int ite , double w , double f ) {};
	virtual void SetConfig( int par , int ite ) {};
	virtual void SetConfig( int par , int ite , double w , double f , double p1 , double p2, double p3 ) {};
	virtual void SetConfig( int par , int ite , double p1 , double p2 , double p3 ) {};
	void PSOEvolve();

	double HammingDist( const int *test );
	void copy( vector<int*> A , vector<int*> &B , int prasize );
	void clear_PSOResult();

};