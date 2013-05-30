/*
** Experiment Compare Study
*/
#pragma once
#include"PSO_CPSO.h"
#include"PSO_DPSO.h"

#include"PSO_DHPSO.h"
#include"PSO_DCLPSO.h"
#include"PSO_DAPSO.h"
#include"PSO_DDMS.h"

#include"PSO_CHPSO.h"
#include"PSO_CCLPSO.h"
#include"PSO_CAPSO.h"
#include"PSO_CDMS.h"
#include<string>

struct CoveringArray
{
	int par ;
	int* val ;
	int tway ;
	vector<SUBINFO> sub ;
};

class ExperimentComp
{
public:
	ExperimentComp( Configuration c1 , Configuration c2 , double p1 , double p2 , double p3 , int t )
	{
		config_cpso = c1 ;
		config_dpso = c2 ;

		pro1_default = p1 ;
		pro2_default = p2 ;
		pro3_default = p3 ;

		times = t ;
	};
	~ExperimentComp()
	{
	};

public:
	Configuration config_cpso ;
	Configuration config_dpso ;
	double pro1_default ;
	double pro2_default ;
	double pro3_default ;
	int times ;

	// result
	vector<double> ParSize ;   // mean
	vector<double> ParTime ;   // mean
	vector<int> ParBest ;
	vector<int> ParWorst ;

	// CA
	vector<CoveringArray> CA ;

public:
	void ClearCA();
	void InitialCA_1();
	void InitialCA_2();
	void InitialCA_3();
	
	void InitialCA_5();
	void InitialCA_6();
	void InitialCA_7();

	void InitialCA_tp();
	void InitialCA_tp1();

	// CPSO and DPSO
	void CompareStudy_1() ;

	// Discrete CLPSO, TVAC, DMS-PSO, APSO
	void CompareStudy_v() ;
	void CompareStudy_v_add();

	// Conventional CLPSO, TVAC, DMS-PSO, APSO
	void CompareStudy_v_c() ;
	void CompareStudy_v_c_1() ;

	// Conventional
	void CompareStudy_cv( string alg );

};
	