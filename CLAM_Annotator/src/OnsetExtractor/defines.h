#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

#define mapa2 map<int, double, ltint>
#define itermapa2 map<int, double, ltint>::iterator

#define peaksvector vector<mapa2>
#define iterpeaksvector vector<mapa2>::iterator

#define myvector vector<spectralpattern>
#define itermyvector vector<spectralpattern>::iterator

#define peaksattvector vector<peaksatt>
#define iterpeaksattvector vector<peaksatt>::iterator

#define notepair pair<int,double>
#define notesvector vector<notepair>

#define vectorchords vector<chord>

#define hashset set<int>
typedef std::vector< std::vector<double> > Tmatrix;

//#define mapa map<double, int, ltdoub>
//#define itermapa map<double, int,ltdoub>::iterator
//#define spset multiset<spectralpattern,ltsp> 
//#define iterspset multiset<spectralpattern,ltsp>::iterator





/*--------------------------------------------

 Ordering of integers in a hash_set

---------------------------------------------*/

struct ltint
{
  bool operator()(const int i1, const int i2) const
  {
    return i1<i2;
  }
};

/****
 NOT USED!
***/

struct ltdoub
{
  bool operator()(const double s1, const double s2) const
  {
    return s1>s2;
  }
};

/*----------------------------------------------

  Ordering of spectral patterns (probability ordering) NOT USED!!!!

----------------------------------------------*/

//struct ltsp
//{
//  bool operator()(const spectralpattern sp1, const spectralpattern sp2) const
//  {
//  	return sp1.prob>sp2.prob;
//  }
//};


#endif
