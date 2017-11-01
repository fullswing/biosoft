#include "chaining.hh"

bool my_mapcompare(pair<int,chain> &tmp1,pair<int,chain> &tmp2) {
  return (tmp1.first<tmp2.first);
}

bool my_compare(chain tmp1,chain tmp2) {
  return (tmp1.p<tmp2.p);
}

chaining::chaining(string filename) {
  ifstream file(filename.c_str());
  if(file.fail()) {
    cout<<"Couldn't open file"<<endl;
  }
  string buf;
  int i=0;
  while(getline(file,buf,'\t')) {
    alignment a;
    chain tmp;
    if(i%5==0) { 
      a.sx=atoi(buf.c_str()); 
    } else if(i%5==1) { 
      a.ex=atoi(buf.c_str()); 
    } else if(i%5==2) { 
      a.sy=atoi(buf.c_str());
    } else if(i%5==3) { 
      a.ey=atoi(buf.c_str());
    } else if(i%5==4) { 
      a.score=atoi(buf.c_str()); 
    }
    i++;
   
  if(i>0 && i%5==0) {
      tmp.A=a;
      tmp.p=a.sx;
      tmp.isEnd=false;
      X.push_back(tmp);
      tmp.p=a.ex;
      tmp.isEnd=true;
      X.push_back(tmp);
    }
  }
  sort(X.begin(),X.end(),my_compare);
  X_buf=X;
}


void chaining::calc_chain() {
  for(vector<chain>::iterator x=X.begin();x!=X.end();++x) {
    if(x->isEnd) {
      //x.Aより悪いアラインメントをすべてYから除く
      bool b=true;
      for(multimap<int,chain>::iterator it=Y.begin();it!=Y.end();++it) {
        if(x->A.ey<=it->second.A.ey && x->A.score>it->second.A.score) {
          Y.erase(it);
        } else if(it->second.A.ey<=x->A.ey && it->second.A.score>=x->A.score) {
          b=false;
        }
      }
      if(b==true) {
        chain tmp;
        tmp.p=x->p;
        tmp.isEnd=x->isEnd;
        tmp.A=x->A;
        Y.insert(pair<int,chain>(tmp.p,tmp));
      }
//      sort(Y.begin(),Y.end(),my_mapcompare);
    } else {
      int max_s=0;
      int max_sx=0;
      int max_sy=0;
      int tmp;
      int c=0;
      int b=false;
      vector<chain>::iterator tmp_x=X.begin();
      //Yを探索
      for(multimap<int,chain>::iterator it=Y.begin();it!=Y.end();++it) {
        //C<Bとなる組みが見つかる
        if(x->A.sx>=it->second.A.ex && x->A.sy>=it->second.A.ey) {
          tmp_x=x;
          b=true;
          for(vector<chain>::iterator x_ = x+1;x_!=X.end();++x_) {
            if(x_->A.ex==x->A.ex && x_->A.ey==x->A.ey) {
              if (c==0) {
                //アラインメントのスコアを記憶
                tmp=x_->A.score;
                max_s=tmp+it->second.A.score;
                max_sx=it->second.A.sx;
                max_sy=it->second.A.sy;
                c++;
              } else {
                x_->A.score=tmp;
                int tmp_sx=it->second.A.sx;
                int tmp_sy=it->second.A.sy;
                int tmp_s=tmp+it->second.A.score;
                if(tmp_s>max_s) { 
                  max_sx=tmp_sx;
                  max_sy=tmp_sy;
                  max_s=tmp_s;
                }
                break;
              }
            }
          }
        }
      }
      if(b) {
        for(vector<chain>::iterator x_=tmp_x+1;x_!=X.end();++x_) {
          if(x_->A.ex==x->A.ex && x_->A.ey==x->A.ey) {
            x_->A.sx=max_sx;
            x_->A.sy=max_sx;
            x_->A.score=max_s;
          }   
        }
      }
    }
  }
}

int chaining::chain_index() {
  int i=0;
  int max=0;
  for(multimap<int,chain>::iterator it=Y.begin();it!=Y.end();++it) {
    int tmp=it->second.A.score;
    if(tmp>max) { max=tmp; }
  }
  for(vector<chain>::iterator x=X.begin();x!=X.end();++x) {
    if(x->isEnd && x->A.score==max) { break; }
    i++;
  }
  return i;
}

void chaining::traceback() {
  int index=chain_index();
  from_x.push_back(X_buf[index].A.sx);
  to_x.push_back(X_buf[index].A.ex);
  from_y.push_back(X_buf[index].A.sy);
  to_y.push_back(X_buf[index].A.ey);
  int s=X[index].A.score-X_buf[index].A.score;
  int n=X.size();
  int f_index=index;
  for(int j=n-1;j>=0;--j) {
    chain tmp1=X[j];
    chain tmp2=X_buf[f_index];
    chain tmp3=X_buf[j];
    if(tmp1.isEnd && tmp1.A.ex<=tmp2.A.sx && tmp1.A.ey<=tmp2.A.sy && s==tmp1.A.score) {
      from_x.push_back(tmp3.A.sx);
      from_y.push_back(tmp3.A.sy);
      to_x.push_back(tmp3.A.ex);
      to_y.push_back(tmp3.A.ey);
      f_index=j;
      s-=tmp3.A.score;
    }
  }
  reverse(from_x.begin(),from_x.end());
  reverse(from_y.begin(),from_y.end());
  reverse(to_x.begin(),to_x.end());
  reverse(to_y.begin(),to_y.end());
  for(int j=0;j<from_x.size();j++) {
    cout<<j<<"th alignment is from ("<<from_x[j]<<","<<from_y[j]<<") to ("<<to_x[j]<<","<<to_y[j]<<")"<<endl;
  }
  cout<<"Total score:"<<X[index].A.score<<endl;
}

void chaining::print_chain() {
  cout<<"X_buf"<<endl;
  for(vector<chain>::iterator it=X_buf.begin();it!=X_buf.end();++it) {
    cout<<"isEnd:"<<setw(3)<<it->isEnd<<" p:"<<setw(3)<<it->p
      <<" sx:"<<setw(3)<<it->A.sx<<" ex:"<<setw(3)<<it->A.ex
      <<" sy:"<<setw(3)<<it->A.sy<<" ey:"<<setw(3)<<it->A.ey
      <<" score:"<<setw(3)<<it->A.score<<endl;
  }
    
  cout<<"X"<<endl;
  for(vector<chain>::iterator it=X.begin();it!=X.end();++it) {
    cout<<"isEnd:"<<setw(3)<<it->isEnd<<" p:"<<setw(3)<<it->p
      <<" sx:"<<setw(3)<<it->A.sx<<" ex:"<<setw(3)<<it->A.ex
      <<" sy:"<<setw(3)<<it->A.sy<<" ey:"<<setw(3)<<it->A.ey
      <<" score:"<<setw(3)<<it->A.score<<endl;
  }

  cout<<"Y"<<endl;
  for(multimap<int,chain>::iterator it=Y.begin();it!=Y.end();++it) {
    cout<<"sx:"<<setw(3)<<it->second.A.sx<<" ex:"<<setw(3)<<it->second.A.ex
      <<" sy:"<<setw(3)<<it->second.A.sy<<" ey:"<<setw(3)<<it->second.A.ey
      <<" score:"<<it->second.A.score<<endl;
  }
}

void chaining::execute() {
  calc_chain();
  traceback();
}
