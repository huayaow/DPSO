#include"ExperimentComp.h"


void ExperimentComp::InitialCA_tp()
{	
	ClearCA();

	CoveringArray a1 ;
	SUBINFO s ;
	SUBINFO s1 ;
	SUBINFO s2 ;

	// 4
	a1.par = 10 ;
	a1.val = new int[a1.par] ;
	for( int k=0 ; k<a1.par ; k++ )
		a1.val[k] = 4 ;
	a1.tway = 4 ;
	CA.push_back(a1) ;

	a1.par = 10 ;
	a1.val = new int[a1.par] ;
	for( int k=0 ; k<a1.par ; k++ )
		a1.val[k] = 5 ;
	a1.tway = 4 ;
	CA.push_back(a1) ;

	a1.par = 10 ;
	a1.val = new int[a1.par] ;
	for( int k=0 ; k<a1.par ; k++ )
		a1.val[k] = 6 ;
	a1.tway = 4 ;
	CA.push_back(a1) ;


	// CA(2,8,4^3 5^3 6^2,CA)
	/*
	// 6
	a1.sub.clear();
	a1.par = 8 ;
	a1.val = new int[a1.par] ;
	a1.val[0] = 4 ; a1.val[1] = 4 ; a1.val[2] = 4 ;
	a1.val[3] = 5 ; a1.val[4] = 5 ; a1.val[5] = 5 ;
	a1.val[6] = 6 ; a1.val[7] = 6 ;
	a1.tway = 2 ;

	s.subparameter = 3 ;
	s.subposition = new int[s.subparameter];
	s.subposition[0] = 0 ;
	s.subposition[1] = 1 ;
	s.subposition[2] = 2 ;
	s.subway = 3 ;

	s1.subparameter = 4 ;
	s1.subposition = new int[s.subparameter];
	s1.subposition[0] = 3 ;
	s1.subposition[1] = 4 ;
	s1.subposition[2] = 5 ;
	s1.subposition[3] = 6 ;
	s1.subway = 4 ;

	a1.sub.push_back(s);
	a1.sub.push_back(s1);
	CA.push_back(a1) ;
	*/

	/*
	// 7
	a1.sub.clear();
	a1.par = 8 ;
	a1.val = new int[a1.par] ;
	a1.val[0] = 4 ; a1.val[1] = 4 ; a1.val[2] = 4 ;
	a1.val[3] = 5 ; a1.val[4] = 5 ; a1.val[5] = 5 ;
	a1.val[6] = 6 ; a1.val[7] = 6 ;
	a1.tway = 2 ;

	s.subparameter = 5 ;
	s.subposition = new int[s.subparameter];
	s.subposition[0] = 0 ;
	s.subposition[1] = 1 ;
	s.subposition[2] = 2 ;
	s.subposition[3] = 3 ;
	s.subposition[4] = 4 ;
	s.subway = 4 ;

	a1.sub.push_back(s);
	CA.push_back(a1) ;
	*/
}




void ExperimentComp::InitialCA_tp1()
{
	ClearCA();

	// CA(t,10,v)
	CoveringArray a1 ;

	a1.par = 10 ;
	a1.val = new int[a1.par] ;
	for( int k=0 ; k<a1.par ; k++ )
		a1.val[k] = 6 ;
	a1.tway = 4 ;
	CA.push_back(a1) ;

}