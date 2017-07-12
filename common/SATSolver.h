#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <vector>
#include <iostream>
#include "../minisat/solver/Solver.H"

/**
 * A literal in a SAT clause.
 */
class InputTerm {

protected:
  int	encoding;

public:
  InputTerm() : encoding(0) {}
  InputTerm(int encoding) : encoding(encoding) {}
  InputTerm(bool negated, int variable) : encoding( (variable << 1) | (int)negated) {}

  operator int() const {
    return encoding;
  }
  InputTerm&operator =(int encoding) {
    this->encoding = encoding;
    return *this;
  }

  bool isNegated() const {
    return (bool)(encoding & 1);
  }
  int getVariable() const {
    return encoding >> 1;
  }
};

/**
 * A SAT clause.
 */
class InputClause {

protected:
  int				maxVariable;
  vec<Lit>	literals;

public:
  InputClause();
  InputClause(const std::vector<InputTerm> &terms);
  InputClause(const std::vector<unsigned> &symbols);
  virtual ~InputClause();

  operator vec<Lit>&();
  operator const vec<Lit>&() const;

  int getMaxVariable() const {
    return maxVariable;
  }

  void clear();
  void append(InputTerm term);
  void undoAppend();
};


/**
 * SAT Solver
 */
class SATSolver {

protected:
  Solver solver;   // the miniSAT implementation
  bool enable;     // whether solver is enabled

public:
  SATSolver(bool ds = false) : enable(ds) {}
  void reserve(int variables);
  void addClause(InputClause &clause);
  bool operator()(const InputClause &clause);

  bool isEnabled() {
    return enable;
  }
};

#endif
