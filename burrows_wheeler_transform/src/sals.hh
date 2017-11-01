#ifndef _SALS_HH_
#define _SALS_HH_

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

class sals {
  public:
    string seq;
    int n;
    int LEN;
    int BEGIN;
    int END;
    vector<int> SA;
    vector<int> ISA;
    vector<int> S;
    sals(string s);
    int get_dnum(int n,int d);
    void radix_sort(int begin,int end);
    int selectRandomPosition(int begin,int end);
    int selectMaxPosition(int begin,int end);
    int get_int(int len,int index);
    void Ternary_Split_Quick_Sort(int begin,int end,int len,int f_r);
    void doubling(int begin,int end,int len);
    void construct_SA();
    void console_output();
    void file_output(string filename);
    ~sals() {}
}; 

#endif
