/**
 * Comprehensive learning PSO
 *
 * J. Liang, A. Qin, P. Suganthan, and S. Baskar, Comprehensive learning
 * particle swarm optimizer for global optimization of multimodal functions,
 * Evolutionary Computation, IEEE Transactions on, vol.10, no.3, 2006.
 */
#pragma once

#include "../Framework.h"

template <class Particle>
class CLPSO : public Framework {

public:
  CLPSO(SUT *s) {
    sut = s;
    if( TypeName<Particle>::Get() == "C" )
      setConfiguration(80, 250, 0.9, 1.3);
    else
      setConfiguration(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
  };

  int *Evolve() {
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

    // PC[i]
    double *Pc = new double[config.population];
    for (int k = 0; k < config.population; k++)
      Pc[k] = 0.05 + 0.45 * ((exp((double) (10 * k) / (double) (config.population - 1)) - 1) / (exp((double) 10) - 1));

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

      // update the swarm
      int index = 0;
      for (auto i : swarm) {
        // each dimension
        for (int d = 0; d < sut->parameter; d++) {
          // CL: if random > Pc[i], then learn from own pbest
          if ((double) (rand() % 1000) / 1000.0 > Pc[index]) {
            i->velocityUpdateDim(config.weight, config.factor, i->pbest, d, config.pro1);
          }
          // else, learn from other pbest
          else {
            int f1, f2;
            do {
              f1 = (int) (((double) (rand() % 1000) / 1000.0) * config.population);
            } while (f1 == index);
            do {
              f2 = (int) (((double) (rand() % 1000) / 1000.0) * config.population);
            } while (f2 == index);

            if (sut->fitnessValue(swarm[f1]->position, 0) > sut->fitnessValue(swarm[f2]->position, 0))
              i->velocityUpdateDim(config.weight, config.factor, swarm[f1]->pbest, d, config.pro1);
            else
              i->velocityUpdateDim(config.weight, config.factor, swarm[f2]->pbest, d, config.pro1);
          }
        }
        i->positionUpdate(config.pro2, config.pro3);
        index += 1;
      }

      it++;

    }  // end while

    for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
    swarm.clear();
    return best;
  }
};