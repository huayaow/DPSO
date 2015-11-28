/*
** Conventional PSO
*/
#pragma once
#include "../common/PSO.h"
#include "Particle.h"

class C_PSO : public PSO
{
public:
	C_PSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );
	
};