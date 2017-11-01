#include "sals.hh"

using namespace std;

sals::sals(string s) {
  seq=s;
  seq+='$';
  n=seq.size();
  vector<int> tmp(n,0);
  SA=tmp;
  S=tmp;
  //initiallize
  for(int i=1;i<=n;i++) {
    SA[i]=i-1;
  }
  SA[0]=n-1;
  LEN=1;
  BEGIN=1;
  END=n-1;
}

//varはsuffixをintに変換したもの
//d桁目の数字を返す
int sals::get_dnum(int var,int d) {
  return (int)(var/pow(10,d))%10;
}

void sals::radix_sort(int begin,int end) {
  //indexについてradixsort
  int d=0;
  int D=0;
  int tmp_n=n+1;
  while (tmp_n!=0) {
    D++;
    tmp_n/=10;
  }
  vector<int> trg(end-begin+1);

  do {
    int bucket[10]={};
    int a[10]={};
    for(int i=begin;i<=end;i++) {
      a[get_dnum(SA[i],d)]++;
    }

    for(int i=1;i<10;i++) {
      bucket[i]=a[i-1]+bucket[i-1];
    }

    for(int i=begin;i<=end;i++) {
      int b=get_dnum(SA[i],d);
      trg[bucket[b]]=SA[i];
      bucket[b]++;
    }

    for(int i=begin;i<=end;i++) {
      SA[i]=trg[i-begin];
    }
    d++;
  } while(d<=D-1);
}

//Fixme:時間をseedにしているため運が悪いと落ちる
int sals::selectRandomPosition(int begin,int end) {
  srand((unsigned)time(NULL));
  int length=end-begin+1;
  int r=begin+rand()%length;
  return r;
}

int sals::selectMaxPosition(int begin,int end) {
  int m=S[begin];
  int r=begin;
  for(int i=begin+1;i<=end;i++) {
    if(m<S[i]) {
      m=S[i];
      r=i;
    }
  }
  return r;
}

//convert substring of sequence into integer.
int sals::get_int(int len,int index) {
  int n=0;
  for(int i=0;i<len;i++) {
    char tmp=seq[index+i];
    int s=pow(10,len-i-1);
    if(tmp=='$') { 
      n+=0;
      break;
    } else if(tmp=='a' || tmp=='A') { 
      n+=1*s; 
    } else if(tmp=='c' || tmp=='C') { 
      n+=2*s;
    } else if(tmp=='g' || tmp=='G') { 
      n+=3*s; 
    } else if(tmp=='t' || tmp=='T') { 
      n+=4*s; 
    }
  }
  return n;
}

void sals::Ternary_Split_Quick_Sort(int begin,int end,int len,int f_r) {
  for(int i=begin;i<=end;i++) {
    S[i]=get_int(len,SA[i]);
  }

  int random=selectRandomPosition(begin,end);
  if(random==f_r) {
    if(random-begin>end-random) {
      random-=1;
    } else {
      random+=1;
    }
  }
  
  swap(SA[end],SA[random]);
  swap(S[end],S[random]);
  int pivot=S[end];
  int i=begin-1;
  int j=end;
  int mj=end-1;
  int mi=begin;
  for(;;) {
    for(i++;S[i]<=pivot && i<end;i++) {
      if (S[i]==pivot && i!=end) {
        swap(SA[i],SA[mi]);
        swap(S[i],S[mi]);
        mi++;
      }
    }
    for (j--;pivot<=S[j] && i<j;j--) {
      if (S[j]==pivot && j>begin) { 
        swap(SA[j],SA[mj]);
        swap(S[j],S[mj]);
        mj--;
      }
    }
    if(i>=j) { break; }
    swap(SA[i],SA[j]);   
    swap(S[i],S[j]);   
  }
  if(i==j && j>begin) {
    j--; 
  }

  while(mi>begin) { 
    swap(SA[mi-1],SA[j]);
    swap(S[mi-1],S[j]);
    mi--;
    if (j>begin) {
      j--;
    }
  }
  while (mj<end && i<end) {
    swap(SA[mj+1],SA[i]);
    swap(S[mj+1],S[i]);
    mj++;
    i++;
  }
  if(i<end) {
    Ternary_Split_Quick_Sort(i,end,len,random);
  }
  if(j>begin) {
    Ternary_Split_Quick_Sort(begin,j,len,random);
  }
}

//given:sequence,suffixarraay(to be constructed),begin and end index,length in doubling
void sals::doubling(int begin,int end,int len) {
  if(len>=n) { return; }
  Ternary_Split_Quick_Sort(begin,end,len,-1000);
  //depth first 
  for(int i=begin;i<=end;i++) {
    string tmp1=seq.substr(SA[i],len);
    string tmp2=seq.substr(SA[i+1],len);
    if(tmp1!=tmp2){
      if(begin!=i) {
//        radix_sort(begin,i);
      }
      doubling(begin,i,2*len);
      begin=i+1;
    }
  }
  return;
}

void sals::construct_SA() {
  doubling(BEGIN,END,LEN);
}

void sals::console_output() {
  for(int i=0;i<n;i++) {
    cout<<setw(4)<<i<<" "<<setw(4)<<SA[i]<<" ";
    cout<<seq.substr(SA[i],seq.size()-SA[i])<<endl;
  }
}

void sals::file_output(string filename) {
  ofstream writing_file;
  writing_file.open(filename,ios::out);
  cout<<"suffix array has been constructed in "<<filename<<endl;
  writing_file<<"seq:"<<seq<<endl;
  for(int i=0;i<n;i++) {
    writing_file<<setw(4)<<i<<" "<<setw(4)<<SA[i]<<" ";
    writing_file<<seq.substr(SA[i],seq.size()-SA[i])<<endl;
  }
}
