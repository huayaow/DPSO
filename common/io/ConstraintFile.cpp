// Copyright 2008, 2009 Brady J. Garvin
// This file is part of Covering Arrays by Simulated Annealing (CASA).

#include "ConstraintFile.h"
#include <iostream>
using namespace std;

ConstraintFile::ConstraintFile(const string& filename) {
  if (!filename.size())
    return;

  ifstream infile(filename.data());
  if (!infile.is_open()) {
    cerr << "cannot open constraint file" << endl;
  }

  unsigned clauseCount;
  infile >> clauseCount;
  clauses = std::move(std::vector<InputClause> (clauseCount));

  for (unsigned i = 0; i < clauseCount; ++i) {
    InputClause& clause = clauses[i];
    unsigned termCount;
    infile >> termCount;
    while (termCount--) {
      char sign;
      unsigned symbol;
      do {
        infile >> sign;
      } while (sign != '-' && sign != '+');
      infile >> symbol;
      clause.append(InputTerm(sign == '-', symbol));
    }
  }

  infile.close();
}

const std::vector<InputClause>&ConstraintFile::getClauses() const {
  return clauses;
}
