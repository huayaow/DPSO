/*
** Discrete DMS-PSO
*/
#pragma once
#include "../common/PSO.h"
#include "DParticle.h"

class D_DMSPSO : public PSO
{
public:
	D_DMSPSO( SUT* s )
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

};