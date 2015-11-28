#include"SUT.h"

#include<iostream>
#include<cmath>
using namespace std ;

// ----------------------------------------------------------------------------
//  计算函数，各变量下标从0开始
// ----------------------------------------------------------------------------
// 计算C(n,m)
int SUT::cal_combine( int n , int m )   
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
// 计算c[]在所有组合的字典序中的序号，在n个参数中选m个情况下 (n,m)
int SUT::cal_combine2num( const int *c , const int n , const int m )    
{
	int ret = cal_combine( n , m ) ;
	for( int i=0 ; i<m ; i++ )
		ret -= cal_combine( n-c[i]-1 , m-i );
	ret--;
	return ret ;                
}
// 计算字典序第t个的参数组合，结果存入c中，在n个参数中选m个情况下 (n,m)
void SUT::cal_num2combine( int *c , int t , const int n , const int m )  
{
	t++;                        // 输入+1
	int j=1 , k ;
	for( int i=0 ; i<m ; c[i++]=j++ )
		for( ; t>( k = cal_combine( n-j , m-i-1 ) ) ; t-=k , j++ )
			;
	for( int p=0 ; p<m ; p++ )   // 输出-1
		c[p]--;
}

// ----------------------------------------------------------------------------
// SUT初始化
// ----------------------------------------------------------------------------
// 子覆盖
void SUT::SetSub( const vector<SUBINFO> sub )
{
	for( vector<SUBINFO>::const_iterator i = sub.begin() ; i != sub.end() ; i++ )
	{
		SUBINFO temp ;
		temp.subparameter = (*i).subparameter ;
		temp.subway = (*i).subway ;
		temp.subposition = new int[temp.subparameter] ;
		for( int k=0 ; k<temp.subparameter ; k++ )
			temp.subposition[k] = (*i).subposition[k] ;
		subInfo.push_back(temp);
	}
	// 计算行数
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		(*i).coverIndex = coverMax ;
		(*i).coverSub = cal_combine( (*i).subparameter , (*i).subway );
		coverMax += (*i).coverSub ;
	}
	// 一条测试用例最多覆盖数
	testcaseCoverMax = coverMax ;
}

// 种子
void SUT::SetSeed( const vector<int*> seed )
{
	for( vector<int*>::const_iterator i = seed.begin() ; i != seed.end() ; i++ )
	{
		int *temp = new int[parameter] ;
		for( int k=0 ; k<parameter ; k++ )
			temp[k] = (*i)[k] ;
		seedInfo.push_back(temp);
	}
}

// ----------------------------------------------------------------------------
// 种子预处理，对所有Covered，在Evolve时再考虑加入
// ----------------------------------------------------------------------------
void SUT::preforseed()
{
	for( vector<int*>::const_iterator i = seedInfo.begin() ; i != seedInfo.end() ; i++ )
		FitnessValue((*i),1);
}


// ----------------------------------------------------------------------------
// 分配AllS空间，并生成
// ----------------------------------------------------------------------------
void SUT::GenerateS()
{
	// 初始化
	if( AllS != 0 )
	{
		for( int i=0 ; i<coverMax ; i++ )
			delete[] AllS[i];
		delete[] AllS;
		AllS = 0;
	}
	SCount = 0 ;

	// 分配coverMax行
	AllS = new MyByte*[coverMax] ;

	// 按字典序生成所有参数间的组合情况
	int *temp = new int[tway] ;      // 迭代记录
	int *temp_max = new int[tway];   // 各自最大值
	for( int k=0 ; k<tway ; k++ )  // 初始化
	{
		temp[k] = k ;
		temp_max[k] = parameter - tway + k ;
	}

	int end = tway - 1 ;
	int ptr = end ;

	int already = 0 ;
	while( already < coverMain )
	{
		//
		// 处理temp，此时temp[]标记了参数序号
		//

		// 计算涉及参数的组合个数
		int allcomb = 1 ;
		for( int p=0 ; p<tway ; p++ )
			allcomb = allcomb * value[temp[p]] ;
		// 生成新的1行
		int column = (int)ceil((double)allcomb/(double)8) ; 
		AllS[already] = new MyByte[column];
		// 初始化，全为0
		for( int k=0 ; k<column ; k++ )
			AllS[already][k].allbyte = 0x00 ;

		// 计算总的组合个数
		SCount += allcomb ;

		//
		// 求下一个组合
		//
		temp[end] = temp[end] + 1 ;  // 末位加1
		ptr = end ;
		while( ptr > 0 )
		{
			if( temp[ptr] > temp_max[ptr] )  // 超过该位允许最大值
			{
				temp[ptr-1] = temp[ptr-1] + 1 ;   // 前一位加1
				ptr-- ;
			}
			else
				break ;
		}
		if( temp[ptr] <= temp_max[ptr])  // 若该位值不是最大，后面每位在前一位基础上加1
		{
			for( int i=ptr+1 ; i<tway ; i++ ) 
				temp[i] = temp[i-1] + 1 ;
		}
		already++ ;
	}

	delete[] temp ;
	delete[] temp_max ;

	// 考虑子覆盖
	if( subInfo.size() != 0 )
		GenerateSSub();

	// 处理种子
	if( seedInfo.size() !=0 )
		preforseed();

	// 记录总的待覆盖组合数
	SCountAll = SCount ;
}

// AllS中Sub部分
void SUT::GenerateSSub()
{
	// 对每个sub
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		int subway = (*i).subway ;
		int subparameter = (*i).subparameter ;
		int *subposition = (*i).subposition ;

		// 按字典序生成所有参数间的组合情况
		int *subtemp = new int[subway] ;      // 迭代记录
		int *subtemp_max = new int[subway];   // 各自最大值
		for( int k=0 ; k<subway ; k++ )  // 初始化
		{
			subtemp[k] = k ;
			subtemp_max[k] = subparameter - subway + k ;
		}

		int subend = subway - 1 ;
		int subptr = subend ;

		int subalready = 0 ;
		int subrow = (*i).coverIndex ;
		while( subalready < (*i).coverSub )
		{
			//
			// 处理subtemp
			// subtemp[]是按[0,1,...]标号的，subtemp[]的各值代表在subposition[]中的位置
			//

			// 计算涉及参数的组合个数
			int suballcomb = 1 ;
			for( int p=0 ; p<subway ; p++ )
				suballcomb = suballcomb * value[subposition[subtemp[p]]] ;
			// 生成新的1行
			int subcolumn = (int)ceil((double)suballcomb/(double)8) ; 
			AllS[subrow] = new MyByte[subcolumn];
			// 初始化，全为0
			for( int k=0 ; k<subcolumn ; k++ )
				AllS[subrow][k].allbyte = 0x00 ;

			// 计算总的组合个数
			SCount += suballcomb ;

			//
			// 求下一个组合
			//
			subtemp[subend] = subtemp[subend] + 1 ;  // 末位加1
			subptr = subend ;
			while( subptr > 0 )
			{
				if( subtemp[subptr] > subtemp_max[subptr] )  // 超过该位允许最大值
				{
					subtemp[subptr-1] = subtemp[subptr-1] + 1 ;   // 前一位加1
					subptr-- ;
				}
				else
					break ;
			}
			if( subtemp[subptr] <= subtemp_max[subptr])  // 若该位值不是最大，后面每位在前一位基础上加1
			{
				for( int i=subptr+1 ; i<subway ; i++ ) 
					subtemp[i] = subtemp[i-1] + 1 ;
			}
			// row++
			subrow++;
			subalready++ ;
		}
		delete[] subtemp ;
		delete[] subtemp_max ;

	} // end for each sub
}

// ----------------------------------------------------------------------------
// 计算测试用例test在未覆盖组合对集S中能覆盖的组合对数
// 标记FLAG=0只计算不修改，FLAG=1则将覆盖的组合情况设置为已覆盖
// 输入：一个测试用例test，标记FLAG
// 输出：覆盖组合数
// ----------------------------------------------------------------------------
int SUT::FitnessValue( const int *test , int FLAG )
{
	int num = 0 ;   // 返回值

	// 依次按字典序生成test的各种组合情况，然后从AllS表中判断是否覆盖，O(C(par,tway))

	int *pos = new int[tway] ;      // 储存参数情况
	int *pos_max = new int[tway];   // 各自最大值
	for( int k=0 ; k<tway ; k++ )  // 初始化
	{
		pos[k] = k ;
		pos_max[k] = parameter - tway + k ;
	}
	int end = tway - 1 ;
	int ptr = end ;

	int *sch = new int[tway] ;  // 存储取值的情况

	for( int row = 0 ; row < coverMain ; row++ )
	{
		// 得到组合情况
		for( int k=0 ; k<tway ; k++ )
			sch[k] = test[pos[k]] ;
		// 判断是否覆盖
		if( !Covered( pos , sch , FLAG ) )
			num++ ;

		// 生成一下个
		pos[end] = pos[end] + 1 ;  // 末位加1
		ptr = end ;
		while( ptr > 0 )
		{
			if( pos[ptr] > pos_max[ptr] )  // 超过该位允许最大值
			{
				pos[ptr-1] = pos[ptr-1] + 1 ;   // 前一位加1
				ptr-- ;
			}
			else
				break ;
		}
		if( pos[ptr] <= pos_max[ptr])  // 若该位值不是最大，后面每位在前一位基础上加1
		{
			for( int i=ptr+1 ; i<tway ; i++ ) 
				pos[i] = pos[i-1] + 1 ;
		}
	}
	delete[] pos;
	delete[] pos_max;
	delete[] sch;

	if( subInfo.size() !=  0 )
		num += FitnessValueSub( test , FLAG );

	return num ;
}

// Sub部分
int SUT::FitnessValueSub( const int *test , int FLAG )
{
	int subnum = 0 ;   // 返回值

	// 对每个Sub
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		int subway = (*i).subway ;
		int subparameter = (*i).subparameter ;
		int *subposition = (*i).subposition ;
		// 依次按字典序生成test的各种组合情况，然后从AllS表中判断是否覆盖，O(C(subpar,subway))

		int *subpos = new int[subway] ;      // 储存参数情况
		int *subpos_max = new int[subway];   // 各自最大值
		for( int k=0 ; k<subway ; k++ )  // 初始化
		{
			subpos[k] = k ;
			subpos_max[k] = subparameter - subway + k ;
		}
		int subend = subway - 1 ;
		int subptr = subend ;

		int *subsch = new int[subway] ;  // 存储取值的情况

		for( int row = 0 ; row < (*i).coverSub ; row++ )
		{
			// 得到组合情况
			for( int k=0 ; k<subway ; k++ )
				subsch[k] = test[subposition[subpos[k]]] ;

			// 判断是否覆盖
			if( !Covered( subpos , subsch , i , FLAG ) )
				subnum++ ;

			// 生成一下个
			subpos[subend] = subpos[subend] + 1 ;  // 末位加1
			subptr = subend ;
			while( subptr > 0 )
			{
				if( subpos[subptr] > subpos_max[subptr] )  // 超过该位允许最大值
				{
					subpos[subptr-1] = subpos[subptr-1] + 1 ;   // 前一位加1
					subptr-- ;
				}
				else
					break ;
			}
			if( subpos[subptr] <= subpos_max[subptr])  // 若该位值不是最大，后面每位在前一位基础上加1
			{
				for( int i=subptr+1 ; i<subway ; i++ ) 
					subpos[i] = subpos[i-1] + 1 ;
			}
		}
		delete[] subpos;
		delete[] subpos_max;
		delete[] subsch;
	}

	return subnum ;
}

// ----------------------------------------------------------------------------
// 计算一个取值情况是否覆盖，其中pos存储参数字典序号，sch存储对应取值
// 输入：pos[tway]，sch[tway]，FLAG=1则表示未覆盖时需要将该位置1
// 输出：true为已覆盖，false为未覆盖
// ----------------------------------------------------------------------------
bool SUT::Covered( const int *pos , const int *sch , int FLAG )
{
	// 通过position计算得到行号，schema计算得到列号，查AllS得出结果，接近O(1)

	bool ret = true ;  // 返回值

	// 存储行列信息
	int row = 0 ;
	int column = 0 ;       // 哪一个BYTE
	int column_bit = 0 ;   // 哪一个bit

	// 计算row，下标从0开始计算
	row = cal_combine2num( pos , parameter , tway );

	// 计算column，下标从0开始计算
	int Index = 0 ;
	int it = 0 ;
	for( int i=0 ; i<tway ; i++ )
	{
		it = sch[i] ;
		for( int j=i+1 ; j<tway ; j++ )
			it = value[pos[j]] * it ;
		Index += it ;
	}

	column = Index / 8 ;
	column_bit = Index % 8 ;

	MyByte byte = AllS[row][column] ;
	switch( column_bit )
	{
	case 0:
		if( !byte.bit.bit0 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit0 = 1 ;
				SCount--;
			}
		}
		break ;
	case 1:
		if( !byte.bit.bit1 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit1 = 1 ;
				SCount--;
			}
		}
		break ;
	case 2:
		if( !byte.bit.bit2 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit2 = 1 ;
				SCount--;
			}
		}
		break ;
	case 3:
		if( !byte.bit.bit3 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit3 = 1 ;
				SCount--;
			}
		}
		break ;
	case 4:
		if( !byte.bit.bit4 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit4 = 1 ;
				SCount--;
			}
		}
		break ;
	case 5:
		if( !byte.bit.bit5 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit5 = 1 ;
				SCount--;
			}
		}
		break ;
	case 6:
		if( !byte.bit.bit6 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit6 = 1 ;
				SCount--;
			}
		}
		break ;
	case 7:
		if( !byte.bit.bit7 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit7 = 1 ;
				SCount--;
			}
		}
		break ;
	}
	return ret ;
}

// Sub部分
// 其中pos存储子覆盖中对应字典序号，真实参数位置在sub.subposition中，sch存储对应取值
bool SUT::Covered( const int *pos , const int *sch , vector<SUBINFO>::const_iterator sub , int FLAG )
{
	int subway = (*sub).subway ;
	int subparameter = (*sub).subparameter ;
	int *subposition = (*sub).subposition ;

	// 通过position计算得到行号，schema计算得到列号，查AllS得出结果，接近O(1)

	bool subret = true ;  // 返回值

	// 存储行列信息
	int subrow = (*sub).coverIndex ;
	int subcolumn = 0 ;       // 哪一个BYTE
	int subcolumn_bit = 0 ;   // 哪一个bit

	// 计算row，下标从0开始计算
	subrow += cal_combine2num( pos , subparameter , subway );

	// 计算column，下标从0开始计算
	int subIndex = 0 ;
	int it = 0 ;
	for( int i=0 ; i<subway ; i++ )
	{
		it = sch[i] ;
		for( int j=i+1 ; j<subway ; j++ )
			it = value[subposition[pos[j]]] * it ;
		subIndex += it ;
	}

	subcolumn = subIndex / 8 ;
	subcolumn_bit = subIndex % 8 ;

	MyByte byte = AllS[subrow][subcolumn] ;
	switch( subcolumn_bit )
	{
	case 0:
		if( !byte.bit.bit0 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit0 = 1 ;
				SCount--;
			}
		}
		break ;
	case 1:
		if( !byte.bit.bit1 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit1 = 1 ;
				SCount--;
			}
		}
		break ;
	case 2:
		if( !byte.bit.bit2 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit2 = 1 ;
				SCount--;
			}
		}
		break ;
	case 3:
		if( !byte.bit.bit3 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit3 = 1 ;
				SCount--;
			}
		}
		break ;
	case 4:
		if( !byte.bit.bit4 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit4 = 1 ;
				SCount--;
			}
		}
		break ;
	case 5:
		if( !byte.bit.bit5 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit5 = 1 ;
				SCount--;
			}
		}
		break ;
	case 6:
		if( !byte.bit.bit6 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit6 = 1 ;
				SCount--;
			}
		}
		break ;
	case 7:
		if( !byte.bit.bit7 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit7 = 1 ;
				SCount--;
			}
		}
		break ;
	}
	return subret ;
}



