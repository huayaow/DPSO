#pragma once

#include <vector>
#include "Comb.h"
#include "SATSolver.h"
using namespace std;

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

/**
 * Requirement of variable covering strength
 */
struct SubInfo
{
	int subWay;         // covering strength
	int subParameter;   // number of parameters involved
	int* subPosition;   // the position of each parameter

  // the followings will be initialized in initializeCombination()
  int coverIndex ;    // the index in AllS
	int coverSub ;      // C(subParameter, subWay)
};

/**
 * Testing Model
 */
class SUT {

public:
	SUT( int p , const int *v , int t ) : parameter(p), tway(t) {
		value = new int[p] ;
		for( int i = 0 ; i < p ; i++ )
			value[i] = v[i] ;

		AllS = nullptr ;
		coverMain = combine(p, t);
    coverMax = coverMain;
		testcaseCoverMax = coverMain;

    relations = new int*[parameter];
    int start = 1;
    for (int i = 0; i < parameter; i++) {
      relations[i] = new int[value[i]];
      for (int j = 0; j < value[i]; j++, start++)
        relations[i][j] = start;
    }

	};
	~SUT()
	{
		delete[] value ;
    for( int i = 0 ; i < parameter ; i++ )
      delete[] relations[i];
    delete[] relations;

		if( AllS != nullptr ) {
			for( int i = 0 ; i < coverMax ; i++ )
				delete[] AllS[i];
			delete[] AllS;
			AllS = nullptr;
		}

		for( vector<SubInfo>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
			delete[] (*i).subPosition;
		subInfo.clear();

		for( vector<int*>::iterator i = seedInfo.begin() ; i != seedInfo.end() ; i++ )
			delete[] (*i) ;
		seedInfo.clear();
	}

  //
  // Model
  //
public:
	int parameter ;
	int* value ;
	int tway ;
	
	vector<SubInfo> subInfo ; 	// variable covering strength
	vector<int*> seedInfo ;     // seed

  //
  // Constraint
  //
protected:
  int** relations;     // mapping between parameter value and variable in CNF, start from 1
  SATSolver solver ;   // SAT solver

public:
  // set constraint based on the results obtained from ConstraintFile
  void setConstraint(const vector<InputClause> &clause);
  // whether a test case is valid
  bool isValid(const int* test);
  // whether a combination is valid
  bool isValid(const int* pos, const int* sch, int strength);

  //
  // Combinations to be covered
  //
public:
  int coverMain ;  // C(parameter,tway), for uniform covering
  int coverMax ;   // C(parameter,tway) + AllSub, for variable covering
  int SCountAll ;  // total number of combinations to be covered
  int SCount ;     // number of yet uncovered combinations
  int testcaseCoverMax ;  // maximum number of combination that can be covered by a test case

protected:
  MyByte** AllS ;  // each combination to be covered

public:
  // set variable strength covering requirement
	void appendVariableStrength(int para, const int *position, int strength);
	// initialize all combinations to be covered
  void initializeCombination();
	// compute fitness value of each test case
	int fitnessValue(const int *test, int FLAG);
	// whether a combination is covered or not
  bool covered(const int *pos, const int *val, int FLAG);
	// seed
  void setSeed(const vector<int*> seed);

  void show() {
    cout << "-----------------------------" << endl;
    cout << "Parameter = " << parameter << ", t-way = " << tway << endl;
		cout << "Constraint = " << solver.isEnabled() << endl;
    cout << "coverMain = " << coverMain << endl;
    cout << "coverMax = " << coverMax << endl;
    cout << "SCount = " << SCount << endl;
    cout << "-----------------------------" << endl;
  }

private:
  // methods to deal with variable strength covering requirement
  void initializeCombinationSub();
  int fitnessValueSub(const int *test, int FLAG);
  bool coveredSub(const int *pos, const int *val, const SubInfo sub, int FLAG);
  // method to update AllS
  void coveredBit(MyByte &byte, int bit, bool &result, int FLAG);

};







