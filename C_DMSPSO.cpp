/*
** C_DMSPSO.cpp : Conventional DMS-PSO
*/
#include"C_DMSPSO.h"

void C_DMSPSO::SetConfig( int par , int ite , double w , double f )
{
	config.population = par ;
	config.iteration = ite ;
	config.weight = w ;
	config.factor = f ;
}

int* C_DMSPSO::Evolve()
{
	double inertia = 0.9 ;
	double inertia_max = 0.9 ;
	double inertia_min = 0.4 ;

	int *best = new int[sut->parameter] ;  
	
	vector<Particle> T ; 	               
	int *gBest = new int[sut->parameter];  
	int fitbest = 0 ;

	for( int i = 0 ; i < config.population ; i++ )
	{
		Particle a( sut->parameter , sut->value ) ;
		a.RandomInit();

		T.push_back(a);
	}

	vector<Particle>::iterator x = T.begin();     
	for( int c = 0 ; c < sut->parameter ; c++)
		gBest[c] = (*x).position[c] ;

	int it = 1 ;

	// DMS
	int M = 4 ; // the number of particles of each group
	int R = 5 ;
	int SS = config.population/M ;  // the number of groups
	int *Swarms = new int[config.population] ;
	int *Swarms_num = new int[SS] ;

	int **Swarms_localpos = new int*[SS] ;
	for( int k=0; k<SS; k++ )
		Swarms_localpos[k] = new int[sut->parameter] ;

	int *Swarms_localbest = new int[SS] ;
	for( int k=0; k<SS; k++ )
		Swarms_localbest[k] = -1 ;

	// random divide population
	for( int k=0 ; k<SS ; k++ )
		Swarms_num[k] = 0 ;

	for( int k=0 ; k<config.population; k++ )
	{
		int a = (int)( (double)(rand()%1000)/1000.0 * SS );
		while( Swarms_num[a] == M )
			a = (int)( (double)(rand()%1000)/1000.0 * SS );

		Swarms[k] = a ;
		Swarms_num[a]++ ;
	}


	while( true )
	{
		int index = 0 ;
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

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

			if ( fit > (*i).fitness_pbest )
				(*i).Setpbest( fit );
			
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}

			// update local best
			for( int k=0 ; k<config.population; k++ )
			{
				if( Swarms[k] == Swarms[index] && fit > Swarms_localbest[Swarms[k]] )
				{
					Swarms_localbest[Swarms[k]] = fit ;
					for( int c = 0 ; c< sut->parameter ; c++)
						Swarms_localpos[Swarms[k]][c] = (*i).position[c] ;
				}
			}
		}

		if ( it >= (int)((double)config.iteration * 0.9) )
			break ;

		index = 0 ;
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )  
		{
			(*i).velocityUpdate( config.weight , config.factor , Swarms_localpos[Swarms[index]] );
			(*i).positionUpdate();
		} 

		it++ ;

		// inertia 0.9->0.4
		inertia = inertia_max - ( inertia_max - inertia_min ) * ( (double)it / (double)config.iteration );

		// re divided swarm
		if( it % R == 0 )
		{
			// random divide population
			for( int k=0 ; k<SS ; k++ )
				Swarms_num[k] = 0 ;

			for( int k=0 ; k<config.population; k++ )
			{
				int a = (int)( (double)(rand()%1000)/1000.0 * SS );
				while( Swarms_num[a] == M )
					a = (int)( (double)(rand()%1000)/1000.0 * SS );
				Swarms[k] = a ;
				Swarms_num[a]++ ;
			}

			for( int k=0; k<SS; k++ )
				Swarms_localbest[k] = 0 ;
		}

	}  // end 0.9 while

	// last 0.1
	while( true )
	{
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

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

			if ( fit > (*i).fitness_pbest )
				(*i).Setpbest( fit );
			
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
		}

		if ( it >= config.iteration )
			break ;

		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )  
		{
			(*i).velocityUpdate( config.weight , config.factor , gBest );
			(*i).positionUpdate();
		} 

		it++ ;

		// inertia 0.9->0.4
		inertia = inertia_max - ( inertia_max - inertia_min ) * ( (double)it / (double)config.iteration );

	}  // end 0.1 while

	for( int k = 0 ; k < sut->parameter ; k++ ) 
		best[k] = gBest[k] ;

	delete[] gBest ;
	for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		i->clear();
	T.clear();

	delete[] Swarms ;
	delete[] Swarms_num ;

	for( int k=0; k<SS; k++ )
		delete[] Swarms_localpos[k] ;
	delete[] Swarms_localpos ;
	
	return best ;
}