/**
 * Time-varying Acceleration Coefficient PSO
 *
 * A. Ratnaweera, S. Halgamuge, and H. Watson, Self-organizing hierarchical
 * particle swarm optimizer with time-varying acceleration coefficients,
 * Evolutionary Computation, IEEE Transactions on, vol.8, no.3, 2004.
 */
#pragma once

#include "../Framework.h"

template<class Particle>
class TVAC : public Framework {

public:
  TVAC(SUT *s) {
    sut = s;
    if( TypeName<Particle>::Get() == "C" )
      setConfiguration(80, 250, 0.9, 1.3);
    else
      setConfiguration(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
  };

  int *Evolve() {
    double inertia = 0.9;
    const double inertia_max = 0.9;
    const double inertia_min = 0.4;
    double factor1 = 1.3;
    double factor2 = 1.3;
    const double factor_max = 1.8;
    const double factor_min = 0.8;

    vector<Particle*> swarm;
    int *best = new int[sut->parameter];
    int fit_best = 0;

    for (int i = 0; i < config.population; i++) {
      Particle *a = new Particle(sut->parameter, sut->value, sut->tway, checkValidity);
      a->randomInit();
      swarm.push_back(a);
    }

    for (int c = 0; c < sut->parameter; c++)
      best[c] = swarm[0]->position[c];

    int it = 1;
    while (true) {
      for ( auto i : swarm ) {
        if (i->isPositionValid()) {
          int fit = sut->fitnessValue(i->position, 0);

          if (fit == sut->testcaseCoverMax) {
            for (int c = 0; c < sut->parameter; c++)
              best[c] = i->position[c];

            for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
            swarm.clear();
            return best;
          }

          if (fit > i->fitness_pbest)
            i->setPbest(fit);

          if (fit > fit_best) {
            fit_best = fit;
            for (int c = 0; c < sut->parameter; c++)
              best[c] = i->position[c];
          }

          if( TypeName<Particle>::Get() == "D" && fit == fit_best) {
            if (hammingDist(i->position) < hammingDist(best)) {
              for (int c = 0; c < sut->parameter; c++)
                best[c] = i->position[c];
            }
          }
        }
      }  // end for

      if (it >= config.iteration)
        break;

      for (auto i : swarm) {
        i->velocityUpdate(inertia, factor1, factor2, best, config.pro1);
        i->positionUpdate(config.pro2, config.pro3);
      }

      it++;

      // TVAC
      // adjust inertia and factor
      inertia = inertia_max - (inertia_max - inertia_min) * ((double) it / (double) config.iteration);
      factor1 = (factor_min - factor_max) * ((double) it / (double) config.iteration) + factor_max;
      factor2 = (factor_max - factor_min) * ((double) it / (double) config.iteration) + factor_min;

    }  // end while

    for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
    swarm.clear();

    return best;
  }

};