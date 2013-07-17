/*
** C_CLPSO.cpp : Conventional CLPSO
*/
#include"C_CLPSO.h"

// ----------------------------------------------------------------------------
// initialization
// ----------------------------------------------------------------------------
void C_CLPSO::SetConfig( int par , int ite , double w , double f )
{
	config.population = par ;
	config.iteration = ite ;
	config.weight = w ;
	config.factor = f ;
}

// ----------------------------------------------------------------------------
// generate a test case
// ----------------------------------------------------------------------------
int* C_CLPSO::Evolve()
{
	// return test case
	int *best = new int[sut->parameter] ;  
	
	vector<Particle> T ; 	               // swarm
	int *gBest = new int[sut->parameter];  // the best position of the swarm
	int fitbest = 0 ;

	// initialize
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

	// iteration count
	int it = 1 ;

	// PC[i]
	double *Pc = new double[config.population] ;
	for( int k = 0 ; k < config.population ; k++ )
		Pc[k] = 0.05 + 0.45 * ((exp((double)(10*k)/(double)(config.population-1)) - 1) / (exp((double)10) - 1));

	// iterations
	while( true )
	{
		// compute fitness value for each particle£¬then update pbest£¬gbest
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

			// if fitness(t) = coverMax, return
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

			// update pBest
			if ( fit > (*i).fitness_pbest )
				(*i).Setpbest( fit );
			
			// update gBest
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
		}  // end for

		// break condition
		if ( it >= config.iteration )
			break ;

		// update the swarm
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