#include "bwt.hh"
#include "bwt.cc"

int main(int argc,char** argv) {  
  if(argc!=2) {
    cerr<<"Usage:"<<argv[0]<<" (input filename)"<<endl;
    return -1;
  }

  string seq;
  ifstream ifs(argv[1]);
  if(!ifs) {
    cerr<<"Failed to read file:"<<argv[1]<<endl;
    cerr<<"Confirm the filename."<<endl;
    return -1;
  }
  getline(ifs,seq);
//  cout<<seq<<endl;

  string w;
  cout<<"prefix?:";
  cin>>w;
  
  bwt B(seq,w);
  B.search();

  return 0;
}
