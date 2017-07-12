/**
 * Dynamic multi-swarm PSO
 *
 * J. Liang and P. Suganthan, Dynamic multi-swarm particle swarm optimizer,
 * in Swarm Intelligence Symposium, 2005. SIS 2005. Proceedings 2005 IEEE,
 * june 2005, pp. 124-129.
 */
#pragma once

#include "../Framework.h"

template <class Particle>
class DMSPSO : public Framework {

public:
  DMSPSO(SUT *s) {
    sut = s;
    if( TypeName<Particle>::Get() == "C" )
      setConfiguration(80, 250, 0.9, 1.3);
    else
      setConfiguration(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
  };

  int *Evolve() {
    double inertia = 0.9;
    double inertia_max = 0.9;
    double inertia_min = 0.4;

    vector<Particle*> T;
    int *best = new int[sut->parameter];
    int fit_best = 0;

    for (int i = 0; i < config.population; i++) {
      Particle *a = new Particle(sut->parameter, sut->value, sut->tway, checkValidity);
      a->randomInit();
      T.push_back(a);
    }

    for (int c = 0; c < sut->parameter; c++)
      best[c] = T[0]->position[c];

    int it = 1;

    // DMS
    int M = 4; // the number of particles of each group
    int R = 5;
    int SS = config.population / M;  // the number of groups
    int *Swarms = new int[config.population];
    int *Swarms_num = new int[SS];

    int **Swarms_localpos = new int *[SS];
    for (int k = 0; k < SS; k++)
      Swarms_localpos[k] = new int[sut->parameter];

    int *Swarms_localbest = new int[SS];
    for (int k = 0; k < SS; k++)
      Swarms_localbest[k] = -1;

    // random divide population
    for (int k = 0; k < SS; k++)
      Swarms_num[k] = 0;

    for (int k = 0; k < config.population; k++) {
      int a = (int) ((double) (rand() % 1000) / 1000.0 * SS);
      while (Swarms_num[a] == M)
        a = (int) ((double) (rand() % 1000) / 1000.0 * SS);

      Swarms[k] = a;
      Swarms_num[a]++;
    }

    while (true) {
      int index = 0;
      for ( auto i : T ) {
        if (i->isPositionValid()) {
          int fit = sut->fitnessValue(i->position, 0);

          if (fit == sut->testcaseCoverMax) {
            for (int c = 0; c < sut->parameter; c++)
              best[c] = i->position[c];

            for_each(T.begin(), T.end(), [](Particle* p){delete p;});
            T.clear();
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

          // update local best
          for (int k = 0; k < config.population; k++) {
            if (Swarms[k] == Swarms[index] && fit > Swarms_localbest[Swarms[k]]) {
              Swarms_localbest[Swarms[k]] = fit;
              for (int c = 0; c < sut->parameter; c++)
                Swarms_localpos[Swarms[k]][c] = i->position[c];
            }
          }
        }
        index += 1;
      }  // end for

      if (it >= (int) ((double) config.iteration * 0.9))
        break;

      index = 0;
      for (auto i : T) {
        i->velocityUpdate(inertia, config.factor, config.factor, Swarms_localpos[Swarms[index]], config.pro1);
        i->positionUpdate(config.pro2, config.pro3);
        index += 1;
      }

      it++;

      // inertia 0.9->0.4
      inertia = inertia_max - (inertia_max - inertia_min) * ((double) it / (double) config.iteration);

      // re divided swarm
      if (it % R == 0) {
        // random divide population
        for (int k = 0; k < SS; k++)
          Swarms_num[k] = 0;

        for (int k = 0; k < config.population; k++) {
          int a = (int) ((double) (rand() % 1000) / 1000.0 * SS);
          while (Swarms_num[a] == M)
            a = (int) ((double) (rand() % 1000) / 1000.0 * SS);
          Swarms[k] = a;
          Swarms_num[a]++;
        }
        for (int k = 0; k < SS; k++)
          Swarms_localbest[k] = 0;
      }
    }  // end 0.9 while

    // last 0.1
    while (true) {
      for ( auto i : T ) {
        if (i->isPositionValid()) {
          int fit = sut->fitnessValue(i->position, 0);

          if (fit == sut->testcaseCoverMax) {
            for (int c = 0; c < sut->parameter; c++)
              best[c] = i->position[c];

            for_each(T.begin(), T.end(), [](Particle* p){delete p;});
            T.clear();
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

      for (auto i : T) {
        i->velocityUpdate(inertia, config.factor, config.factor, best, config.pro1);
        i->positionUpdate(config.pro2, config.pro3);
      }

      it++;

      // inertia 0.9->0.4
      inertia = inertia_max - (inertia_max - inertia_min) * ((double) it / (double) config.iteration);

    }  // end 0.1 while

    for_each(T.begin(), T.end(), [](Particle* p){delete p;});
    T.clear();

    delete[] Swarms;
    delete[] Swarms_num;
    for (int k = 0; k < SS; k++)
      delete[] Swarms_localpos[k];
    delete[] Swarms_localpos;

    return best;
  }

};