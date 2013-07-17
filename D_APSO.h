/*
** Discrete APSO
*/
#pragma once
#include"PSO.h"
#include"DParticle.h"

class D_APSO : public PSO
{
public:
	D_APSO( SUT* s )
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

private:
	double FCalculate( vector<DParticle> T, int* gbest );
	int FuzzyDicsion( double f , int previous );
	double Gaussrand( double E , double V );
};