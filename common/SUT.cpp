#include "SUT.h"
#include <cmath>
#include <algorithm>
using namespace std;

/**
 * Set variable covering requirement.
 * @param para
 * @param position
 * @param strength
 */
void SUT::appendVariableStrength(int para, const int *position, int strength) {
  SubInfo x;
  x.subParameter = para ;
  x.subWay = strength ;
  x.subPosition = new int[para];
  copy(position, position + strength, x.subPosition);

  x.coverIndex = coverMax;
  x.coverSub = combine(x.subParameter, x.subWay);

  // update variables
  coverMax += x.coverSub;
  testcaseCoverMax = coverMax;
  subInfo.push_back(x);
}

/**
 * Use a set of test cases as seed.
 * @param seed
 */
void SUT::setSeed(const vector<int *> seed) {
  for (auto each : seed) {
    int *temp = new int[parameter];
    for (int k = 0; k < parameter; k++)
      temp[k] = each[k];
    seedInfo.push_back(temp);
  }
}

/**
 * Initialize all combinations to be covered.
 */
void SUT::initializeCombination() {
  if (AllS != nullptr) {
    for (int i = 0; i < coverMax; i++)
      delete[] AllS[i];
    delete[] AllS;
    AllS = nullptr;
  }

  SCount = 0;
  AllS = new MyByte *[coverMax];

  // for each parameter combination in Main part
  int already = 0;
  vector<int*> allP = allPos(parameter, tway);
  for (auto pos : allP) {
    // number of all value combinations
    int allcomb = 1;
    for (int p = 0; p < tway; p++)
      allcomb = allcomb * value[pos[p]];

    int column = (int) ceil((double) allcomb / (double) 8);
    AllS[already] = new MyByte[column];
    for (int k = 0; k < column; k++)
      AllS[already][k].allbyte = 0x00;

    SCount += allcomb;
    already += 1;

    // deal with constraint
    if (solver.isEnabled()) {
      for (auto val : allVal(pos, tway, value)) {
        if (!isValid(pos, val, tway)) {
          covered(pos, val, 1);
        }
      }
    }
  }
  for_each(allP.begin(), allP.end(), default_delete<int>());
  allP.clear();

  // deal with variable covering strength
  if (subInfo.size() != 0)
    initializeCombinationSub();

  // deal with seed
  if (seedInfo.size() != 0) {
    for (auto each : seedInfo)
      fitnessValue(each, 1);
  }

  SCountAll = SCount;
}

/**
 * Initialize all combinations to be covered. Variable Strength Part.
 */
void SUT::initializeCombinationSub() {
  for (auto each : subInfo) {
    int t = each.subWay;
    int para = each.subParameter;
    int* position = each.subPosition;

    int row = each.coverIndex;
    int* pos = new int[t];
    vector<int*> allP = allPos(para, t);
    for (auto _pos : allP) {
      // actual position
      for (int k=0 ; k<t ; k++)
        pos[k] = position[_pos[k]];

      // number of all value combinations
      int allcomb = 1;
      for (int k = 0; k < t; k++)
        allcomb = allcomb * value[_pos[k]];

      int column = (int) ceil((double) allcomb / (double) 8);
      AllS[row] = new MyByte[column];
      for (int k = 0; k < column; k++)
        AllS[row][k].allbyte = 0x00;

      SCount += allcomb;

      // deal with constraint with SubInfo
      if (solver.isEnabled()) {
        vector<int*> allV = allVal(pos, t, value);
        for (auto val : allV) {
          if (!isValid(pos, val, t)) {
            coveredSub(pos, val, each, 1);
          }
        }
        for_each(allV.begin(), allV.end(), default_delete<int>());
        allV.clear();
      }
      // next row
      row += 1 ;
    }
    for_each(allP.begin(), allP.end(), default_delete<int>());
    allP.clear();
    delete[] pos;
  } // end for each sub
}

/**
 * Compute the number of yet uncovered combinations that can be covered
 * by a given test case. FLAG indicates whether to update AllS.
 * @param test
 * @param FLAG
 * @return
 */
int SUT::fitnessValue(const int *test, int FLAG) {
  int num = 0;

  int* val = new int[tway];
  vector<int*> allP = allPos(parameter, tway);
  for (auto pos : allP) {
    for (int k = 0; k < tway; k++)
      val[k] = test[pos[k]];

    if (!covered(pos, val, FLAG))
      num++;
  }
  for_each(allP.begin(), allP.end(), default_delete<int>());
  allP.clear();
  delete[] val;

  if (subInfo.size() != 0)
    num += fitnessValueSub(test, FLAG);

  return num;
}

/**
 * Fitness value computation for variable strength part.
 * @param test
 * @param FLAG
 * @return
 */
int SUT::fitnessValueSub(const int* test, int FLAG) {
  int num = 0;
  for (auto each : subInfo) {
    int t = each.subWay;
    int para = each.subParameter;
    int *position = each.subPosition;

    int *pos = new int[t];
    int *val = new int[t];
    vector<int*> allP = allPos(para, t);
    for (auto _pos : allP) {
      // actual position
      for (int k = 0; k < t; k++)
        pos[k] = position[_pos[k]];
      // actual value
      for (int k = 0; k < t; k++)
        val[k] = test[pos[k]];

      if (!coveredSub(_pos, val, each, FLAG))
        num++;
    }
    for_each(allP.begin(), allP.end(), default_delete<int>());
    allP.clear();
    delete[] pos;
    delete[] val;
  }
  return num;
}


/**
 * Determine whether a combination is covered or not. If FLAG = true,
 * then update AllS to make this combination as covered.
 * @param pos
 * @param val
 * @param FLAG
 * @return
 */
bool SUT::covered(const int* pos, const int* val, int FLAG) {
  bool ret = true;

  int row = pos2num(pos, parameter, tway);
  int column = 0;       // which BYTE
  int column_bit = 0;   // which bit

  int Index = 0, it = 0;
  for (int i = 0; i < tway; i++) {
    it = val[i];
    for (int j = i + 1; j < tway; j++)
      it = value[pos[j]] * it;
    Index += it;
  }

  column = Index / 8;
  column_bit = Index % 8;

  coveredBit(AllS[row][column], column_bit, ret, FLAG);
  return ret;
}

/**
 * Deal with each bit in MyByte.
 * @param byte
 * @param bit
 * @param result
 * @param FLAG
 */
void SUT::coveredBit(MyByte &byte, int bit, bool &result, int FLAG) {
  switch (bit) {
    case 0:
      if (!byte.bit.bit0) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit0 = 1;
          SCount--;
        }
      }
      break;
    case 1:
      if (!byte.bit.bit1) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit1 = 1;
          SCount--;
        }
      }
      break;
    case 2:
      if (!byte.bit.bit2) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit2 = 1;
          SCount--;
        }
      }
      break;
    case 3:
      if (!byte.bit.bit3) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit3 = 1;
          SCount--;
        }
      }
      break;
    case 4:
      if (!byte.bit.bit4) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit4 = 1;
          SCount--;
        }
      }
      break;
    case 5:
      if (!byte.bit.bit5) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit5 = 1;
          SCount--;
        }
      }
      break;
    case 6:
      if (!byte.bit.bit6) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit6 = 1;
          SCount--;
        }
      }
      break;
    case 7:
      if (!byte.bit.bit7) {
        result = false;
        if (FLAG == 1) {
          byte.bit.bit7 = 1;
          SCount--;
        }
      }
      break;
    default:
      cerr << "error at coveredBit()" << endl;
      break;
  }
}

/**
 *
 * @param _pos
 * @param val
 * @param sub
 * @param FLAG
 * @return
 */
bool SUT::coveredSub(const int* _pos, const int* val, const SubInfo sub, int FLAG) {
  int t = sub.subWay;
  int para = sub.subParameter;
  int* position = sub.subPosition;

  bool ret = true;

  int row = sub.coverIndex + pos2num(_pos, para, t);

  int column = 0;
  int column_bit = 0;

  int subIndex = 0;
  int it = 0;
  for (int i = 0; i < t; i++) {
    it = val[i];
    for (int j = i + 1; j < t; j++)
      it = value[position[_pos[j]]] * it;
    subIndex += it;
  }

  column = subIndex / 8;
  column_bit = subIndex % 8;

  coveredBit(AllS[row][column], column_bit, ret, FLAG);
  return ret;
}


/**
 * Set hard constraints based on clauses.
 * Set at-least and at-most constraints based on SUT,
 * @param clause
 */
void SUT::setConstraint(const vector<InputClause> &clauses) {
  // hard constraint
  for (unsigned i = 0; i < clauses.size(); i++) {
    solver.addClause(const_cast<InputClause &>(clauses[i]));
  }

  for (unsigned i = 0; i < parameter; i++) {
    // at-least constraint
    InputClause atLeast;
    for (unsigned j = 0; j < value[i]; j++)
      atLeast.append(InputTerm(false, relations[i][j]));
    solver.addClause(atLeast);

    // at-most constraint
    for (unsigned x = 0; x < value[i] - 1; x++) {
      for (unsigned y = x + 1; y < value[i]; y++) {
        InputClause atMost;
        atMost.append(InputTerm(true, relations[i][x]));
        atMost.append(InputTerm(true, relations[i][y]));
        solver.addClause(atMost);
      }
    }
  }
}

/**
 * Determine whether a test case is valid.
 * @param test
 * @return
 */
bool SUT::isValid(const int *test) {
  if (!solver.isEnabled())
    return true ;

  InputClause clause;
  for (unsigned i = 0; i < parameter; i++)
    clause.append(InputTerm(false, relations[i][test[i]]));

  return solver(clause);
}

/**
 * Determine whether a combination is valid.
 * @param test
 * @return
 */
bool SUT::isValid(const int *pos, const int *sch, int strength) {
  if (!solver.isEnabled())
    return true ;

  InputClause clause;
  for (unsigned i = 0; i < strength; i++)
    clause.append(InputTerm(false, relations[pos[i]][sch[i]]));

  return solver(clause);
}
