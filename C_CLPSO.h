/*
** C_CLPSO.h : Conventional CLPSO
** Reference:
** [1] J. Liang, A. Qin, P. Suganthan, and S. Baskar, ¡°Comprehensive learning
**     particle swarm optimizer for global optimization of multimodal functions,¡±
**     Evolutionary Computation, IEEE Transactions on, vol.10, no.3, 2006.
*/
#pragma once
#include"PSO.h"
#include"Particle.h"

class C_CLPSO : public PSO
{
public:
	C_CLPSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite , double w , double f );
};