/*
** DPSO
*/
#include"PSO_DPSO.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// 设置及初始化
// ----------------------------------------------------------------------------
void PSO_DPSO::SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 )
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
int* PSO_DPSO::Evolve()
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

	// inertia 0.9 -> 0.4
	bool adap_inertia = false ;
	if ( config.weight == -1 )
	{
		adap_inertia = true ;
		config.weight = 0.9 ;
	}

	// 生成一个测试用例，gBest
	while( true )
	{
		/*
		outfile<<"【round "<<it<<"】"<<endl;
		vector<DParticle>::const_iterator xx = T.begin() + 10 ;
		for( int k=0 ; k<sut->parameter; k++ )
			outfile<<(*xx).position[k]<<" " ;
		outfile<<endl;
		for(map<PosVal,double>::const_iterator y=(*xx).velocity.begin() ; y!=(*xx).velocity.end() ; y++ )
			outfile<<"("<<y->first.posI<<" , "<<y->first.valI<<") , "<<y->second<<endl;
		
		outfile<<"pbest : ";
		for( int k=0 ; k<sut->parameter; k++ )
			outfile<<(*xx).pbest[k]<<" " ;
		outfile<<endl;
		outfile<<"gbest : ";
		for( int k=0 ; k<sut->parameter; k++ )
			outfile<<gBest[k]<<" ";
		outfile<<endl;
		outfile<<endl;
		*/

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
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ )  
		{
			i->velocityUpdate( config.weight , config.factor , config.factor, pro1_threshold , gBest );
			i->positionUpdate( pro2_threshold , pro3_threshold );
		} 
		
		// iteration++
		it++ ;

		// inertia 0.9 -> 0.4
		if( adap_inertia )
			config.weight = 0.9 - 0.5 * ( (double)it / (double)config.iteration );

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

/*
double PSO_DPSO::HammingDist( const int *test )
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
}*/