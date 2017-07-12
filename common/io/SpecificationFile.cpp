#include "SpecificationFile.h"
#include <fstream>
#include <iostream>
using namespace std;

SpecificationFile::SpecificationFile(const string &filename) {

  ifstream infile(filename.data());
  if (!infile.is_open()) {
    cerr << "cannot open specification file" << endl;
  }

  infile >> tway >> parameter;
  value = new int[parameter];
  for (unsigned i = 0; i < parameter; i++) {
    infile >> value[i];
  }

  infile.close();
}
