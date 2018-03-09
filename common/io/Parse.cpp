#include "Parse.h"
#include <getopt.h>
#include <iostream>
using namespace std;

const char *const short_opts = "c:o:a:r:vh";

const option long_opts[] = {
  {"constraint", required_argument, nullptr, 'c'},
  {"output",     required_argument, nullptr, 'o'},
  {"algorithm",  required_argument, nullptr, 'a'},
  {"repeat",     required_argument, nullptr, 'r'},
  {"version",    no_argument,       nullptr, 'v'},
  {"help",       no_argument,       nullptr, 'h'},
  {nullptr,      no_argument,       nullptr, 0}
};

static const string supported[] = {
  "CPSO", "CTVAC", "CCLPSO", "CAPSO", "CDMSPSO",
  "DPSO", "DTVAC", "DCLPSO", "DAPSO", "DDMSPSO"
};

static const char *version = "1.0";
static const char *program_name = "DPSO";
static const char *program =
  "Generate (constrained) covering arrays by conventional and discrete "
  "particle swarm optimization algorithms.\n\n";

static const char *doc =
  "  -c, --constraint  [FILE]  the file to indicate constraints\n"
  "  -o, --output      [FILE]  the file to store generated covering array\n"
  "  -a, --algorithm   [ALG]   the particular PSO algorithm to be used\n"
  "  -r, --repeat      [COUNT] set the number of repetitions\n"
  "  -v, --version             show current version and exit\n"
  "  -h, --help                show help and exit\n\n";

static const char *usage = "Usage: DPSO [OPTIONS] [MODEL_FILE]\n\n";

string modelFile = "";
string constraintFile = "";
string outputFile = "array.txt";
string algorithm = "DPSO";
int repeat = 1;

/**
 * Parse program arguments and save them into variables.
 * @param argc
 * @param argv
 */
void setOptions(int argc, char **argv) {
  int index = 0;
  while (true) {
    auto opt = getopt_long(argc, argv, short_opts, long_opts, &index);
    switch (opt) {
      case 'c':
        constraintFile = optarg;
        break;
      case 'o':
        outputFile = optarg;
        break;
      case 'a': {
        algorithm = optarg;
        const string *f = find(begin(supported), end(supported), algorithm);
        if (f == end(supported)) {
          cout << usage << endl;
          exit(1);
        }
        break;
      }
      case 'r':
        repeat = atoi(optarg);
        break;
      case 'v':
        cout << program_name << " version " << version << endl;
        break;
      case 'h':
        cout << program_name << " version " << version << endl;
        cout << program << doc << usage << endl;
        exit(1);
      case '?':
        cout << usage << endl;
        exit(1);
      default:
        if (argc - optind < 1) {
          cout << usage << endl;
          exit(1);
        }
        if (argc - optind > 1) {
          cerr << "Warning: ignoring extraneous arguments after model file" << endl;
        }
        modelFile = argv[optind];
        return;
    }
  }
}

void showOptions() {
  cout << "---------------------------------------" << endl;
  cout << "model file      = " << modelFile << endl;
  cout << "constraint file = " << constraintFile << endl;
  cout << "output file     = " << outputFile << endl;
  cout << "algorithm       = " << algorithm << endl;
  cout << "repeat          = " << repeat << endl;
  cout << "---------------------------------------" << endl;
}