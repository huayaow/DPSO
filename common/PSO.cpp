/*
** Base PSO
*/
#include"PSO.h"

// ----------------------------------------------------------------------------
// clear
// ----------------------------------------------------------------------------
void PSO::clear_PSOResult()
{
	for( vector<int*>::iterator i = PSO_Result.begin() ; i != PSO_Result.end() ; i++ )
		delete[] (*i) ;
	PSO_Result.clear();
}

// ----------------------------------------------------------------------------
// copy A to B with the number of parameter parsize
// ----------------------------------------------------------------------------
void PSO::copy( vector<int*> A , vector<int*> &B , int prasize )
{
	if( B.size() != 0 )
	{
		for( vector<int*>::iterator i = B.begin() ; i != B.end() ; i++ )
			delete[] (*i) ;
		B.clear();
	}
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
// covering array generation
// ----------------------------------------------------------------------------
void PSO::PSOEvolve()
{
	PSO_SIZE = MAX ;
	PSO_TIME = MAX ;

	clear_PSOResult(); 

	// initialize
	sut->GenerateS();
	time_t nowtime1 = time( 0 );

	// One-test-at-a-time
	while( sut->SCount != 0 )
	{
		int *best = Evolve();
		PSO_Result.push_back( best );
		int cov = sut->FitnessValue( best , 1 );
	}

	time_t nowtime2 = time(0);
	
	PSO_SIZE = PSO_Result.size();
	PSO_TIME = nowtime2 - nowtime1 ;

	// if there has seed ...
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

// ----------------------------------------------------------------------------
// output this->PSO_Result to file
// ----------------------------------------------------------------------------
void PSO::writeThisResult(string filename, string info)
{
	ofstream file;
	file.open(filename);

	if (file.is_open())
	{
		// details of sut
		file << "PARAMETER " << this->sut->parameter << endl;
		file << "VALUE ";
		for (int k = 0; k < this->sut->parameter; k++)
			file << this->sut->value[k] << " ";
		file << endl;
		file << "TWAY " << this->sut->tway << endl;

		// detail of info
		file << "ALGORITHM " << info.c_str() << endl;

		// ca
		file << "SIZE " << PSO_SIZE << endl;
		file << "TIME " << PSO_TIME << endl;
		for (vector<int*>::iterator i = PSO_Result.begin(); i != PSO_Result.end(); i++)
		{
			for (int v = 0; v < this->sut->parameter; v++)
				file << (*i)[v] << " ";
			file << endl;
		}
		file << "END" << endl;
		file.close();
	}
	
}