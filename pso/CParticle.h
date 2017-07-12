/**
 * Conventional Particle Representation
 * use a D-dimension vector to represent particle's position and velocity
 */
#pragma once

#include <cstdlib>
#include <cmath>
#include "../common/SUT.h"

class CParticle {

public:
  CParticle(int dim, const int *ran, int s, function<bool(const int*)> check)
    : dimension(dim), range(ran), checkValidity(check) {

    position = new int[dimension];
    velocity = new double[dimension];
    pbest = new int[dimension];
  };

  CParticle(int dim, const int *ran, int s) {
    CParticle(dim, ran, s, nullptr);
  };

  ~CParticle() {
    delete[] position;
    delete[] velocity;
    delete[] pbest;
  }

private:
  int dimension;      // the number of dimensions
  const int *range;   // the number of values of each dimension
  function<bool(const int*)> checkValidity ;  // the method to check validity of position

public:
  // current position, velocity and fitness value
  int *position;
  double *velocity;
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
  void velocityUpdate(double weight, double factor1, double factor2, const int *gbest, double pro1 = -1);
  void positionUpdate(double pro2 = -1, double pro3 = -1);
  // update velocity for CLPSO
  void velocityUpdateDim(double weight, double factor, const int *pbest, int dim, double pro1 = -1);

  // determine whether current position is valid
  bool isPositionValid() {
    return checkValidity == nullptr || checkValidity(position);
  }
};







