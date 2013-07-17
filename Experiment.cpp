#include"Experiment.h"

/* --------------------------------------------------------------
  iteration
-------------------------------------------------------------- */
void Experiment::Par_iteration( int* parvalue, int num, string alg )
{
	ofstream outfile("Par_iteration.txt");
	ofstream outfile_t("Par_iteration_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"iteration = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		pso = new PSO_CPSO( sut );
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		if( alg == "CPSO" )
			pso->SetConfig( config_default.population , parvalue[ite] , config_default.weight , config_default.factor );
		else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , parvalue[ite] , config_default.weight , config_default.factor , 
			pro1_default , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}


/* --------------------------------------------------------------
  population
-------------------------------------------------------------- */
void Experiment::Par_population( int* parvalue, int num, string alg)
{
	ofstream outfile("Par_population.txt");
	ofstream outfile_t("Par_population_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default iteration = "<<config_default.iteration<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"population = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		pso = new PSO_CPSO( sut );
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		if( alg == "CPSO" )
			pso->SetConfig( parvalue[ite] , config_default.iteration , config_default.weight , config_default.factor );
		else if( alg == "DPSO" )
			pso->SetConfig( parvalue[ite] , config_default.iteration , config_default.weight , config_default.factor , 
			pro1_default , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();

}

/* --------------------------------------------------------------
  inertia
-------------------------------------------------------------- */
void Experiment::Par_inertia( double* parvalue, int num, string alg )
{
	ofstream outfile("Par_inertia.txt");
	ofstream outfile_t("Par_inertia_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"iteration = "<<config_default.iteration<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"inertia = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		pso = new PSO_CPSO( sut );
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		if( alg == "CPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , parvalue[ite] , config_default.factor );
		else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , parvalue[ite] , config_default.factor , 
			pro1_default , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}



/* --------------------------------------------------------------
  factor
-------------------------------------------------------------- */
void Experiment::Par_factor( double* parvalue, int num, string alg )
{
	ofstream outfile("Par_factor.txt");
	ofstream outfile_t("Par_factor_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"iteration = "<<config_default.iteration<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"factor = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		pso = new PSO_CPSO( sut );
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		if( alg == "CPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , config_default.weight , parvalue[ite] );
		else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , config_default.weight , parvalue[ite] , 
			pro1_default , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}





/* --------------------------------------------------------------
  prob1 , only DPSO
-------------------------------------------------------------- */
void Experiment::Par_Prob1( double* parvalue, int num, string alg )
{
	ofstream outfile("Par_prob1.txt");
	ofstream outfile_t("Par_prob1_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"iteration = "<<config_default.iteration<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"prob1 = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		return ;
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		//if( alg == "CPSO" )
		//	return ;
		//else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , config_default.weight , config_default.factor , 
			parvalue[ite] , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}



/* --------------------------------------------------------------
  prob2 , only DPSO
-------------------------------------------------------------- */
void Experiment::Par_Prob2( double* parvalue, int num, string alg )
{
	ofstream outfile("Par_prob2.txt");
	ofstream outfile_t("Par_prob2_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"iteration = "<<config_default.iteration<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"prob2 = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		return ;
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		//if( alg == "CPSO" )
		//	return ;
		//else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , config_default.weight , config_default.factor , 
			pro1_default, parvalue[ite], pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}



/* --------------------------------------------------------------
  prob3 , only DPSO
-------------------------------------------------------------- */
void Experiment::Par_Prob3( double* parvalue, int num, string alg )
{
	ofstream outfile("Par_prob3.txt");
	ofstream outfile_t("Par_prob3_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"default population = "<<config_default.population<<", "
		<<"iteration = "<<config_default.iteration<<", "
		<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro3_default<<endl;
	outfile<<"prob3 = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		return ;
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		//if( alg == "CPSO" )
		//	return ;
		//else if( alg == "DPSO" )
			pso->SetConfig( config_default.population , config_default.iteration , config_default.weight , config_default.factor , 
			pro1_default, pro2_default, parvalue[ite] );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();
}



/* --------------------------------------------------------------
  sum = population_i(parvalue[i]) * iteration_i
-------------------------------------------------------------- */
void Experiment::Par_relations( int* parvalue, int sum, int num, string alg )
{
	int *tp_parvalue = new int[num];
	for( int c=0 ; c<num ; c++ )
	{
		tp_parvalue[c] = sum / parvalue[c] ;
		cout<<tp_parvalue[c]<<" ";
	}
	cout<<endl;

	ofstream outfile("Par_relations.txt");
	ofstream outfile_t("Par_relations_time.txt");
	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"parameter = "<<sut->parameter<<" , value = ";
	for( int k=0 ; k<sut->parameter ; k++ )
		outfile<<sut->value[k]<<" ";
	outfile<<", tway = "<<sut->tway<<endl;
	outfile<<"weight = "<<config_default.weight<<", "
		<<"factor = "<<config_default.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<"population = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"iteration = ";
	for( int c=0 ; c<num ; c++ )
		outfile<<tp_parvalue[c]<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	ParSize.clear();
	ParTime.clear();
	ParBest.clear();
	ParWorst.clear();

	srand( time(0) ) ;

	// alg
	PSO *pso ;
	if( alg == "CPSO" )
		pso = new PSO_CPSO( sut );
	else if( alg == "DPSO" )
	    pso = new PSO_DPSO( sut );
	else
	{
		cout<<"error, no such alg"<<endl;
		return ;
	}
	
	for( int ite = 0 ; ite < num ; ite++ )
	{
		if( alg == "CPSO" )
			pso->SetConfig( parvalue[ite] , tp_parvalue[ite] , config_default.weight , config_default.factor );
		else if( alg == "DPSO" )
			pso->SetConfig( parvalue[ite] , tp_parvalue[ite] , config_default.weight , config_default.factor , 
			pro1_default , pro2_default, pro3_default );

		int meansize = 0 ;
		int meantime = 0 ;

		int bestsize = MAX ;
		int worstsize = 0 ;

		outfile<<parvalue[ite]<<","<<tp_parvalue[ite]<<": ";
		outfile_t<<parvalue[ite]<<","<<tp_parvalue[ite]<<": ";

		for( int i=0 ; i<times ; i++ )
		{
			pso->PSOEvolve();
			
			cout<<pso->PSO_SIZE<<"("<<pso->PSO_TIME<<") ";
			outfile<<pso->PSO_SIZE<<" ";
			outfile_t<<pso->PSO_TIME<<" ";
			
			meansize += pso->PSO_SIZE ;
			meantime += pso->PSO_TIME ;

			if( pso->PSO_SIZE < bestsize )
				bestsize = pso->PSO_SIZE ;
			if( pso->PSO_SIZE > worstsize )
				worstsize = pso->PSO_SIZE ;
		}

		double size = double(meansize)/(double)times ;
		double time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		ParSize.push_back(size);
		ParTime.push_back(time);
		ParBest.push_back(bestsize);
		ParWorst.push_back(worstsize);

		outfile<<endl;
		outfile_t<<endl;
	}

	outfile<<"--------------------------------------------------------------------------"<<endl;
	outfile<<"mean size:"<<endl;
	for(vector<double>::const_iterator i = ParSize.begin(); i != ParSize.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"mean time:"<<endl;
	for(vector<double>::const_iterator i = ParTime.begin(); i != ParTime.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"best size:"<<endl;
	for(vector<int>::const_iterator i = ParBest.begin(); i != ParBest.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"worst size:"<<endl;
	for(vector<int>::const_iterator i = ParWorst.begin(); i != ParWorst.end(); i++ )
		outfile<<(*i)<<" ";
	outfile<<endl;
	outfile<<"--------------------------------------------------------------------------"<<endl;

	outfile.close();
	outfile_t.close();

}