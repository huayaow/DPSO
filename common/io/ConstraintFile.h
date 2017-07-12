// Copyright 2008, 2009 Brady J. Garvin
// This file is part of Covering Arrays by Simulated Annealing (CASA).

#ifndef CONSTRAINTFILE_H
#define CONSTRAINTFILE_H

#include <fstream>
#include <string>
#include <vector>
#include "../SATSolver.h"

class ConstraintFile {

private:
  std::vector<InputClause> clauses;

public:
  ConstraintFile(const std::string &filename);
  const std::vector<InputClause> &getClauses() const;

  bool isEmpty() const {
    return clauses.size() == 0;
  }
};

#endif
