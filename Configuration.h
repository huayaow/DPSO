#pragma once

#define MAX 999999

struct Configuration
{
	unsigned int random ;   // 随机数种子

	// 算法参数
	int population ;
	int iteration ;
	double weight ;
    double factor ;

};