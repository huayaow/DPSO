#include "SATSolver.h"
#include <iostream>
InputClause::InputClause() : maxVariable(-1) {}

InputClause::InputClause(const std::vector<InputTerm> &terms) : maxVariable(-1) {
  unsigned long size = terms.size();
  for (unsigned i = 0; i < size; ++i) {
    append(terms[i]);
  }
}

InputClause::InputClause(const std::vector<unsigned> &symbols) : maxVariable(-1) {
  unsigned long size = symbols.size();
  for (unsigned i = 0; i < size; ++i) {
    append(InputTerm(false, symbols[i]));
  }
}

InputClause::~InputClause() {}

InputClause::operator vec<Lit>&() {
  return literals;
}
InputClause::operator const vec<Lit>&() const {
  return literals;
}

void InputClause::clear(){
  maxVariable = -1;
  literals.clear();
}

void InputClause::append(InputTerm term) {
  int variable = term.getVariable();
  if (variable > maxVariable) {
    maxVariable = variable;
  }
  literals.push(term.isNegated() ? ~Lit(variable) : Lit(variable));
}

void InputClause::undoAppend() {
  literals.pop();
}


// ------------------------------------------------------------
// SATSolver
// ------------------------------------------------------------
void SATSolver::reserve(int variables) {
  while (variables >= solver.nVars()) {
    solver.newVar();
  }
}

void SATSolver::addClause(InputClause &clause) {
  reserve(clause.getMaxVariable());
  solver.addClause(clause);
  enable = true;
}

bool SATSolver::operator()(const InputClause &clause) {
  if (!enable) {
    return true;
  }
  reserve(clause.getMaxVariable());
  return solver.simplify() && solver.solve(clause);
}
