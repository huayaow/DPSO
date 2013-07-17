#include"DParticle.h"

void DParticle::clear()
{
	delete[] position ;
	velocity.clear();
	TempV.clear();
	delete[] pbest ;
	range = NULL ;
	delete[] ValMax ;
}

// ----------------------------------------------------------------------------
// n - parameter , m - tway , t - num 
// ----------------------------------------------------------------------------
// compute C(n,m)
int DParticle::cal_combine( int n , int m )   
{
	int ret = 1 ;
	int p = n ;
	for( int x=1 ; x<=m ; x++ , p-- )
	{
		ret = ret * p ;
		ret = ret / x ;
	}
	return ret ;
}
// 在C(n,m)中，依据具体位置pso返回序号
int DParticle::cal_pos2num( const int *pos  )    
{
	int ret = cal_combine( dimension , strength ) ;
	for( int i=0 ; i<strength ; i++ )
		ret -= cal_combine( dimension-pos[i]-1 , strength-i );
	ret--;
	return ret ;                
}
// 在C(n,m)中，依据序号返回具体位置pso
int* DParticle::cal_num2pos( int t  )  
{
	int* pos = new int[strength] ;
	t++;                          // 输入+1
	int j=1 , k ;
	for( int i=0 ; i<strength ; pos[i++]=j++ )
		for( ; t>( k = cal_combine( dimension-j , strength-i-1 ) ) ; t-=k , j++ );
	for( int p=0 ; p<strength ; p++ )    // 输出-1
		pos[p]--;
	return pos ;
}

// 在pos位置，依据val返回序号
int DParticle::cal_val2num( const int *val , const int *pos )
{
	int ret = 0 ;
	int com = 1 ;
	for( int k=strength-1 ; k>=0 ; k-- )
	{
		ret += com * val[k] ;
		com = com * range[pos[k]] ;
	}
	return ret ;
}
// 在pos位置，依据序号返回val
int* DParticle::cal_num2val(  int t , const int *pos )
{
	int *v = new int[strength] ;
	int com = 1 ;
	for( int k=1 ; k<strength ; k++ )
		com = com * range[pos[k]] ;

	for( int k=0 ; k<strength-1 ; k++ )
	{
		v[k] = t / com ;
		t -= v[k] * com ;
		com = com / range[pos[k+1]] ;
	}
	v[strength-1] = t ;
	return v ;
}


// ----------------------------------------------------------------------------
// 随机初始化，fitness初始为0
// dim = 维度，range = 每个维度取值，strength = 覆盖强度
// ----------------------------------------------------------------------------
void DParticle::RandomInit()
{
	// position
	//position = new int[dimension] ;
	for( int i = 0 ; i < dimension ; i++ )
		position[i] = (int)( ((double)(rand()%1000)/1000.0) * range[i] ) ;
	
	// velocity
	map<PosVal,double>::iterator iter ;
	for( int i = 0 ; i < PosMax ; i++ )
	{
		PosVal pv ;
		pv.posI = (int)( ((double)(rand()%1000)/1000.0) * PosMax ) ;            // random posI
		pv.valI = (int)( ((double)(rand()%1000)/1000.0) * ValMax[pv.posI] ) ;   // random valI
		
		// insert
		iter = velocity.find( pv );
		if( iter != velocity.end() )
		{
			iter->second += (double)(rand()%1000)/1000.0 ;
			if( iter->second > 1.0 )
				iter->second = 1.0 ;
		}
		else
			//velocity.insert( pair<PosVal,double>(pv, 1 - 0.5 * (double)(rand()%1000)/1000.0) );
		    velocity.insert( pair<PosVal,double>(pv, (double)(rand()%1000)/1000.0) );
	}
	fitness_now = 0 ;

	//for( iter = velocity.begin() ; iter != velocity.end() ; iter++ )
	//	cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;

	//pbest= new int[dimension] ;
	for( int i = 0 ; i < dimension ; i++ )
		pbest[i] = position[i] ;
	fitness_pbest = 0 ;
}

// ----------------------------------------------------------------------------
// 更新历史最优
// ----------------------------------------------------------------------------
void DParticle::Setpbest( int fit )
{
	for( int i = 0 ; i < dimension ; i++ )
		pbest[i] = position[i] ;
	fitness_pbest = fit ;
}

// ----------------------------------------------------------------------------
// velocity
// 参数：weight , factor , threshold
// ----------------------------------------------------------------------------
void DParticle::velocityUpdate( double weight , double factor1 , double factor2 , double prob1 , const int* gbest )
{
	// weight * velocity
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; x++ )
		x->second = x->second * weight ;
	/*
	cout<<"term1"<<endl;
	for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
		cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
	cout<<"-------------------"<<endl;
	*/
	// pbest and gbest
	for( int x = 0 ; x < PosMax ; x++ )    // for each parameter combination
	{
		int *pos = cal_num2pos(x) ;
		int *val = new int[strength] ;

		//
		// construct pbest schema
		//
		int pcount = 0 ;
		for( int c=0 ; c<strength ; c++ )
		{
			if( pbest[pos[c]] == position[pos[c]] )
				pcount++ ;
		}
		// this schema is in pbest, but not in position
		if( pcount != strength )
		{
			for( int k=0 ; k<strength ; k++ )
				val[k] = pbest[pos[k]] ;

			PosVal pv ;
			pv.posI = x ;
			pv.valI = cal_val2num(val,pos);

			// search in velocity
			map<PosVal,double>::iterator iter ;
			iter = velocity.find( pv );
			if( iter != velocity.end() )   // this schema has already in velocity
			{
				iter->second += factor1 * ((double)(rand()%1000)/2000.0) ;
				//if( iter->second > 1.0 )
				//	iter->second = 1.0 ;
			}
			else  // insert new schema
				velocity.insert(pair<PosVal,double>(pv, factor1 * ((double)(rand()%1000)/2000.0)) );
		}

		//
		// construct gbest schema
		//
		int gcount = 0 ;
		for( int c=0 ; c<strength ; c++ )
		{
			if( gbest[pos[c]] == position[pos[c]] )
				gcount++ ;
		}
		if( gcount != strength )
		{
			for( int k=0 ; k<strength ; k++ )
				val[k] = gbest[pos[k]] ;

			PosVal gv ;
			gv.posI = x ;
			gv.valI = cal_val2num(val,pos);

			// search in velocity
			map<PosVal,double>::iterator iter ;
			iter = velocity.find( gv );
			if( iter != velocity.end() )   // this schema has already in velocity
			{
				iter->second += factor2 * ((double)(rand()%1000)/2000.0) ;
				//if( iter->second > 1.0 )
				//	iter->second = 1.0 ;
			}
			else  // insert new schema
				velocity.insert(pair<PosVal,double>(gv, factor2 * ((double)(rand()%1000)/2000.0)) );
		}

		delete[] pos;
		delete[] val ;
	}
	/*
	cout<<"term2"<<endl;
	for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
		cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
	cout<<"-------------------"<<endl;
	*/
	// final cut
	// only the prob which larger than threshold can be kept
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; )
	{
		if( x->second < prob1 )
			x = velocity.erase(x) ;
		else
		{
			if( x->second > 1.0 )
				x->second = 1.0 ;
			x++ ;
		}
	}
	/*
	cout<<"final"<<endl;
	for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
		cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
	cout<<"-------------------"<<endl;
	*/

	// sort
	TempV.clear();
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; x++ )
		TempV.insert( pair<double,PosVal>(x->second, x->first) );

	/*
	cout<<"TempV"<<endl;
	for( set<pair<double,PosVal>>::iterator iter = TempV.begin() ; iter != TempV.end() ; iter++ )
		cout<<iter->first<<" , ("<<iter->second.posI<<", "<<iter->second.valI<<")"<<endl;
	cout<<"-------------------"<<endl;
	*/
}


// ----------------------------------------------------------------------------
// position
// 参数：prob
// ----------------------------------------------------------------------------
void DParticle::positionUpdate( double prob2, double prob3 )
{
	//
	// convergence ?
	//
	if( velocity.size() == 0 )
	{
		RandomInit();
		return ;
	}

	int *np = new int[dimension] ;
	for( int k=0 ; k<dimension ; k++ )
		np[k] = -1 ;

	// update
	int parcount = dimension ;
	for( set<pair<double,PosVal>>::reverse_iterator iter = TempV.rbegin() ; iter != TempV.rend() ; iter++ )
	{
		if( (double)(rand()%1000)/1000.0 < iter->first )
		{
			int *pos = cal_num2pos( iter->second.posI );
		    int *val = cal_num2val( iter->second.valI , pos );

			for( int k=0 ; k<strength ; k++ )
			{
				if( np[pos[k]] == -1 && (double)(rand()%1000)/1000.0 < prob2 )
				{
					np[pos[k]] = val[k] ;
					parcount-- ;
				}
			}
			delete[] pos ;
			delete[] val ;
		}
		if( parcount == 0 )
			break ;
	}

	// new position
	for( int k=0 ; k<dimension ; k++ )
	{
		if( np[k] != -1 )
			position[k] = np[k] ;
	}
	delete[] np ;
	//delete[] token ;

	// random change
	if( (double)(rand()%1000)/1000.0 < prob3 )
	{
		int pa = (int)(((double)(rand()%1000)/1000.0)*dimension) ;
		int va = (int)(((double)(rand()%1000)/1000.0)*range[pa]) ;
		position[pa] = va ;
	}
	
}

// ----------------------------------------------------------------------------
// velocity，用于CLPSO
// 参数：weight , factor , threshold
// ----------------------------------------------------------------------------
void DParticle::velocityUpdate_Comprehensive( double weight , double factor, double prob1, const int* clbest)
{
	// weight * velocity
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; x++ )
		x->second = x->second * weight ;

	// clbest
	for( int x = 0 ; x < PosMax ; x++ )    // for each parameter combination
	{
		int *pos = cal_num2pos(x) ;
		int *val = new int[strength] ;

		//
		// construct clbest schema
		//
		int gcount = 0 ;
		for( int c=0 ; c<strength ; c++ )
		{
			if( clbest[pos[c]] == position[pos[c]] )
				gcount++ ;
		}
		if( gcount != strength )
		{
			for( int k=0 ; k<strength ; k++ )
				val[k] = clbest[pos[k]] ;

			PosVal gv ;
			gv.posI = x ;
			gv.valI = cal_val2num(val,pos);

			// search in velocity
			map<PosVal,double>::iterator iter ;
			iter = velocity.find( gv );
			if( iter != velocity.end() )   // this schema has already in velocity
				iter->second += factor * ((double)(rand()%1000)/2000.0) ;
			else  // insert new schema
				velocity.insert(pair<PosVal,double>(gv, factor * ((double)(rand()%1000)/2000.0)) );
		}

		delete[] pos;
		delete[] val ;
	}
	
	// final cut
	// only the prob which larger than threshold can be kept
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; )
	{
		if( x->second < prob1 )
			x = velocity.erase(x) ;
		else
		{
			if( x->second > 1.0 )
				x->second = 1.0 ;
			x++ ;
		}
	}

	// sort
	TempV.clear();
	for( map<PosVal,double>::iterator x = velocity.begin() ; x != velocity.end() ; x++ )
		TempV.insert( pair<double,PosVal>(x->second, x->first) );
}