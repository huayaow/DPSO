/**
 * Standard conventional and discrete PSO
 */
#pragma once

#include "Framework.h"
#include "CParticle.h"
#include <vector>

template<class Particle>
class PSO : public Framework {

public:
  PSO(SUT *s) {
    sut = s;
    if( TypeName<Particle>::Get() == "C" )
      setConfiguration(80, 250, 0.9, 1.3);
    else
      setConfiguration(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
  };

  // run PSO to generate a test case
  int *Evolve() {

    vector<Particle*> swarm;
    int *best = new int[sut->parameter];
    int fit_best = 0;

    for (int i = 0; i < config.population; i++) {
      Particle *a = new Particle(sut->parameter, sut->value, sut->tway, checkValidity);
      a->randomInit();
      while (!a->isPositionValid())
        a->randomInit();
      swarm.push_back(a);
    }

    // best = swarm[0]
    for (int c = 0; c < sut->parameter; c++)
      best[c] = swarm[0]->position[c];

    // number of iteration
    int it = 1;

    // if adaptive inertia is enabled, decrease from 0.9 -> 0.4
    bool adap_inertia = (config.weight == -1);
    double inertia = adap_inertia ? 0.9 : config.weight;

    while (true) {
      // evaluate each particle
      for ( auto i : swarm ) {
        bool validity = i->isPositionValid();

        // compute fitness value
        int penalty = validity ? 0 : int(sut->testcaseCoverMax * 0.5);
        int fit = sut->fitnessValue(i->position, 0) - penalty;

        // if fitness(t) = coverMax, then return
        if (fit == sut->testcaseCoverMax) {
          for (int c = 0; c < sut->parameter; c++)
            best[c] = i->position[c];

          for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
          swarm.clear();
          return best;
        }

        // update pBest
        if (fit > i->fitness_pbest)
          i->setPbest(fit);

        // update gBest
        if (validity && fit > fit_best) {
          fit_best = fit;
          for (int c = 0; c < sut->parameter; c++)
            best[c] = i->position[c];
        }

        // additional distance evaluation for DPSO
        if (validity && TypeName<Particle>::Get() == "D" && fit == fit_best) {
          if (hammingDist(i->position) < hammingDist(best)) {
            for (int c = 0; c < sut->parameter; c++)
              best[c] = i->position[c];
          }
        }
      }  // end for

      // stop condition
      if (it >= config.iteration)
        break;

      // update position and velocity
      for (auto i : swarm) {
        i->velocityUpdate(inertia, config.factor, config.factor, best, config.pro1);
        i->positionUpdate(config.pro2, config.pro3);
      }

      // next iteration
      it++;

      // adjust inertia weight
      if (adap_inertia)
        inertia = 0.9 - 0.5 * ((double) it / (double) config.iteration);

    }  // end while

    for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
    swarm.clear();
    return best;
  }
};