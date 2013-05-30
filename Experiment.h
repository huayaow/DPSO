/*
** Experiment Parameter Tuning
*/
#pragma once
#include"PSO_CPSO.h"
#include"PSO_DPSO.h"
#include<string>

class Experiment
{
public:
	Experiment( SUT *s , Configuration c , double p1 , double p2 , double p3 , int t )
	{
		sut = s ;
		config_default = c ;
		pro1_default = p1 ;
		pro2_default = p2 ;
		pro3_default = p3 ;
		times = t ;
	};
	~Experiment()
	{
	};

public:
	SUT *sut ;
	Configuration config_default ;
	double pro1_default ;
	double pro2_default ;
	double pro3_default ;
	int times ;

	// result
	vector<double> ParSize ;   // mean
	vector<double> ParTime ;   // mean
	vector<int> ParBest ;
	vector<int> ParWorst ;

public:
	// parameter tuning
	void Par_iteration( int* parvalue, int num, string alg ) ;
	void Par_population( int* parvalue, int num, string alg );
	void Par_inertia( double* parvalue, int num, string alg );
	void Par_factor( double* parvalue, int num, string alg );

	void Par_relations( int* parvalue, int sum, int num, string alg );

	void Par_Prob1( double* parvalue, int num, string alg );
	void Par_Prob2( double* parvalue, int num, string alg );
	void Par_Prob3( double* parvalue, int num, string alg );
};

// srand(12)
const int SEED[20] = 
    { 77 , 5628 , 6232 , 29052 , 1558 , 26150 , 12947 , 29926 , 11981 , 22371 , 
    4078 , 28629 , 4665 , 2229 , 24699 , 27370 , 3081 , 18012 , 24965 , 2064 } ;
// srand(7)   
const int SEED1[20] = 
    { 61 , 17422 , 15215 , 7040 , 15521 , 6516 , 30152 , 11794 , 27727 , 20344 , 
    4818 , 28409 , 25042 , 17801 , 12042 , 6215 , 63 , 24189 , 15812 , 15512 };
// srand(201256)
const int SEED2[20] =
    { 1896 , 9475 , 20738 , 7682 , 26044 , 23888 , 14716 , 3755 , 31417 , 14772 , 
    24960 , 12624 , 19444 , 32447 , 15211 , 11946 , 8152 , 22400 , 2851 , 11222 };

	