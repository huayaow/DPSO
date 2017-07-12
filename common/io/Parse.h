#ifndef PARSE_H
#define PARSE_H

#include <string>
using namespace std;

extern string  modelFile ;
extern string  constraintFile ;
extern string  outputFile ;
extern string  algorithm ;
extern int     repeat;

void setOptions(int argc, char **argv);
void showOptions();

#endif
