/*
** C_DMSPSO.h : Conventional DMS-PSO
** Reference:
** [1] J. Liang and P. Suganthan, ¡°Dynamic multi-swarm particle swarm optimizer,¡±
**     in Swarm Intelligence Symposium, 2005. SIS 2005. Proceedings
**     2005 IEEE, june 2005, pp. 124 ¨C 129.
*/
#pragma once
#include "../common/PSO.h"
#include "Particle.h"

class C_DMSPSO : public PSO
{
public:
	C_DMSPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );
	
};