/*
** CLPSO
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class PSO_CCLPSO : public PSO
{
public:
	PSO_CCLPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );

};