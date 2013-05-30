/*
** Conventional PSO
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class PSO_CPSO : public PSO
{
public:
	PSO_CPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );
	
};