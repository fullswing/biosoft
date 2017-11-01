#ifndef _CHAINING_HH_
#define _CHAINING_HH_

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

typedef struct Align {
  int sx;
  int ex;
  int sy;
  int ey;
  int score;
} alignment;

typedef struct Chain {
  int p;
  int isEnd;
  alignment A;
} chain;

class chaining {
  public:
    vector<chain> X;
    vector<chain> X_buf;
    multimap<int,chain> Y;
    vector<int> from_x,to_x,from_y,to_y;
    chaining() {};
    chaining(string filename);
//    bool my_compare(chain tmp1,chain tmp2);
    void calc_chain();
    int chain_index();
    void print_chain();
    void traceback();
    void execute();
    ~chaining() {};
};

#endif
