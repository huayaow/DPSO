#ifndef GENERATOR_H
#define GENERATOR_H

#include "SUT.h"
#include "../pso/Framework.h"
#include "../pso/PSO.h"
#include "../pso/variants/TVAC.h"
#include "../pso/variants/CLPSO.h"
#include "../pso/variants/APSO.h"
#include "../pso/variants/DMSPSO.h"

static Framework *initGenerator(string algorithm, SUT *sut) {
  Framework *fp;

  if (algorithm == "CPSO")
    fp = new PSO<CParticle>(sut);
  else if (algorithm == "DPSO")
    fp = new PSO<DParticle>(sut);

  else if (algorithm == "CTVAC")
    fp = new TVAC<CParticle>(sut);
  else if (algorithm == "DTVAC")
    fp = new TVAC<DParticle>(sut);

  else if (algorithm == "CAPSO")
    fp = new APSO<CParticle>(sut);
  else if (algorithm == "DAPSO")
    fp = new APSO<DParticle>(sut);

  else if (algorithm == "CCLPSO")
    fp = new CLPSO<CParticle>(sut);
  else if (algorithm == "DCLPSO")
    fp = new CLPSO<DParticle>(sut);

  else if (algorithm == "CDMSPSO")
    fp = new DMSPSO<CParticle>(sut);
  else if (algorithm == "DDMSPSO")
    fp = new DMSPSO<DParticle>(sut);

  else {
    cerr << "cannot identify the specified algorithm" << endl;
    return nullptr;
  }

  return fp;
}

#endif
