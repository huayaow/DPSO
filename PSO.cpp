/*
** Base PSO
*/
#include"PSO.h"

#include<ctime>
#include<cstdlib>
#include<cmath>
#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// 清理
// ----------------------------------------------------------------------------
void PSO::clear_PSOResult()
{
	for( vector<int*>::iterator i = PSO_Result.begin() ; i != PSO_Result.end() ; i++ )
		delete[] (*i) ;
	PSO_Result.clear();
}

void PSO::copy( vector<int*> A , vector<int*> &B , int prasize )
{
	// 判断B是否已分配，若已分配，删除B
	if( B.size() != 0 )
	{
		for( vector<int*>::iterator i = B.begin() ; i != B.end() ; i++ )
			delete[] (*i) ;
		B.clear();
	}
	// 分配新空间，并加入
	for( vector<int*>::iterator i = A.begin() ; i != A.end() ; i++ )
	{
		int *test = new int[prasize] ;
		for( int v=0 ; v<prasize ; v++ )
			test[v] = (*i)[v] ;
		B.push_back(test);
	}
}


// ----------------------------------------------------------------------------
// Hamming Distance
// ----------------------------------------------------------------------------
double PSO::HammingDist( const int *test )
{
	int hamming = 0 ;
	if( PSO_Result.size() == 0 )
		return 0 ;

	for( vector<int*>::const_iterator i = PSO_Result.begin() ; i != PSO_Result.end() ; i++ )
	{
		int ham = 0 ;
		for( int k=0 ; k<sut->parameter ; k++ )
		{
			if( test[k] != (*i)[k] )
				ham++ ;
		}
		hamming += ham ;
	}
	double h = (double)hamming / (double)PSO_Result.size() ;
	return h ;
}


// ----------------------------------------------------------------------------
// One-test-at-a-time
// ----------------------------------------------------------------------------
void PSO::PSOEvolve()
{
	PSO_SIZE = MAX ;
	PSO_TIME = MAX ;
	//PSO_COV = 0 ;

	// 清空
	clear_PSOResult(); 
	//FEtimes = 0 ;

	//
	// 开始执行
	//

	// 初始化
	sut->GenerateS();
	time_t nowtime1 = time( 0 );

	// 逐一生成测试用例
	while( sut->SCount != 0 )
	{
		int *best = Evolve();
		PSO_Result.push_back( best );
		int cov = sut->FitnessValue( best , 1 );

		cout<<PSO_Result.size()<<", "<<sut->SCountAll-sut->SCount<<endl;

		//if( FEtimes > 10000 )
		//	break;
	}

	time_t nowtime2 = time(0);
	
	PSO_SIZE = PSO_Result.size();
	PSO_TIME = nowtime2 - nowtime1 ;
	//PSO_COV = sut->SCountAll - sut->SCount ;

	// 如果有种子，最后加入TResult
	if( sut->seedInfo.size() != 0 )
	{
		vector<int*> temp ;
		copy( PSO_Result , temp , sut->parameter );
		clear_PSOResult(); 
		copy( sut->seedInfo , PSO_Result , sut->parameter );
		for( vector<int*>::iterator i = temp.begin() ; i != temp.end() ; i++ )
			PSO_Result.push_back((*i)) ;
		PSO_SIZE = PSO_SIZE + sut->seedInfo.size() ;
	}
}