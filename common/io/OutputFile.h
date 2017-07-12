#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H

#include <string>
#include <vector>
#include "../SUT.h"

class OutputFile {

public:
  OutputFile() {
    size = ULONG_MAX;
    time = ULLONG_MAX;
  }
  ~OutputFile() {
    if (array.size() != 0) {
      for_each(array.begin(), array.end(), default_delete<int>());
      array.clear();
    }
  }
  unsigned long size ;
  unsigned long time ;

  void update(const std::vector<int*> &arr, int para, unsigned long tm);
  void write(const SUT* sut, const std::string &filename);

private:
  std::vector<int*> array ;

};

#endif
