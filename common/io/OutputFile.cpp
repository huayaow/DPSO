#include "OutputFile.h"
#include <fstream>
using namespace std;

void OutputFile::update(const std::vector<int*> &arr, int para, unsigned long tm) {
  if (arr.size() < size) {
    size = arr.size();
    time = tm;
    if (array.size() != 0) {
      for_each(array.begin(), array.end(), default_delete<int>());
      array.clear();
    }
    for (auto x : arr) {
      int *test = new int[para];
      for (int v = 0; v < para; v++)
        test[v] = x[v];
      array.push_back(test);
    }
  }
}

void OutputFile::write(const SUT *sut, const std::string &filename) {
  ofstream file;
  file.open(filename);

  if (file.is_open()) {
    file << "PARAMETER " << sut->parameter << endl;
    file << "VALUE ";
    for (int k = 0; k < sut->parameter; k++)
      file << sut->value[k] << " ";
    file << endl;
    file << "COVERING STRENGTH " << sut->tway << endl;

    file << "SIZE " << array.size() << endl;
    file << "TIME " << time << endl;
    for (auto i : array) {
      for (int v = 0; v < sut->parameter; v++)
        file << i[v] << " ";
      file << endl;
    }
    file << "END" << endl;
    file.close();
  }
}

