/*
** D-APSO
*/
#include"PSO_DAPSO.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// 设置及初始化
// ----------------------------------------------------------------------------
void PSO_DAPSO::SetConfig( int par , int ite , double p1 , double p2 , double p3 )
{
	config.population = par ;
	config.iteration = ite ;
	//config.weight = w ;
	//config.factor = f ;

	pro1_threshold = p1 ;
	pro2_threshold = p2 ;
	pro3_threshold = p3 ;
}


// ----------------------------------------------------------------------------
// 生成一条测试用例
// ----------------------------------------------------------------------------
int* PSO_DAPSO::Evolve()
{
	/*
	ofstream outfile("result.txt",ios::app);
	outfile<<"-------------------------------------"<<endl;
	outfile<<"Evolve"<<endl;
	outfile<<"-------------------------------------"<<endl;
	*/
	double inertia = 0.9 ;
	double factor1 = 1.3 ;
	double factor2 = 1.3 ;
	double factor_max = 1.8 ;
	double factor_min = 0.8 ;

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

	// adaptive
	int state = 1 ;
	double f_value = 0 ;
	double sigma_max = 1.0 ;
	double sigma_min = 0.1 ;

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

		// adaptive parameter
		f_value = FCalculate( T , gBest ) ;
		state = FuzzyDicsion( f_value , state );
		
		inertia = 1 / ( 1 + 1.5 * exp( -2.6 * f_value ) ) ;

		if( state == 1 )
		{
			factor1 = factor1 + ( 0.05 + (double)(rand()%50)/1000.0 );
			factor2 = factor2 - ( 0.05 + (double)(rand()%50)/1000.0 );
		}
		if( state == 2 )
		{
			factor1 = factor1 + 0.5 * ( 0.05 + (double)(rand()%50)/1000.0 );
			factor2 = factor2 - 0.5 * ( 0.05 + (double)(rand()%50)/1000.0 );
		}
		if( state == 3 )
		{
			factor1 = factor1 + 0.5 * ( 0.05 + (double)(rand()%50)/1000.0 );
			factor2 = factor2 + 0.5 * ( 0.05 + (double)(rand()%50)/1000.0 );
		}
		if( state == 4 )
		{
			factor1 = factor1 - ( 0.05 + (double)(rand()%50)/1000.0 );
			factor2 = factor2 + ( 0.05 + (double)(rand()%50)/1000.0 );
		}

		if( factor1 > factor_max )
			factor1 = factor_max ;
		if( factor1 < factor_min )
			factor1 = factor_min ;

		if( factor2 > factor_max )
			factor2 = factor_max ;
		if( factor2 < factor_min )
			factor2 = factor_min ;

		// 更新粒子群
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ )  
		{
			i->velocityUpdate( inertia, factor1, factor2, pro1_threshold , gBest );
			i->positionUpdate( pro2_threshold , pro3_threshold );
		} 
		
		// ELS
		int *gbest_tmp = new int[sut->parameter];
		for( int k=0 ; k<sut->parameter ; k++ )
			gbest_tmp[k] = gBest[k] ;

		int dim = (int)( ((double)(rand()%1000)/1000.0) * sut->parameter ); 
		gbest_tmp[dim] = gbest_tmp[dim] + (int)((double)( sut->value[dim] - 1 ) * 
			Gaussrand(0, pow(sigma_max-(sigma_max-sigma_min)*(it/config.iteration),2) ));
		if( gbest_tmp[dim] >= sut->value[dim] )
			gbest_tmp[dim] = sut->value[dim] - 1 ;
		if( gbest_tmp[dim] < 0 )
			gbest_tmp[dim] = 0 ;

		int fit_tmp = sut->FitnessValue(gbest_tmp,0) ;
		if( fit_tmp > fitbest )
		{
			fitbest = fit_tmp ;
			for( int c = 0 ; c < sut->parameter ; c++)
				gBest[c] = gbest_tmp[c] ;
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



// f = ( d[g] - d[min] ) / ( d[max] - d[min] ), where d[i] = the mean distance of particle i
double PSO_DAPSO::FCalculate( vector<DParticle> T, int* gbest )
{
	double min_dis = (double)MAX ;
	double max_dis = 0 ;
	double g_dis = 0 ;

	// for each particle
	for( vector<DParticle>::const_iterator i = T.begin(); i != T.end(); i++ )
	{
		double alltmp = 0 ;
		for( vector<DParticle>::const_iterator j = T.begin(); j != T.end() && j != i ; j++ )
		{
			double tmp = 0 ;
			for( int k = 0 ; k < sut->parameter ; k++ )
				tmp += pow( (double)i->position[k] - (double)j->position[k] , 2) ;
			alltmp += sqrt(tmp) ;
		}
		alltmp = alltmp / (double)(T.size()-1) ;
		
		if( alltmp < min_dis )
			min_dis = alltmp ;

		if( alltmp > max_dis )
			max_dis = alltmp ;
	}

	// for gbest
	for( vector<DParticle>::const_iterator i = T.begin(); i != T.end(); i++ )
	{
		double tmp = 0 ;
		for( int k = 0 ; k < sut->parameter ; k++ )
			tmp += pow( (double)i->position[k] - (double)gbest[k] , 2) ;
		g_dis += sqrt(tmp) ;
		g_dis = g_dis / (double)(T.size()-1) ;
	}

	// f
	return (g_dis - min_dis) / (max_dis - min_dis) ;
}


// S1 = exploration
// S2 = exploitation
// S3 = convergence
// S4 = jumping-out
int PSO_DAPSO::FuzzyDicsion( double f , int previous)
{
	if( f <= 0.2 )
		return 3 ;

	if( 0.2 < f && f < 0.3 )
	{
		// s3 > s2
		if( f < (double)7/(double)30 )
		{
			if( previous == 2 || previous == 1 )
				return 2 ;
			else
				return 3 ;
		}
		// s3 <= s2
		else
		{
			if( previous == 3 )
				return 3 ;
			else
				return 2 ;
		}
	}

	if( 0.3 <= f && f <= 0.4 )
		return 2 ;

	if( 0.4 < f && f < 0.6 )
	{
		// s2 > s1
		if( f < 0.5 )
		{
			if( previous == 1 || previous == 4 )
				return 1 ;
			else
				return 2 ;
		}
		// s2 <= s1
		else
		{
			if( previous == 2 )
				return 2 ;
			else
				return 1 ;
		}
	}

	if( 0.6 <= f && f <= 0.7 )
		return 1 ;

	if( 0.7 < f && f < 0.8 )
	{
		// s1 > s4
		if( f < (double)23/(double)30 )
		{
			if( previous == 4 || previous == 3 )
				return 4 ;
			else
				return 1 ;
		}
		// s1 <= s4
		else
		{
			if( previous == 1 )
				return 1 ;
			else
				return 4 ;
		}
	}

	if( f >= 0.8 )
		return 4 ;

	return -1 ;

	/*
	// s1
	if( f <= 0.4 || f > 0.8 )
		s[0] = 0 ;
	else if( 0.4 < f && f <= 0.6 )
		s[0] = 5 * f - 2 ;
	else if( 0.6 < f && f <= 0.7 )
		s[0] = 1 ;
	else
		s[0] = -10 * f + 8 ;

	// s2
	if( f <= 0.2 || f > 0.6 )
		s[1] = 0 ;
	else if( 0.2 < f && f <= 0.3 )
		s[1] = 10 * f - 2 ;
	else if( 0.3 < f && f <= 0.4 )
		s[1] = 1 ;
	else
		s[1] = -5 * f + 3 ;

	// s3
	if( f <= 0.1 )
		s[2] = 1 ;
	else if( 0.1 < f && f <= 0.3 )
		s[2] = -5 * f + 1.5 ;
	else
		s[2] = 0 ;

	// s4
	if( f <= 0.7 )
		s[3] = 0 ;
	else if( 0.7 < f && f <= 0.9 )
		s[3] = 5 * f - 3.5 ;
	else
		s[3] = 1 ;
	*/
}

// Gaussian Distribution
// Expectation - E , Variance - V
double PSO_DAPSO::Gaussrand( double E , double V )
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
     
    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
             
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
         
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
         
    phase = 1 - phase;

	X = X * V + E;
    return X;
}