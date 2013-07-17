/*
** Experiment Comparison Study
*/
#pragma once
#include"PSO_CPSO.h"
#include"PSO_DPSO.h"
#include"D_TVAC.h"
#include"D_CLPSO.h"
#include"D_APSO.h"
#include"D_DMSPSO.h"
#include"C_TVAC.h"
#include"C_CLPSO.h"
#include"C_APSO.h"
#include"C_DMSPSO.h"
#include<fstream>
#include<iostream>
#include<ctime>
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

	// CPSO and DPSO
	void CompareStudy() ;

	// Discrete CLPSO, TVAC, DMS-PSO, APSO
	void CompareStudy_d() ;

	// Conventional CLPSO, TVAC, DMS-PSO, APSO
	void CompareStudy_c() ;


};
	