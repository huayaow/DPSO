#include"CParticle.h"

// ----------------------------------------------------------------------------
// initialize
// ----------------------------------------------------------------------------
void CParticle::randomInit() {
  // get a random and valid position
  do {
    for (int i = 0; i < dimension; i++)
      position[i] = (int) (((double) (rand() % 1000) / 1000.0) * range[i]);
  } while (!isPositionValid());

  // get a random velocity
  for (int i = 0; i < dimension; i++)
    velocity[i] = 2 * ((double) (rand() % 1000) / 1000.0) - 1;

  // set pbest as itself
  for (int i = 0; i < dimension; i++)
    pbest[i] = position[i];

  fitness_pbest = 0;
  fitness = 0;
}

// ----------------------------------------------------------------------------
// update pbest based on current position
// ----------------------------------------------------------------------------
void CParticle::setPbest(int fit) {
  if (isPositionValid()) {
    for (int i = 0; i < dimension; i++)
      pbest[i] = position[i];
    fitness_pbest = fit;
  }
}

// ----------------------------------------------------------------------------
// velocity update
// variants version does not use pro1
// ----------------------------------------------------------------------------
void CParticle::velocityUpdate(double weight, double factor1, double factor2,
                              const int *gbest, double pro1) {
  for (int v = 0; v < dimension; v++) {
    double r1 = (double) (rand() % 1000) / 1000.0;
    double r2 = (double) (rand() % 1000) / 1000.0;

    velocity[v] = (double) weight * velocity[v] +
                  (double) factor1 * r1 * (pbest[v] - position[v]) +
                  (double) factor2 * r2 * (gbest[v] - position[v]);

    // maximum velocity limitation
    if (velocity[v] > range[v] / 2)
      velocity[v] = (double) range[v] / 2;
    if (velocity[v] < -range[v] / 2)
      velocity[v] = (double) -range[v] / 2;
  }
}

// ----------------------------------------------------------------------------
// velocity update for CLPSO
// ----------------------------------------------------------------------------
void CParticle::velocityUpdateDim(double weight, double factor, const int *pbest,
                                 int dim, double pro1) {
  double r1 = (double) (rand() % 1000) / 1000.0;
  velocity[dim] = (double) weight * velocity[dim] +
                  (double) factor * r1 * (pbest[dim] - position[dim]);

  // maximum velocity limitation
  if (velocity[dim] > range[dim] / 2)
    velocity[dim] = (double) range[dim] / 2;
  if (velocity[dim] < -range[dim] / 2)
    velocity[dim] = (double) -range[dim] / 2;
}

// ----------------------------------------------------------------------------
// position update
// ----------------------------------------------------------------------------
void CParticle::positionUpdate(double pro2, double pro3) {
  for (int v = 0; v < dimension; v++) {
    position[v] = (int)(position[v] + velocity[v]);

    // Reflecting Walls
    if (position[v] >= range[v])
      position[v] = 2 * range[v] - position[v] - 1;
    if (position[v] < 0)
      position[v] = -position[v] - 1;
  }
}