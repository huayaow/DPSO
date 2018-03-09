#include <string>
#include <iostream>
#include "common/SUT.h"
#include "common/Generator.h"
#include "common/io/Parse.h"
#include "common/io/ConstraintFile.h"
#include "common/io/SpecificationFile.h"
#include "common/io/OutputFile.h"
using namespace std;

int main(int argc, char **argv) 
{
  setOptions(argc, argv);
  showOptions();

  SpecificationFile sFile(modelFile);
  SUT *sut = new SUT(sFile.parameter, sFile.value, sFile.tway);

  if (constraintFile != "") {
    ConstraintFile cFile(constraintFile);
    sut->setConstraint(cFile.getClauses());
  }

  Framework* generator = initGenerator(algorithm, sut);

  srand((unsigned int)time(0));
  OutputFile outFile;
  for( int i = 0 ; i < repeat ; i++ ) {
    generator->PSOEvolve();
    outFile.update(generator->ARRAY, sut->parameter, generator->TIME);
  }
  outFile.write(sut, outputFile);
  cout << "Best Size = " << outFile.size << " Time = " << outFile.time << endl;

	return 0;
}




