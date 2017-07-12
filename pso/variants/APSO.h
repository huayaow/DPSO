/**
 * Adaptive PSO
 *
 * Z. hui Zhan, J. Zhang, Y. Li, and H.-H. Chung, Adaptive particle swarm
 * optimization, Systems, Man, and Cybernetics, Part B: Cybernetics, IEEE
 * Transactions on, vol. 39, no. 6, pp. 1362-1381, dec. 2009.
 */
#pragma once

#include "../Framework.h"
#include <cfloat>

template <class Particle>
class APSO : public Framework {

public:
  APSO(SUT *s) {
    sut = s;
    if( TypeName<Particle>::Get() == "C" )
      setConfiguration(80, 250, 0.9, 1.3);
    else
      setConfiguration(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
  };

  int *Evolve() {
    double inertia = 0.9;
    double factor1 = 1.3;
    double factor2 = 1.3;
    double factor_max = 1.8;
    double factor_min = 0.8;

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

    // adaptive parameter adjustment
    int state = 1;
    double f_value = 0;
    double sigma_max = 1.0;
    double sigma_min = 0.1;

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

      // adaptive
      f_value = fCalculate(swarm, best);
      state = fuzzyDecision(f_value, state);

      inertia = 1 / (1 + 1.5 * exp(-2.6 * f_value));

      if (state == 1) {
        factor1 = factor1 + (0.05 + (double) (rand() % 50) / 1000.0);
        factor2 = factor2 - (0.05 + (double) (rand() % 50) / 1000.0);
      }
      if (state == 2) {
        factor1 = factor1 + 0.5 * (0.05 + (double) (rand() % 50) / 1000.0);
        factor2 = factor2 - 0.5 * (0.05 + (double) (rand() % 50) / 1000.0);
      }
      if (state == 3) {
        factor1 = factor1 + 0.5 * (0.05 + (double) (rand() % 50) / 1000.0);
        factor2 = factor2 + 0.5 * (0.05 + (double) (rand() % 50) / 1000.0);
      }
      if (state == 4) {
        factor1 = factor1 - (0.05 + (double) (rand() % 50) / 1000.0);
        factor2 = factor2 + (0.05 + (double) (rand() % 50) / 1000.0);
      }

      if (factor1 > factor_max)
        factor1 = factor_max;
      if (factor1 < factor_min)
        factor1 = factor_min;

      if (factor2 > factor_max)
        factor2 = factor_max;
      if (factor2 < factor_min)
        factor2 = factor_min;

      for (auto i : swarm) {
        i->velocityUpdate(inertia, factor1, factor2, best, config.pro1);
        i->positionUpdate(config.pro2, config.pro3);
      }

      // ELS
      int *gbest_tmp = new int[sut->parameter];
      for (int k = 0; k < sut->parameter; k++)
        gbest_tmp[k] = best[k];

      int dim = (int) (((double) (rand() % 1000) / 1000.0) * sut->parameter);
      double gr = gaussianRand(0, pow(sigma_max - (sigma_max - sigma_min) * (it / config.iteration), 2));
      gbest_tmp[dim] = gbest_tmp[dim] + (int) ((double) (sut->value[dim] - 1) * gr);
      if (gbest_tmp[dim] >= sut->value[dim])
        gbest_tmp[dim] = sut->value[dim] - 1;
      if (gbest_tmp[dim] < 0)
        gbest_tmp[dim] = 0;

      int fit_tmp = sut->fitnessValue(gbest_tmp, 0);
      if (fit_tmp > fit_best && checkValidity(gbest_tmp)) {
        fit_best = fit_tmp;
        for (int c = 0; c < sut->parameter; c++)
          best[c] = gbest_tmp[c];
      }

      it++;
    } // end while

    for_each(swarm.begin(), swarm.end(), [](Particle* p){delete p;});
    swarm.clear();
    return best;
  }

private:
  // f = ( d[g] - d[min] ) / ( d[max] - d[min] )
  // where d[i] = the mean distance of particle i
  double fCalculate(const vector<Particle *> swarm, const int *gbest) {
    double min_dis = DBL_MAX;
    double max_dis = DBL_MIN;
    double g_dis = 0;

    // for each particle
    for (int i = 0 ; i < swarm.size() ; i++) {
      double all_tmp = 0;
      for (int j = 0 ; j < swarm.size() ; j++) {
        if ( i != j ) {
          double tmp = 0;
          for (int k = 0; k < sut->parameter; k++)
            tmp += pow((double) swarm[i]->position[k] - (double) swarm[j]->position[k], 2);
          all_tmp += sqrt(tmp);
        }
      }
      all_tmp = all_tmp / (double) (swarm.size() - 1);

      if (all_tmp < min_dis)
        min_dis = all_tmp;

      if (all_tmp > max_dis)
        max_dis = all_tmp;
    }

    // for gbest
    for (auto i : swarm) {
      double tmp = 0;
      for (int k = 0; k < sut->parameter; k++)
        tmp += pow((double) i->position[k] - (double) gbest[k], 2);
      g_dis += sqrt(tmp);
    }
    g_dis = g_dis / (double) (swarm.size() - 1);

    // return f
    return (g_dis - min_dis) / (max_dis - min_dis);
  }

  // S1 = exploration
  // S2 = exploitation
  // S3 = convergence
  // S4 = jumping-out
  int fuzzyDecision(double f, int previous) {
    if (f <= 0.2)
      return 3;

    if (0.2 < f && f < 0.3) {
      // s3 > s2
      if (f < (double) 7 / (double) 30)
        return (previous == 2 || previous == 1) ? 2 : 3 ;
      // s3 <= s2
      else
        return (previous == 3) ? 3 : 2 ;
    }

    if (0.3 <= f && f <= 0.4)
      return 2;

    if (0.4 < f && f < 0.6) {
      // s2 > s1
      if (f < 0.5)
        return (previous == 1 || previous == 4) ? 1 : 2 ;
      // s2 <= s1
      else
        return (previous == 2) ? 2 : 1 ;
    }

    if (0.6 <= f && f <= 0.7)
      return 1;

    if (0.7 < f && f < 0.8) {
      // s1 > s4
      if (f < (double) 23 / (double) 30)
        return (previous == 4 || previous == 3) ? 4 : 1 ;
      // s1 <= s4
      else
        return  (previous == 1) ? 1 : 4 ;
    }

    if (f >= 0.8)
      return 4;

    return -1;
  }

  // Gaussian Distribution
  // Expectation - E , Variance - V
  double gaussianRand(double E, double V) {
    static double V1, V2, S;
    static int phase = 0;
    double X;
    if (phase == 0) {
      do {
        double U1 = (double) rand() / RAND_MAX;
        double U2 = (double) rand() / RAND_MAX;
        V1 = 2 * U1 - 1;
        V2 = 2 * U2 - 1;
        S = V1 * V1 + V2 * V2;
      } while (S >= 1 || S == 0);
      X = V1 * sqrt(-2 * log(S) / S);
    } else {
      X = V2 * sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;
    return X * V + E;
  }
};