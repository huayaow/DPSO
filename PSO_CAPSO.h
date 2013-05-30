/*
** Conventional APSO
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class PSO_CAPSO : public PSO
{
public:
	PSO_CAPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite  );
	
	double FCalculate( vector<Particle> T, int* gbest );
	int FuzzyDicsion( double f , int previous );
	double Gaussrand( double E , double V );
};