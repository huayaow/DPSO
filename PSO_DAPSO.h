/*
** D-APSO
*/
#pragma once
#include"PSO.h"
#include"DParticle.h"

class PSO_DAPSO : public PSO
{
public:
	PSO_DAPSO( SUT* s )
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

	double FCalculate( vector<DParticle> T, int* gbest );
	int FuzzyDicsion( double f , int previous );
	double Gaussrand( double E , double V );
};