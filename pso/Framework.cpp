#include"Framework.h"

// ----------------------------------------------------------------------------
// clear
// ----------------------------------------------------------------------------
void Framework::clear() {
  for_each(ARRAY.begin(), ARRAY.end(), default_delete<int>());
  ARRAY.clear();
}

// ----------------------------------------------------------------------------
// set parameter configurations
// ----------------------------------------------------------------------------
void Framework::setConfiguration(int par, int ite, double w, double f,
                                 double p1, double p2, double p3) {
  config.population = par ;
  config.iteration = ite ;
  config.weight = w ;
  config.factor = f ;
  config.pro1 = p1 ;
  config.pro2 = p2 ;
  config.pro3 = p3 ;
}

// ----------------------------------------------------------------------------
// copy array A to array B
// ----------------------------------------------------------------------------
void Framework::copy(vector<int *> A, vector<int *> &B, int para) {
  if (B.size() != 0) {
    for_each(B.begin(), B.end(), default_delete<int>());
    B.clear();
  }
  for (vector<int *>::iterator i = A.begin(); i != A.end(); i++) {
    int *test = new int[para];
    for (int v = 0; v < para; v++)
      test[v] = (*i)[v];
    B.push_back(test);
  }
}

// ----------------------------------------------------------------------------
// hamming distance
// ----------------------------------------------------------------------------
double Framework::hammingDist(const int *test) {
  int hamming = 0;
  if (ARRAY.size() == 0)
    return 0;

  for (vector<int *>::const_iterator i = ARRAY.begin(); i != ARRAY.end(); i++) {
    int ham = 0;
    for (int k = 0; k < sut->parameter; k++) {
      if (test[k] != (*i)[k])
        ham++;
    }
    hamming += ham;
  }
  double h = (double) hamming / (double) ARRAY.size();
  return h;
}

// ----------------------------------------------------------------------------
// covering array generation
// ----------------------------------------------------------------------------
void Framework::PSOEvolve() {
  clear();

  // set the function to check constraint
  checkValidity = bind(static_cast<bool(SUT::*)(const int*)>(&SUT::isValid),
                       sut, placeholders::_1);

  // initialize all combinations to be covered
  sut->initializeCombination();

  time_t time_1 = time(0);
  while (sut->SCount != 0) {
    // generate a best one that covers as more combinations as possible
    int *best = Evolve();
    // add it into test suite
    ARRAY.push_back(best);
    // update the covered combinations
    sut->fitnessValue(best, 1);
  }
  time_t time_2 = time(0);

  SIZE = ARRAY.size();
  TIME = static_cast<unsigned long>(time_2 - time_1);

  // if there has seed ...
  if (sut->seedInfo.size() != 0) {
    vector<int *> temp;
    copy(ARRAY, temp, sut->parameter);
    clear();
    copy(sut->seedInfo, ARRAY, sut->parameter);
    for (vector<int *>::iterator i = temp.begin(); i != temp.end(); i++)
      ARRAY.push_back((*i));
    SIZE = SIZE + sut->seedInfo.size();
  }
}

// ----------------------------------------------------------------------------
// print covering array to console
// ----------------------------------------------------------------------------
void Framework::printArray() {
  cout << "PARAMETER " << sut->parameter << endl;
  cout << "VALUE ";
  for (int k = 0; k < sut->parameter; k++)
    cout << sut->value[k] << " ";
  cout << endl;
  cout << "TWAY " << sut->tway << endl;
  cout << "SIZE " << SIZE << endl;
  cout << "TIME " << TIME << endl;
  for (vector<int *>::iterator i = ARRAY.begin(); i != ARRAY.end(); i++) {
    for (int v = 0; v < this->sut->parameter; v++)
      cout << (*i)[v] << " ";
    cout << endl;
  }
  cout << "END" << endl;
}
