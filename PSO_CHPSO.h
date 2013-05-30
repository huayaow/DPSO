/*
** Conventional TVAC
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class PSO_CHPSO : public PSO
{
public:
	PSO_CHPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite );
	
};