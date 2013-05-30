/*
** Conventional Particle
*/
#pragma once

#include<cstdlib>
#include<cmath>
using namespace std;

class Particle
{
	// 构造函数
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
	// 参数个数及取值的指针
	int dimension ;
	int* range ;

	// 当前位置，速度及适应值
	int* position ;
	double* velocity ;
	int fitness_now ;

	// 历史上最优位置及适应值
	int* pbest ;
	int fitness_pbest ;

public:
	void RandomInit() ;  // 随机初始化

	// 用当前位置更新历史最优
	void Setpbest( int fit );

	// 更新
	void velocityUpdate( double weight , double factor , const int* gbest ) ;
	void positionUpdate() ;

	// CLPSO更新
	void velocityUpdateDim( double weight, double factor , const int* pbest, int dim );

	void clear();

};







