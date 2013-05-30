/*
** Conventional CLPSO
*/
#include"PSO_CCLPSO.h"

// ----------------------------------------------------------------------------
// 设置及初始化
// ----------------------------------------------------------------------------
void PSO_CCLPSO::SetConfig( int par , int ite , double w , double f )
{
	config.population = par ;
	config.iteration = ite ;
	config.weight = w ;
	config.factor = f ;
}

// ----------------------------------------------------------------------------
// 生成一条测试用例
// ----------------------------------------------------------------------------
int* PSO_CCLPSO::Evolve()
{
	// 返回值
	int *best = new int[sut->parameter] ;  
	
	vector<Particle> T ; 	               // 粒子群
	int *gBest = new int[sut->parameter];  // 种群最优
	int fitbest = 0 ;

	// 初始化粒子群
	for( int i = 0 ; i < config.population ; i++ )
	{
		Particle a( sut->parameter , sut->value ) ;
		a.RandomInit();

		T.push_back(a);
	}

	// gBest = T[0]
	vector<Particle>::iterator x = T.begin();     
	for( int c = 0 ; c < sut->parameter ; c++)
		gBest[c] = (*x).position[c] ;

	// 迭代次数
	int it = 1 ;

	// PC[i]
	double *Pc = new double[config.population] ;
	for( int k = 0 ; k < config.population ; k++ )
		Pc[k] = 0.05 + 0.45 * ((exp((double)(10*k)/(double)(config.population-1)) - 1) / (exp((double)10) - 1));

	// 生成一个测试用例，gBest
	while( true )
	{
		// 计算每个粒子的fitness值，并更新pbest，gbest
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

			// 若fitness(t) = coverMax ， 返回
			if( fit == sut->testcaseCoverMax )   	
			{
				for( int c = 0 ; c< sut->parameter ; c++)
					best[c] = (*i).position[c] ;

				delete[] gBest ;
				for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
					i->clear();
				T.clear();

				return best ;
			}

			// 更新pBest
			if ( fit > (*i).fitness_pbest )
				(*i).Setpbest( fit );
			
			// 更新gBest
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
		}  // end for

		// 中止条件
		if ( it >= config.iteration )
			break ;

		// 更新粒子群
		int index = 0 ;
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )  
		{
			// each dimension
			for( int d = 0 ; d < sut->parameter ; d++ )
			{
				// CL
				// random > Pc[i] , learn from own pbest
				if( (double)(rand()%1000)/1000.0 > Pc[index] )
					i->velocityUpdateDim( config.weight , config.factor , i->pbest , d );
				// random < Pc[i] , learn from other pbest
				else
				{
					int f1, f2 ;
					do {
						f1 = (int)(((double)(rand()%1000)/1000.0)*config.population) ;
					} while( f1 == index );
					do {
						f2 = (int)(((double)(rand()%1000)/1000.0)*config.population) ;
					} while( f2 == index );

					vector<Particle>::const_iterator x1 = T.begin() + f1 ;
					vector<Particle>::const_iterator x2 = T.begin() + f2 ;
					
					if( sut->FitnessValue(x1->position,0) > sut->FitnessValue(x2->position,0) )
						i->velocityUpdateDim( config.weight , config.factor , x1->pbest , d );
					else
						i->velocityUpdateDim( config.weight , config.factor , x2->pbest , d );	
				}
			}
			i->positionUpdate();
		} 

		// iteration++
		it++ ;

	}  // end while

	for( int k = 0 ; k < sut->parameter ; k++ )   // best = gBest.position
		best[k] = gBest[k] ;

	delete[] gBest ;
	for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		i->clear();
	T.clear();

	return best ;
}