/*
** C_APSO.h : Conventional APSO
** Referenec:
** [1] Z. hui Zhan, J. Zhang, Y. Li, and H.-H. Chung, ¡°Adaptive particle swarm
**     optimization,¡± Systems, Man, and Cybernetics, Part B: Cybernetics,
**     IEEE Transactions on, vol. 39, no. 6, pp. 1362 ¨C1381, dec. 2009.
*/
#pragma once
#include "../common/PSO.h"
#include "Particle.h"

class C_APSO : public PSO
{
public:
	C_APSO( SUT* s )
	{
		sut = s ;
	};

public:
	int* Evolve();
	void SetConfig( int par , int ite  );
	
private:
	double FCalculate( vector<Particle> T, int* gbest );
	int FuzzyDicsion( double f , int previous );
	double Gaussrand( double E , double V );
};