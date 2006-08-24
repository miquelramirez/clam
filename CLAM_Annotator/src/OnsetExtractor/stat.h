#include <stdio.h>
#include <assert.h>
#include <math.h>
  
class Stat {
	long cnt;     // number of values
	double sumx;  // sum of values
        double sumx2; // sum of squares of values
	double maxval;
      public:
	Stat();       // constructor
        void add(double);       // add a value
    	long n();     // get number of values
	double max();
	double sum(); // get sum of values
      	double mean();// get average
  	double stdev();         // get standard deviation
};
        
      // constructor
Stat::Stat()
{
    maxval = 0;
    cnt = 0;
    sumx = 0.0;
    sumx2 = 0.0;
}
      
    // add a value
void Stat::add(double d)
{
     if (d>maxval) maxval = d;
     cnt++;
     sumx += d;
     sumx2 += d * d;
}
    
  // get number of values
long Stat::n()
{
    return cnt;
}

   // get sum of values
double Stat::sum()
{
    assert(cnt >= 1);
    return sumx;
}

  // get average value
double Stat::mean()
{
    assert(cnt >= 1);
    return sumx / double(cnt);
}

  // get standard deviation
double Stat::stdev()
{
    assert(cnt >= 2);
        
    return sqrt((double(cnt) * sumx2 - sumx * sumx) / (double(cnt) * double(cnt - 1)));
}

double Stat::max()
{
    return maxval;
}

/*
  // driver program
main()
{
     Stat st;
     int i;
    
     for (i = 1; i <= 10; i++)
       st.add(double(i));
  
     printf("n = %ld\n", st.n());
     printf("average = %.2f\n", st.mean());
     printf("standard deviation = %.2f\n", st.stdev());
        
    return 0;
}
*/ 

