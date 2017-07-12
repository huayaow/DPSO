/**
 * Discrete Particle Representation
 * use a set with probability to represent particle's velocity
 */
#pragma once

#include <cstdlib>
#include <cmath>
#include <map>
#include <set>
#include <iostream>
#include "../common/Comb.h"

using namespace std;

// to represent a t-way combination
struct PosVal {
  PosVal() {}
  PosVal(int pos, int val) : posI(pos), valI(val) {}

  int posI;  // index of parameter combination
  int valI;  // index of value combination

  bool operator<(PosVal const &_PV) const {
    return posI < _PV.posI || (posI == _PV.posI && valI < _PV.valI);
  }

  bool operator=(PosVal const &_PV) const {
    return posI == _PV.posI && valI == _PV.valI;
  }
};

class DParticle {
public:
  DParticle(int dim, const int *ran, int s, function<bool(const int *)> check)
    : dimension(dim), range(ran), strength(s), checkValidity(check) {

    PosMax = combine(dim, s);
    ValMax = new int[PosMax];
    for (int k = 0; k < PosMax; k++) {
      int comb = 1;
      int *pos = num2pos(k, dim, s);
      for (int p = 0; p < s; p++)
        comb = comb * range[pos[p]];
      delete[] pos;
      ValMax[k] = comb;
    }

    position = new int[dimension];
    pbest = new int[dimension];
  };

  DParticle(int dim, const int *ran, int s) {
    DParticle(dim, ran, s, nullptr);
  };

  ~DParticle() {
    delete[] position ;
    delete[] pbest ;
    delete[] ValMax ;
    velocity.clear();
    TempV.clear();
  }

private:
  int dimension;      // the number of dimensions
  const int *range;   // the number of values of each dimension
  function<bool(const int *)> checkValidity;  // the method to check validity of position

  int strength;
  int PosMax;
  int *ValMax;
  set<pair<double, PosVal> > TempV;

public:
  // current position, velocity and fitness value
  int *position;
  map<PosVal, double> velocity;
  int fitness;

  // the best personal position (pbest) and its fitness value
  int *pbest;
  int fitness_pbest;

public:
  // randomly initialize
  void randomInit();
  // update pbest
  void setPbest(int fit);
  // update velocity and position
  void velocityUpdate(double weight, double factor1, double factor2, const int *gbest, double prob1);
  void positionUpdate(double prob2, double prob3);
  // update velocity for CLPSO
  void velocityUpdateDim(double weight, double factor, const int *clbest, int dim, double prob1);

  // determine whether current position is valid
  bool isPositionValid() {
    return checkValidity == nullptr || checkValidity(position);
  }
};







