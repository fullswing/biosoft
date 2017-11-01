#include "bwt.hh"

bwt::bwt(string s,string W) : sals(s) {
  w = W;
  vector<char> tmp0(n);
  vector<int> tmp1(5,0);
  vector<vector<int> > tmp2(4,vector<int>(n,0));
  BWT = tmp0;
  C = tmp1;
  Occ = tmp2;
}

int base_to_int(char c) {
  if(c == '$') { return 0; }
  else if(c == 'a' || c == 'A') { return 1; }
  else if(c == 'c' || c == 'C') { return 2; }
  else if(c == 'g' || c == 'G') { return 3; }
  else if(c == 't' || c == 'T') { return 4; }
  return -1;
}

void bwt::construct_BWT() {
  for(int i = 0; i < n; i++) {
    int tmp = SA[i]-1;
    if(tmp >= 0) {
      BWT[i] = seq[tmp];
      int x = base_to_int(seq[tmp]);
      if(x<4) { C[x+1]++; }
    } else {
      BWT[i] = '$';
      C[1]++;
    }
  }
  /*
  for(int i=0;i<n;i++) {
    cout<<setw(4)<<i<<setw(2)<<BWT[i]<<endl;
  }
  cout<<"C"<<endl;
   */
  for(int i=0;i<5;i++) {
    if(i>0) { C[i]+=C[i-1]; }
//    cout<<setw(3)<<C[i];
  }
//  cout<<endl;
}

//Occには'$'がないので2次元配列と対応させる際t-1のようになっている
void bwt::construct_Occ() {
  Occ[base_to_int(BWT[0])-1][0]++;
  for(int i = 1; i < n; i++) {
    int t = base_to_int(BWT[i]);
    if(t != 0) { Occ[t-1][i]++; }
    for(int x = 0; x < 4; x++) {
      Occ[x][i] += Occ[x][i-1];
    }
  }
  /*
  cout<<"Occ"<<endl;
  for(int i=0;i<n;i++) {
    for(int x=0;x<4;x++) {
      cout<<setw(3)<<Occ[x][i];
    }
    cout<<endl;
  }
   */
}

void bwt::suffix_output() {
  if(lower_bound <= upper_bound) {
    for(int i = lower_bound; i <= upper_bound; i++) {
      cout << setw(3) << SA[i] << " " << seq.substr(SA[i],seq.size()-SA[i]) << endl;
//      cout<<setw(3)<<SA[i]<<endl;
    }
  } else {
    reverse(w.begin(),w.end());
    cout << "Match for "<< w << " was not found." << endl;
  }
}

void bwt::search_prefix() {
  reverse(w.begin(),w.end());
  int l=w.size()+1;
  int lb[l];
  int ub[l];
  lb[0] = 0;
  ub[0] = n-1;
  for(int i = 1; i < l; i++) {
    string tmp = w.substr(0,i);
    if(lb[i-1] == 0) {
      lb[i] = C[base_to_int(w[i-1])];
    } else {
      lb[i] = C[base_to_int(w[i-1])] + Occ[base_to_int(w[i-1])-1][lb[i-1]-1];
    }
    ub[i] = C[base_to_int(w[i-1])] + Occ[base_to_int(w[i-1])-1][ub[i-1]]-1;
  }

  lower_bound = lb[l-1];
  upper_bound = ub[l-1];
  suffix_output();
//  cout<<"lb:"<<lb[l-1]<<" ub:"<<ub[l-1]<<endl;
}

void bwt::search() {
  construct_SA();
  construct_BWT();
  construct_Occ();
  clock_t sTime,eTime;
  sTime=clock();
  search_prefix();
  eTime=clock();
  printf("Elapsed time = %f sec.\n",(float)(eTime-sTime)/(float)CLOCKS_PER_SEC);
}
