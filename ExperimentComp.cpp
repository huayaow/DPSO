#include"ExperimentComp.h"

void ExperimentComp::ClearCA()
{
	for( vector<CoveringArray>::iterator i = CA.begin() ; i != CA.end() ; i++ )
	{
		delete[] i->val ;
		if( i->sub.size() != 0 )
		{
			for( vector<SUBINFO>::iterator j = i->sub.begin() ; j != i->sub.end() ; j++ )
				delete[] j->subposition ;
		}
	}
}

// CPSO and DPSO
void ExperimentComp::CompareStudy()
{
	srand( time(0) ) ;
	InitialCA_1();

	ofstream outfile("CompareStudy_1.txt");
	ofstream outfile_t("CompareStudy_1_time.txt");

	outfile<<"CPSO default parameter"<<endl;
	outfile<<"iteration = "<<config_cpso.iteration<<", "
		<<"population = "<<config_cpso.population<<", "
		<<"weight = "<<config_cpso.weight<<", "
		<<"factor = "<<config_cpso.factor<<endl;
	outfile<<"DPSO default population"<<endl;
	outfile<<"iteration = "<<config_dpso.iteration<<", "
		<<"population = "<<config_dpso.population<<", "
		<<"weight = "<<config_dpso.weight<<", "
		<<"factor = "<<config_dpso.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<endl;

	// for each CA
	int index = 1 ;
	for( vector<CoveringArray>::iterator i = CA.begin() ; i != CA.end() ; i++ , index++ )
	{
		cout<<"----------------------------------"<<endl;
		cout<<"CA "<<index<<endl;

		SUT* sut = new SUT( i->par , i->val , i->tway );

		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<"parameter = "<<sut->parameter<<" , value = ";
		for( int k=0 ; k<sut->parameter ; k++ )
			outfile<<sut->value[k]<<" ";
		outfile<<", tway = "<<sut->tway<<endl;

		if( i->sub.size() != 0 )
		{
			sut->SetSub( i->sub );
			outfile<<"Subinfo:"<<endl;
			for( vector<SUBINFO>::iterator j = i->sub.begin() ; j != i->sub.end() ; j++ )
			{
				outfile<<"par: ";
				for( int k=0 ; k<j->subparameter ; k++ )
					outfile<<j->subposition[k]<<" " ;
				outfile<<" , sub-way: "<<j->subway<<endl;
			}
			outfile<<endl;
		}

		int meansize = 0 ;
		int meantime = 0 ;
		int bestsize = MAX ;
		int worstsize = 0 ;
		double size , time ;
		PSO *pso ;

		outfile<<"------------CPSO------------"<<endl;
		pso = new PSO_CPSO( sut );
		pso->SetConfig( config_cpso.population , config_cpso.iteration , config_cpso.weight , config_cpso.factor );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------DPSO------------"<<endl;
		pso = new PSO_DPSO( sut );
		pso->SetConfig( config_dpso.population , config_dpso.iteration , config_dpso.weight , config_dpso.factor , 
			pro1_default , pro2_default, pro3_default );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<endl;

		outfile_t<<"--------------------------------------------------------------------------"<<endl;
	}

	outfile.close();
	outfile_t.close();
}


// Discrete CLSPO, TVAC, DMS-PSO, APSO
void ExperimentComp::CompareStudy_d()
{
	srand( time(0) ) ;
	InitialCA_1();

	ofstream outfile("CompareStudy_Var_1.txt");
	ofstream outfile_t("CompareStudy_Var_1_time.txt");

	outfile<<"CLSPO, TVAC, DMS-PSO, APSO"<<endl;
	outfile<<"default parameter"<<endl;
	outfile<<"iteration = "<<config_dpso.iteration<<", "
		<<"population = "<<config_dpso.population<<", "
		<<"weight = "<<config_dpso.weight<<", "
		<<"factor = "<<config_dpso.factor<<", "
		<<"pro1 = "<<pro1_default<<", "
		<<"pro2 = "<<pro2_default<<", "
		<<"pro3 = "<<pro3_default<<endl;
	outfile<<endl;

	// for each CA
	int index = 1 ;
	for( vector<CoveringArray>::iterator i = CA.begin() ; i != CA.end() ; i++ , index++ )
	{
		cout<<"----------------------------------"<<endl;
		cout<<"CA "<<index<<endl;

		SUT* sut = new SUT( i->par , i->val , i->tway );

		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<"parameter = "<<sut->parameter<<" , value = ";
		for( int k=0 ; k<sut->parameter ; k++ )
			outfile<<sut->value[k]<<" ";
		outfile<<", tway = "<<sut->tway<<endl;

		if( i->sub.size() != 0 )
		{
			sut->SetSub( i->sub );
			outfile<<"Subinfo:"<<endl;
			for( vector<SUBINFO>::iterator j = i->sub.begin() ; j != i->sub.end() ; j++ )
			{
				outfile<<"par: ";
				for( int k=0 ; k<j->subparameter ; k++ )
					outfile<<j->subposition[k]<<" " ;
				outfile<<" , sub-way: "<<j->subway<<endl;
			}
			outfile<<endl;
		}

		int meansize = 0 ;
		int meantime = 0 ;
		int bestsize = MAX ;
		int worstsize = 0 ;
		double size , time ;
		PSO *pso ;

		outfile<<"------------D-CLPSO------------"<<endl;
		pso = new D_CLPSO( sut );
		pso->SetConfig( config_dpso.population , config_dpso.iteration , config_dpso.weight , config_dpso.factor , 
			pro1_default , pro2_default, pro3_default );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------D-TVAC------------"<<endl;
		pso = new D_TVAC( sut );
		pso->SetConfig( config_dpso.population , config_dpso.iteration , pro1_default , pro2_default, pro3_default );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------D-DMS-PSO------------"<<endl;
		pso = new D_DMSPSO( sut );
		pso->SetConfig( config_dpso.population , config_dpso.iteration , config_dpso.weight , config_dpso.factor ,
			pro1_default , pro2_default, pro3_default );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------D-APSO------------"<<endl;
		pso = new D_APSO( sut );
		pso->SetConfig( config_dpso.population , config_dpso.iteration , pro1_default , pro2_default, pro3_default );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<endl;

		outfile_t<<"--------------------------------------------------------------------------"<<endl;
	}

	outfile.close();
	outfile_t.close();
}

// conventional CLSPO, TVAC, DMS-PSO, APSO
void ExperimentComp::CompareStudy_c()
{
	srand( time(0) ) ;
	InitialCA_1();

	ofstream outfile("CompareStudy_Var_Con_1.txt");
	ofstream outfile_t("CompareStudy_Var_Con_1_time.txt");

	outfile<<"TVAC, CLPSO, APSO, DMS-PSO"<<endl;
	outfile<<"default parameter"<<endl;
	outfile<<"iteration = "<<config_cpso.iteration<<", "
		<<"population = "<<config_cpso.population<<", "
		<<"weight = "<<config_cpso.weight<<", "
		<<"factor = "<<config_cpso.factor<<endl;
	outfile<<endl;

	// for each CA
	int index = 1 ;
	for( vector<CoveringArray>::iterator i = CA.begin() ; i != CA.end() ; i++ , index++ )
	{
		cout<<"----------------------------------"<<endl;
		cout<<"CA "<<index<<endl;

		SUT* sut = new SUT( i->par , i->val , i->tway );

		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<"parameter = "<<sut->parameter<<" , value = ";
		for( int k=0 ; k<sut->parameter ; k++ )
			outfile<<sut->value[k]<<" ";
		outfile<<", tway = "<<sut->tway<<endl;

		if( i->sub.size() != 0 )
		{
			sut->SetSub( i->sub );
			outfile<<"Subinfo:"<<endl;
			for( vector<SUBINFO>::iterator j = i->sub.begin() ; j != i->sub.end() ; j++ )
			{
				outfile<<"par: ";
				for( int k=0 ; k<j->subparameter ; k++ )
					outfile<<j->subposition[k]<<" " ;
				outfile<<" , sub-way: "<<j->subway<<endl;
			}
			outfile<<endl;
		}

		int meansize = 0 ;
		int meantime = 0 ;
		int bestsize = MAX ;
		int worstsize = 0 ;
		double size , time ;
		PSO *pso ;

		outfile<<"------------TVAC------------"<<endl;
		pso = new C_TVAC( sut );
		pso->SetConfig( config_cpso.population , config_cpso.iteration );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------CLPSO------------"<<endl;
		pso = new C_CLPSO( sut );
		pso->SetConfig( config_cpso.population , config_cpso.iteration , config_cpso.weight , config_cpso.factor );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------APSO------------"<<endl;
		pso = new C_APSO( sut );
		pso->SetConfig( config_cpso.population , config_cpso.iteration );

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();


		meansize = 0 ;
		meantime = 0 ;
	    bestsize = MAX ;
		worstsize = 0 ;

		outfile<<"------------DMS-PSO------------"<<endl;
		pso = new C_DMSPSO( sut );
		pso->SetConfig( config_cpso.population , config_cpso.iteration , config_cpso.weight , config_cpso.factor);

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

		outfile<<endl;
		outfile_t<<endl;

		size = double(meansize)/(double)times ;
		time = double(meantime)/(double)times ;
		cout<<"re: "<<size<<"("<<time<<"), best: "<<bestsize<<", worst: "<<worstsize<<endl;

		outfile<<"mean size: "<<size<<", mean time: "<<time<<", best: "<<bestsize<<", worst: "<<worstsize<<endl;
		pso->~PSO();

		outfile<<"--------------------------------------------------------------------------"<<endl;
		outfile<<endl;

		outfile_t<<"--------------------------------------------------------------------------"<<endl;
	}

	outfile.close();
	outfile_t.close();
}