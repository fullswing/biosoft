#ifndef _BWT_HH_
#define _BWT_HH_

#include "sals.hh"
#include "sals.cc"

class bwt : public sals {
  public:
    vector<char> BWT;
    vector<int> C;
    vector<vector<int> > Occ;
    int lower_bound;
    int upper_bound;
    string w;
    bwt(string s,string W);
    int translate(char c);
    void construct_BWT();
    void construct_Occ();
    void suffix_output();
    void search_prefix();
    void search();
    ~bwt() {}
};

#endif
