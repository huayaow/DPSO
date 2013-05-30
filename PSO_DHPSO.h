/*
** D-TVAC
*/
#pragma once
#include"PSO.h"
#include"DParticle.h"

class PSO_DHPSO : public PSO
{
public:
	PSO_DHPSO( SUT* s )
	{
		sut = s ;
	};

public:
	double pro1_threshold ;
	double pro2_threshold ;
	double pro3_threshold ;

public:
	int* Evolve();
	void SetConfig( int par , int ite , double p1 , double p2 , double p3 );

};