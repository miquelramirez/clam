
#include <math.h>
#include "DataTypes.hxx"
#include "BasicStatistics.hxx"

namespace CLAM {



  TData Moment(TData* data,TSize size,Moments moments,TSize order ,bool abs) 
  {
    int i;
    TData t1;
    TData t2;

    TData s1 = 0;
    TData s2 = 0;
    TData s3 = 0;
    TData s4 = 0;
    TData s5 = 0;

    int num = order;

    for (i=0;i<size;i++) {
      if (!abs) t1 = data[i];
      else t1 = fabs(data[i]);
      s1 += t1;
      if (num == 1) continue;
      t2 = t1*t1;
      s2 += t2;
      if (num == 2) continue;
      t2 *= t1;
      s3 += t2;
      if (num == 3) continue;
      t2 *= t1;
      s4 += t2;
      if (num == 4) continue;
      t2 *= t1;
      s5 += t2;
    }
	
    moments[0] = (s1)/size;
    if (num == 1) return moments[0];
    moments[1] = (s2)/size;
    if (num == 2) return moments[1];
    moments[2] = (s3)/size;
    if (num == 3) return moments[2];
    moments[3] = (s4)/size;
    if (num == 4) return moments[3];
    moments[4] = (s5)/size;
    return moments[4];
    
  }

  TData Energy(Moments moments,TSize size)
  {
    return moments[1]*(TData)size;  // We have to rescale it
  }

  TData RMS(Moments moments,TSize size)
  {
    return sqrt(moments[1]*size);
  }

  TData Mean(Moments moments)
  {
    return moments[0];
  }


  TData CentralMoment(int order,Moments moments)
  {
    switch (order){
    case 1: return 0;
    
      /* -m1² + m2 */ 
    case 2: 
      {
	TData m1 = moments[0];
	TData m2 = moments[1];
	return (-1)*m1*m1 + m2;
      }
    
      /* 2*m1³ - 3*m1*m2 + m3 =   ... 5 Mult  
	 m1*(2*m1² - 3*m2) + m3   ... 4 Mult */
    case 3: 
      {
	TData m1 = moments[0];
	TData m2 = moments[1];
	TData m3 = moments[2];
	return m1*(2*m1*m1 - 3*m2) + m3;
      }
    
      /* -3*m1^4 + 6*m1²*m2 - 4*m1*m3 + m4   = ... 9 Mult 
	 m1*(m1*((-3)*m1² + 6*m2) - 4*m3) + m4 ... 6 Mult */
    case 4: 
      {
	TData m1 = moments[0];
	TData m2 = moments[1];
	TData m3 = moments[2];
	TData m4 = moments[3];
	return m1*(m1*((-3)*m1*m1 + 6*m2) - 4*m3) + m4;
      }
    
      /* 4*u1^5 - 10*u1³*u2 + 10*u1²*u3 - 5*u1*u4+u5    = .... 14 Mult
	 u1*(u1*(u1*(4*u1² - 10*u2) + 10*u3) - 5*u4) + u5 .... 8 Mult   */
    case 5: 
      {
	TData m1 = moments[0];
	TData m2 = moments[1];
	TData m3 = moments[2];
	TData m4 = moments[3];
	TData m5 = moments[4];

	return m1*(m1*(m1*(4*m1*m1 - 10*m2) + 10*m3) - 5*m4) + m5;
      }
    default:
      return -1;
    }
  }

  TData AverageDeviation(TData* data,TSize size) 
  {
    /* TODO */
    return 0;
  }

  TData StandardDeviation(Moments moments)
  { 
    return sqrt(CentralMoment(2,moments));
  }

  TData Variance(Moments moments)
  { 
    return CentralMoment(2,moments); 
  }

  TData Skew(Moments moments)
  { 
    TData dev = StandardDeviation(moments);
    TData m3 = CentralMoment(3,moments);
    if (dev == 0) return 0;
    return  m3/(dev*dev*dev);
  } 


  TData Kurtosis(Moments moments)
  { 
    TData var = Variance(moments);
    TData m4 = CentralMoment(4,moments);
    if (var == 0) return 0;
    return  m4/(var*var) - 3;
  }




  TData Centroid(TData* mag, 
		 TSize size,
		 TData scale)
  {
    TData moments;
    TData centr;
    Moment(mag,size,&moments,1,true);
    centr = CentroidMoment(mag,size,1,scale,&moments);
  
    return centr*scale;
  }

  TData CentroidMoment(TData*   mag, 
		       TSize    size,
		       TSize    order,
		       TData    scale,
		       Moments  moments,
		       Moments  cmoments) 
  {
    return CentroidMoment(mag,size,order,&scale,moments,cmoments,0);
  }


  TData CentroidMoment(TData*   mag, 
		       TSize    size,
		       TSize    order,
		       TData*   pos,
		       Moments  moments,
		       Moments  cmoments,
		       bool array) 
    
  {
    TData sum1 = 0.;
    TData sum2 = 0.;
    TData sum3 = 0.;
    TData sum4 = 0.;
    TData val;
    TData bval;
    TSize i;
  
    static TData  sMoments[4];
    TData* mom;

    /* Calculate Mean if there are no precalculated Moments */

    if (moments == NULL) {
      mom = sMoments;
      Moment(mag,size,mom,order);
    }
    else 
      mom = moments;
  
    for (i=0;i<size;i++)
      {
	bval = fabs(mag[i])* *pos;
	if (array) 
	  pos++;  // if there is an array of positions use that
	else
	  bval*= i;  // if not multiply with the index 
	sum1 += bval;
	if (order < 2) continue;
	val = bval*bval;
	sum2 += val;
	if (order < 3) continue;
	val *= bval;
	sum3 += val;
	if (order < 4) continue;
	val *= bval;
	sum4 += val;
      }

    sum1 /= mom[0]*size;
    if (cmoments) cmoments[0] = sum1; 
    if (order < 2) return sum1;

    sum2 /= mom[1]*size;
    if (cmoments) cmoments[1] = sum2;
    if (order < 3) return sum2;

    sum3 /= mom[2]*size;
    if (cmoments) cmoments[2] = sum3;
    if (order < 4) return sum3;

    sum4 /= mom[3]*size;
    if (cmoments) cmoments[3] = sum4;
    if (order < 5) return sum4;

    /* The order is too high, and we don't have a way to report errors */
    return 0;
  }



  TData SpectralTilt(TData* mag,
		     TSize size,TData scale,Moments moments)
  {

    /* TODO check me check me check me check me check me check me check me */
    TData m1;
    int i;

    TData d1=0;
    TData d2=0;
    TData ti=0;
    TData SumTi2 = 0;
    TData Tilt = 0;

    /* I should write down somewhere how this works .. or why it doesn't :) */

    if (moments == NULL) {
      Moment(mag,size,&m1,1);
    }
    else 
      m1 = moments[0];

    for (i=0;i<size;i++)
      {
	d1 += i/mag[i];
	d2 += 1/mag[i];
      }

    /* ti = m1/ai *(n - (d1/d2)) */
    /* SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]  */

    for (i=0;i<size;i++) {
      Tilt += (1/mag[i] *(i-d1/d2));
      ti = m1/mag[i]*(i - (d1/d2));
      SumTi2 += ti*ti;
    }

    Tilt*= (m1*m1/SumTi2);
    return Tilt;
  }


  TData SpectralTilt(TData* mag,
		     TSize size,TData* pos,Moments moments)
  {

    /* TODO check me check me check me check me check me check me check me */
    TData m1;
    int i;

    TData d1=0;
    TData d2=0;
    TData ti=0;
    TData SumTi2 = 0;
    TData Tilt = 0;

    /* I should write down somewhere how this works .. or why it doesn't :) */

    if (moments == NULL) {
      Moment(mag,size,&m1,1);
    }
    else 
      m1 = moments[0];

    for (i=0;i<size;i++)
      {
	d1 += pos[i]/mag[i];
	d2 += 1/mag[i];
      }

    /* ti = m1/ai *(n - (d1/d2)) */
    /* SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]  */

    for (i=0;i<size;i++) {
      Tilt += (1/mag[i] *(pos[i]-d1/d2));
      ti = m1/mag[i]*(pos[i] - (d1/d2));
      SumTi2 += ti*ti;
    }

    Tilt*= (m1*m1/SumTi2);
    return Tilt;
  }




  TData SpectralFlatnessMeasure(TData* mag,
				TSize size, Moments moments)
  {
    TData mean;
    int i;
    TData geomean = 1;


    if (moments == NULL) {
      Moment(mag,size,&mean,1);
    }
    else 
      mean = moments[0];

    for (i=0;i<size;i++)
      geomean *= mag[i];

    geomean = pow(geomean,1/(TData)size);
  
    return 10*log(geomean/mean);
  }



  TData HighFrequencyCoefficient(TData* mag , TSize size)
  {
	  int i;
	  TData temp = 0;
	  for (i=1;i<size;i++)
		  temp += pow(mag[i],2)*i; 
	  return temp;
  }
  

  TData TonalityCoefficient(TData sfm, TData sfmmax )
  {
    TData temp = sfm/sfmmax;
  
    return (temp < 1 ? temp : 1);  
  }




  TSize CepstrumCoefficients(TData* data, TSize size,TData* coeff,TSize num)
  {
    int i;

    for (i=0;i<size;i++) {
      coeff[i] = log(data[i]);
    }
      
    /* TODO a fft on coeff ... falta */
    
  return 0;    
  }


} /* namespace */


#ifdef TEST

#include <iostream>
#include <sys/timeb.h>
#include <stdlib.h>

using namespace MTG;

SUM_KERNEL(mean,out += X1[i])

int main(char** argv,int argc)
{

  struct timeb tv;
  int j;
  TData test[100];
  TData pos[100];
  int i;


  /* calculate sample data */

  ftime(&tv);
  int seed = tv.time;
  srand(seed);
  for (i=0;i<100;i++) {
    pos[i] = i;
    test[i] = 0;
    for (j=0;j<10;j++)
      test[i] += ((TData)rand())/RAND_MAX;
    test[i] = test[i] > 5 ? 9 : test[i]; 
    cout << test[i] << endl;  
  }


  Moments mom = new TData[4];
  Moments cmom = new TData[4];
  Moments pmom = new TData[4];


  /* 
   * The computations Section
   */

  Moment(test,100,mom);

  /* Centroid Computations */

  TData centr = Centroid(test,100);

  /* reuse Moments and store result in array */
  CentroidMoment(test,100,4,1.0,mom,cmom); 

  TData arr = 1.0;
  TData centr1 =   CentroidMoment(test,100,1,arr); /* easy but slower */
  TData centr2 =   CentroidMoment(test,100,2,arr); /* easy but slower */
  TData centr3 =   CentroidMoment(test,100,3,arr); /* easy but slower */
  TData centr4 =   CentroidMoment(test,100,4,arr); /* easy but slower */


  /* Centroids using additional position data */

  TData centr5 =   CentroidMoment(test,100,4,pos,mom,pmom);

  /* Spectral Tilt Test */

  TData tilt1 = SpectralTilt(test,100,1.0,mom); /* reuse Moments*/
  TData tilt2 = SpectralTilt(test,100,pos); /* easy but slightly slower */

  /* Spectral Flatness */

  TData flat1 = SpectralFlatnessMeasure(test,100,mom); /* reuse Moments*/
  TData flat2 = SpectralFlatnessMeasure(test,100); /* easy but sligthly slower */

  /* General Statistics computation */

  TData gg = Sum<mean>(100,test)/100;


  /* 
   *       The Ouput section 
   */

  cerr << "******************************************************" << endl;
  cerr << "*" << endl;
  cerr << "*                Basic Statistics Test" << endl;
  cerr << "*" << endl;
  cerr << "******************************************************" << endl << endl;

  cerr << "1st Moment = " << mom[0] << endl;
  cerr << "2nd Moment = " << mom[1] << endl;
  cerr << "3rd Moment = " << mom[2] << endl;
  cerr << "4th Moment = " << mom[3] << endl;

  cerr << "1st Central Moment = " << CentralMoment(1,mom) << endl;
  cerr << "2nd Central Moment = " << CentralMoment(2,mom) << endl;
  cerr << "3rd Central Moment = " << CentralMoment(3,mom) << endl;
  cerr << "4th Central Moment = " << CentralMoment(4,mom) << endl;
  cerr << "Standard Deviation = " << StandardDeviation(mom) << endl;
  cerr << "Variance = " << Variance(mom) << endl;
  cerr << "Skew = " << Skew(mom) << endl;
  cerr << "Kurtosis = " << Kurtosis(mom) << endl;

  cerr << "1st CentroidMoment = " << cmom[0] << " == " << centr1 
       << " == " << centr << endl;
  cerr << "2nd CentroidMoment = " << cmom[1] << " == " << centr2 
       << " == " << pmom[1] << endl;
  cerr << "3rd CentroidMoment = " << cmom[2] << " == " << centr3
       << " == " << pmom[2] << endl;
  cerr << "4th CentroidMoment = " << cmom[3] << " == " << centr4
       << " == " << pmom[3] << endl;
  cerr << "Flatness = " << flat1 << " == " << flat2 << endl;
  cerr << "Tilt = " << tilt1 << " == " << tilt2 << endl;

  cerr << "5thmoment = " << gg << endl;
}

#endif
