/*
** C_TVAC.h : Conventional TVAC
** Reference:
** [1] A. Ratnaweera, S. Halgamuge, and H. Watson, ¡°Self-organizing hierarchical
**     particle swarm optimizer with time-varying acceleration coefficients,¡±
**     Evolutionary Computation, IEEE Transactions on, vol.8, no.3, 2004.
*/
#pragma once
#include "../common/PSO.h"
#include "Particle.h"

class C_TVAC : public PSO
{
public:
	C_TVAC( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite );
	
};