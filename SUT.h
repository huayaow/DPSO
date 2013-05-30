#pragma once

#include<vector>
using namespace std;

// 结构定义 : 位运算
typedef unsigned char byte ;
struct Bits
{
	byte bit0:1;
	byte bit1:1;
	byte bit2:1;
	byte bit3:1;
	byte bit4:1;
	byte bit5:1;
	byte bit6:1;
	byte bit7:1;
};
union MyByte
{
	byte allbyte ;
	struct Bits bit;
};

// 子覆盖信息
struct SUBINFO
{
	// subway表示强度，subparameter表示参数个数，subposition[subparameter]表示参数位置
	int subway ;
	int subparameter ;
	int *subposition ;
	// 将在初始化时赋值，标记在AllS中位置
	int coverIndex ;  //  在AllS中行下标
	int coverSub ;    //  C( subparameter , subway )
};

class SUT
{
	// 构造函数
public:
	SUT( int p , const int *v , int t )
	{
		parameter = p ;
		value = new int[p] ;
		for( int i = 0 ; i < p ; i++ )
			value[i] = v[i] ;
		tway = t ;

		AllS = NULL ;
		coverMain = cal_combine( p , t ) ;  	
	    coverMax = coverMain ;
		testcaseCoverMax = coverMax ;
	};
	~SUT()
	{
		delete[] value ;
		if( AllS != NULL )
		{
			for( int i=0 ; i<coverMax ; i++ )
				delete[] AllS[i];
			delete[] AllS;
			AllS = NULL;
		}
		for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
			delete[] (*i).subposition;
		subInfo.clear();
		for( vector<int*>::iterator i = seedInfo.begin() ; i != seedInfo.end() ; i++ )
			delete[] (*i) ;
		seedInfo.clear();
	}

// 变量
public:
	// CA变量
	int parameter ;
	int *value ;
	int tway ;
	
	// 子覆盖
	vector<SUBINFO> subInfo ;

	// 种子 
	vector<int*> seedInfo ;
	
protected:
	// 存储所有应覆盖组合情况，以及记录还未覆盖组合数
	MyByte **AllS ;    // 该位为0表示未覆盖

public:
	int coverMain ;    // 记录Main行数，C(parameter,tway)
	int coverMax ;     // 记录AllS总行数，Main+AllSub
	int SCountAll ;    // 总的待覆盖组合数
	int SCount ;       // 未覆盖组合数
	int testcaseCoverMax ;   // 一个测试用例最多能覆盖组合数
	
// 方法
public:
	// 其余信息初始化
	void SetSub( const vector<SUBINFO> sub );
	void SetSeed( const vector<int*> seed ); 

	// 生成AllS
        void GenerateS();
	void GenerateSSub();

	// 计算fitness
	int FitnessValue( const int *test , int FLAG );
	int FitnessValueSub( const int *test , int FLAG );

	// 判断是否覆盖
        bool Covered( const int *pos , const int *sch , int FLAG );
	bool Covered( const int *pos , const int *sch , vector<SUBINFO>::const_iterator sub , int FLAG );

	// 种子预处理
	void preforseed();

private:
	// private 计算用函数
	int cal_combine( int n , int m );
	int cal_combine2num( const int *c , const int n , const int m );    
	void cal_num2combine( int *c , int t , const int n , const int m );

};







