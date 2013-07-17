#include<iostream>
#include<fstream>
#include<ctime>
#include<Windows.h>
#include"Experiment.h"
#include"ExperimentComp.h"

int main()
{
	// example: use conventional CLPSO to generate CA(2,10,4) 
	int p = 10 ;
	int *v = new int[p] ;
	for( int k=0 ; k<p ; k++)
		v[k] = 4 ;
	int t = 2 ;

	SUT *sut = new SUT( p , v ,t );

	PSO *pso = new C_CLPSO(sut);
	pso->SetConfig( 100 , 1000 , 0.3 , 1.3 );
	pso->PSOEvolve();
	cout<<"final: "<<pso->PSO_SIZE<<endl;

	return 0 ;

	// example: sub covering array setting
	/*
	vector<SUBINFO> subInfo;
	SUBINFO s ;
	s.subparameter = 5 ;
	s.subposition = new int[s.subparameter];
	s.subposition[0] = 0 ;
	s.subposition[1] = 1 ;
	s.subposition[2] = 2 ;
	s.subposition[3] = 3 ;
	s.subposition[4] = 4 ;
	s.subway = 4 ;
	subInfo.push_back(s);
	sut->SetSub(subInfo);
	*/

	// example: compare CPSO and DPSO
	/*
	Configuration config_cpso ;
	config_cpso.population = 80 ;
	config_cpso.iteration = 250 ;
	config_cpso.weight = 0.9 ;
	config_cpso.factor = 1.3 ;

	Configuration config_dpso ;
	config_dpso.population = 80 ;
	config_dpso.iteration = 250 ;
	config_dpso.weight = 0.5 ;
	config_dpso.factor = 1.3 ;

	ExperimentComp *expc = new ExperimentComp(config_cpso, config_dpso, 0.5, 0.3, 0.7, 2);
	expc->CompareStudy();

	return 0 ;
	*/
	
	// example: parameter tunning for CPSO and DPSO
	/*
	Configuration config ;
	config.population = 20 ;
	config.iteration = 50 ;
	config.weight = 0.9 ;
	config.factor = 1.3 ;
	Experiment *exp = new Experiment(sut, config, 0.5, 0.5, 0.5, 30);

	int parIter[13] = {50, 150, 250, 350, 450, 550, 750, 950, 1150, 1350, 1550, 1750, 1950} ;
	int parSize[7] = {10, 20, 30, 40, 60, 80, 100} ;
	double parInertia[6] = {0.1, 0.3, 0.5, 0.7, 0.9, 1.1};
	double parFactor[8] = {0.1, 0.5, 0.9, 1.3, 1.7, 2.1, 2.5, 2.9};
	int rel[7] = {10, 20, 30, 40, 60, 80, 100};
	double pro[5] = {0.1, 0.3, 0.5, 0.7, 0.9};

	exp->Par_iteration(parIter, 13, "CPSO");
	exp->Par_population(parSize, 7, "CPSO");
	exp->Par_inertia(tpi, 1, "DPSO");
	exp->Par_factor(parFactor, 8, "CPSO");
	exp->Par_relations( rel , 20000 , 7 , "DPSO" );

	exp->Par_Prob1(pro, 5, "DPSO");
	exp->Par_Prob2(pro, 5, "DPSO");
	exp->Par_Prob3(pro, 5, "DPSO");

	return 0 ;
	*/
	
	// example: run N times to get average results
	/*
	srand( time(0) ) ;
	int meansize = 0 ;
	int meantime = 0 ;

	// run N times
	ofstream outfile("temp.txt");
	ofstream outfile_t("temp_t.txt");
	int N = 10 ;
	int *result = new int[N] ;
	for( int i=0 ; i<N ; i++ )
	{
		pso->PSOEvolve();
		cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<")"<<endl;
		outfile<<pso->PSO_SIZE<<endl;
		outfile_t<<pso->PSO_TIME<<endl;
		meansize += pso->PSO_SIZE ;
		meantime += pso->PSO_TIME ;
		result[i] = pso->PSO_SIZE ;	
	}

	cout<<endl;
	cout<<double(meansize)/(double)N<<"("<<double(meantime)/(double)N<<")"<<endl;
	outfile<<"mean "<<double(meansize)/(double)N<<endl;
	outfile_t<<"mean "<<double(meantime)/(double)N<<endl;

	delete[] result ;

	return 0 ;
	*/
}