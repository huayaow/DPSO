/*
** DPSO
*/
#pragma once
#include"PSO.h"
#include"DParticle.h"

class PSO_DPSO : public PSO
{
public:
	PSO_DPSO( SUT* s )
	{
		sut = s ;
	};

public:
	double pro1_threshold ;
	double pro2_threshold ;
	double pro3_threshold ;

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 );
	//double HammingDist( const int *test );
};