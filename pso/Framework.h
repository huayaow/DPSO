/**
 * One-test-at-a-time framework for covering array generation.
 */
#pragma once

#include "../common/SUT.h"
#include "CParticle.h"
#include "DParticle.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>

struct Configuration
{
  int population ;
  int iteration ;
  double weight ;
  double factor ;
  double pro1 ;
  double pro2 ;
  double pro3 ;
};

class Framework {
public:
  Framework() {};
  Framework(SUT *s) {
    sut = s;
  };
  ~Framework() {
    clear();
  };

public:
  SUT *sut;
  Configuration config;

  // final covering array generated
  vector<int *> ARRAY;
  unsigned long SIZE;
  unsigned long TIME;

public:
  void PSOEvolve();
  void setConfiguration(int par, int ite, double w, double f,
                        double p1 = 0, double p2 = 0, double p3 = 0);
  void printArray();

protected:
  // run Framework to generate a single test case
  virtual int *Evolve() { return 0; };
  // the function used to check constraint
  function<bool(const int*)> checkValidity ;

  double hammingDist(const int *test);
  void copy(vector<int *> A, vector<int *> &B, int para);
  void clear();
};


template <typename T>
struct TypeName {
  static const char* Get() {
    return typeid(T).name();
  }
};

template <>
struct TypeName<CParticle> {
  static const char* Get() {
    return "C";
  }
};

template <>
struct TypeName<DParticle> {
  static const char* Get() {
    return "D";
  }
};
