/*
** Discrete Particle
*/
#pragma once

#include<cstdlib>
#include<cmath>
#include<map>
#include<set>
#include<iostream>
using namespace std;

struct PosVal
{
	int posI ;    // 参数组合序号
	int valI ;    // 取值组合序号

	bool operator < (PosVal const& _PV) const
	{
		if( posI < _PV.posI || ( posI == _PV.posI && valI < _PV.valI ) )
			return true ;
		else
			return false ;
	}
};

class DParticle
{
	// 构造函数
public:
	DParticle( int dim , int* ran , int s )
	{
		dimension = dim ;
		range = ran ;
		
		// cal
		strength = s ;
		PosMax = cal_combine( dim , s );
		ValMax = new int[PosMax] ;
		for( int k=0 ; k<PosMax ; k++ )
		{
			int comb = 1 ;
			int *pos = cal_num2pos( k );
			for( int p=0 ; p<s ; p++ )
				comb = comb * range[pos[p]] ;
			delete[] pos ;
			ValMax[k] = comb ;
		}

		//position = NULL ;
		//pbest = NULL ;
		position = new int[dimension] ;
		pbest= new int[dimension] ;
	};
	~DParticle()
	{
		
	}

public:
	// 参数个数及取值的指针
	int dimension ;
	int* range ;

	int strength ;    // 覆盖强度
	int PosMax ;    // 参数组合总数
	int *ValMax ;  // 各取值组合总数

	// 当前位置，速度及适应值
	int* position ;
	map<PosVal,double> velocity ;
	int fitness_now ;

	// 历史上最优位置及适应值
	int* pbest ;
	int fitness_pbest ;

	// 临时
	set<pair<double,PosVal>> TempV ;

public:
	void RandomInit() ;  // 随机初始化

	// 用当前位置更新历史最优
	void Setpbest( int fit );

	// 更新
	void velocityUpdate( double weight , double factor1 , double factor2, double prob1 , const int* gbest ) ;
	void positionUpdate( double prob2 , double prob3 ) ;

	// 用于CLPSO
	void velocityUpdate_Comprehensive( double weight , double factor, double prob1, const int* clbest );

	void clear();

public:
	int cal_combine( const int n , const int m );   // C(n,m)
	int cal_pos2num( const int *pos );  // 在C(n,m)中，依据具体位置pos返回序号
	int* cal_num2pos( int t );  // 在C(n,m)中，依据序号返回具体位置pos

	int cal_val2num( const int *val , const int *pos );   // 在pos位置，依据val返回序号  
	int* cal_num2val( int t , const int *pos );     // 在pos位置，依据序号返回val

};







