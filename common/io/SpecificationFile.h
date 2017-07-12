#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <string>

class SpecificationFile {

public:
  int parameter;
  int* value;
  int tway;

  SpecificationFile(const std::string &filename);
};

#endif
