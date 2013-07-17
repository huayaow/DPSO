/*
** Conventional Discrete Particle Representation
** use a D-dimention vector to represent particle's position and velocity
*/
#pragma once
#include<cstdlib>
#include<cmath>

class Particle
{
public:
	Particle( int dim , int* ran )
	{
		dimension = dim ;
		range = ran ;

		position = new int[dimension] ;
		velocity = new double[dimension] ;
		pbest = new int[dimension] ;
	};
	~Particle()
	{
	}

public:
	int dimension ;    // the number of dimensions
	int* range ;       // the number of values of each dimension

	// current position£¬velocity and fitness value
	int* position ;
	double* velocity ;
	int fitness_now ;

	// the best personal postion (pbest) and its fitness value
	int* pbest ;
	int fitness_pbest ;

	// clear
	void clear();

public:
	// randomly initialize
	void RandomInit() ;

	// update pbest
	void Setpbest( int fit );

	// update position and velocity
	void velocityUpdate( double weight , double factor , const int* gbest ) ;
	void positionUpdate() ;

	// used for CLPSO
	void velocityUpdateDim( double weight, double factor , const int* pbest, int dim );
};







