/*
** D-CLPSO
*/
#include"PSO_DCLPSO.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// 设置及初始化
// ----------------------------------------------------------------------------
void PSO_DCLPSO::SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 )
{
	config.population = par ;
	config.iteration = ite ;
	config.weight = w ;
	config.factor = f ;

	pro1_threshold = p1 ;
	pro2_threshold = p2 ;
	pro3_threshold = p3 ;
}


// ----------------------------------------------------------------------------
// 生成一条测试用例
// ----------------------------------------------------------------------------
int* PSO_DCLPSO::Evolve()
{
	/*
	ofstream outfile("result.txt",ios::app);
	outfile<<"-------------------------------------"<<endl;
	outfile<<"Evolve"<<endl;
	outfile<<"-------------------------------------"<<endl;
	*/

	// 返回值
	int *best = new int[sut->parameter] ;  
	
	vector<DParticle> T ; 	               // 粒子群

	// 种群最优位置及适应值
	int *gBest = new int[sut->parameter];  
	int fitbest = 0 ;

	// 初始化粒子群
	for( int i = 0 ; i < config.population ; i++ )
	{
		DParticle a( sut->parameter , sut->value , sut->tway ) ;
		a.RandomInit();

		T.push_back(a);
	}

	// gBest = T[0]
	vector<DParticle>::iterator x = T.begin();     
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
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

			// 若fitness(t) = coverMax
			//if( fit == sut->testcaseCoverMax )
			if( fit == sut->testcaseCoverMax && PSO_Result.size() == 0 )
			{
				for( int c = 0 ; c< sut->parameter ; c++)
					best[c] = (*i).position[c] ;

				delete[] gBest ;
				for( vector<DParticle>::iterator j = T.begin() ; j != T.end() ; j++ )
					j->clear();
				T.clear();

				return best ;
			}

			// 更新pBest
			if ( fit > i->fitness_pbest )
				i->Setpbest( fit );
			
			// 更新gBest
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
			
			else if( fit == fitbest )
			{
				if( HammingDist((*i).position) < HammingDist(gBest) )
				{
					for( int c = 0 ; c< sut->parameter ; c++)
						gBest[c] = (*i).position[c] ;
				}
			}
			
		}  // end for

		// 中止条件
		if ( it >= config.iteration )
			break ;

		// 更新粒子群
		int index = 0 ;
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )  
		{
			// CL
			// random > Pc[i] , learn from own pbest
			if( (double)(rand()%1000)/1000.0 > Pc[index] )
			{
				i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , i->pbest );
				i->positionUpdate( pro2_threshold , pro3_threshold );
			}
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

				vector<DParticle>::const_iterator x1 = T.begin() + f1 ;
				vector<DParticle>::const_iterator x2 = T.begin() + f2 ;

				if( sut->FitnessValue(x1->position,0) > sut->FitnessValue(x2->position,0) )
					i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , x1->pbest );
				else
					i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , x2->pbest );
				
				i->positionUpdate( pro2_threshold , pro3_threshold );
			}
		} 
		
		// iteration++
		it++ ;

	}  // end while

	for( int k = 0 ; k < sut->parameter ; k++ )   // best = gBest.position
		best[k] = gBest[k] ;

	//for( int k = 0 ; k < sut->parameter ; k++ )
	//	cout<<gBest[k]<<" " ;
	//cout<<endl;

	delete[] gBest ;
	for( vector<DParticle>::iterator j = T.begin() ; j != T.end() ; j++ )
		j->clear();
	T.clear();

	return best ;
}