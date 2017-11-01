#include "chaining.hh"
#include "chaining.cc"

int main(int argc,char** argv) {
  if(argc!=2) {
    cout<<"Usage:"<<argv[0]<<" (input filename)"<<endl;
    return -1;
  }
  chaining C(argv[1]);
  C.execute();
}
