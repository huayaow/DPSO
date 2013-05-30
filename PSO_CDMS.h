/*
** Conventional DMS-PSO
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class PSO_CDMS : public PSO
{
public:
	PSO_CDMS( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );
	
};