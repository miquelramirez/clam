/*   Clam Copyright (C)  */


#include "Polynomial.hxx"
#include <CLAM/ProcessingData.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/CLAM_Math.hxx>    


namespace CLAM
{

	//I'm not sure what this does, just copying from similar classes
	void Polynomial::DefaultInit()
	{
		//AddAll();
		UpdateData();
	}

	//solves the roots of the polynomial
	//based on an algorithm implemented in CPAN Polynomial::Solve by John M. Gamble
	//which was based on a perl binding of GNU Scientific library by Nick Ing-Simmons
	//which was based on a a fortran implementation of the QR Hessenberg algorithm by Hiroshi Murakami
	//References:
	//R. S. Martin, G. Peters and J. H. Wilkinson, "The QR Algorithm for Real Hessenberg Matrices", Numer. Math. 14, 219-231(1970).
	//B. N. Parlett and C. Reinsch, "Balancing a Matrix for Calculation of Eigenvalues and Eigenvectors", Numer. Math. 13, 293-304(1969).
	//Alan Edelman and H. Murakami, "Polynomial Roots from Companion Matrix Eigenvalues", Math. Comp., v64,#210, pp.763-776(1995).
	//Numerical Recipes in C, by William Press, Brian P. Flannery, Saul A. Teukolsky, and William T. Vetterling, Cambridge University Press (general reference).
	//Forsythe, George E., Michael A. Malcolm, and Cleve B. Moler (1977), Computer Methods for Mathematical Computations, Prentice-Hall (general reference).
	CLAM::Array <Complex> Polynomial::PolyRoots(DataArray coefficients) {
    
		Array<Complex> roots;  //an array to store the roots
    
		CLAM_ASSERT(coefficients.Size() > 0, "You gave PolyRoots() a polynomial with all zero coefficients.  ");

		//check for zero valued coefficients in the higher power terms
		//(not be needed for use w/ LPC formants, but just in case of future uses)
		while(Abs(coefficients[0]) < epsilon){ 
			coefficients.DeleteElem(0);
		}
    
		//check for zero valued coefficients in the lower powered terms
		while(Abs(coefficients[coefficients.Size()-1]) < epsilon ) {
			coefficients.DeleteElem(coefficients.Size()-1);
			roots.AddElem(0);
		}
    

		//build companion matrix
		BuildCompanion(coefficients);  //I made m, the companion matrix, a member variable instead of passing it

		//find eigenvalues of real upper hessenberg matrix
		roots = EigenHessenberg();  //this is removing a root if there was one from the while loop above

		return roots;
	}
  
  
	//cf, http://en.wikipedia.org/wiki/Companion_matrix
	//cf http://search.cpan.org/src/JGAMBLE/Math-Polynomial-Solve-2.11/lib/Math/Polynomial/Solve.pm
	void Polynomial::BuildCompanion(DataArray coefficients) 
	{
		//divide by leading coefficient:
		int co1 = coefficients[0];
		for ( int j = 0; j < coefficients.Size() ; j++ ) {
			coefficients[j]/=co1;
		}
		coefficients.DeleteElem(0);//if this is uncommented, it will be more like the example perl algorithms

		//resize matrix
		M.resize(coefficients.Size());
		for (int i=0; i<coefficients.Size(); i++)
			M[i].resize(coefficients.Size());

		//zero out matrix
		for(int i=0; i<M.size();i++)
			for(int j=0; j<M[0].size();j++)
				M[i][j] = 0;
	
		//setup sub diagonal matrix
		for (int i = 1; i < M.size(); i++) {
			M[i][i-1] =  1;
		}

		//put the coefficients into the last row:
		for (int i = 0; i < M.size(); i++) {
			M[i][M[i].size()-1] = - coefficients[M.size()-i-1]; 
		}
    
		//balancing the unsymmetric matrix:  //possible improvement: use LAPACK interface
		int notConverged=1;
		while (notConverged==1) {
			notConverged = 0;
			for(int i=0;i<M.size();i++) //go down the subdiagonal
			{
				double c;
				if(i != M.size()-1)  // if the index is not in the last row
				{
					c =  Abs(M[i+1][i]);     // (m at (i+1,i) is the subdiagonal)
				}
				else      // at the last row
				{
					c = 0;
					for(int j=0; j < M.size()-1; j++){   //add up the last column  
						c += Abs(M[j][M.size()-1]);
					}
				}
				double r;
				if(i==0)                                       // if at first row
				{
					r = Abs(M[0][M.size()-1]);   
				}
				else if(i!=M.size()-1)              // if at last row
				{
					r = Abs(M[i][i-1]) + Abs(M[i][M.size()-1]);
				}
				else                                           // if in the middle rows
				{
					r = Abs(M[i][i-1]);
				}
				if (c == 0 || r == 0) 
					continue;
	  
				double g = r/2;  //2 is given as the base of floating point represetnations.  Not sure why...
				double f = 1;
				double s = c+r;
				while(c<g)
				{
					f*=2;       //2 as "base"
					c*=4;       //4 as "base"^2
				}
				g = r*2;
				while(c>=g)
				{
					f/=2;
					c/=4;
				}
	  
				if( (c+r) < 0.95 * s * f) 
				{
					g = 1/f;
					notConverged = 1;

					//what the following code does:
					//for j=0:end m[i,j] *= g
					//for j=0:end m[j,i] *= f
					if(i==0) 
					{
						M[0][M[0].size()-1] =  M[0][M[0].size()-1] *g ;
					}
					else
					{
						M[i][i-1] =  M[i][i-1] *g ;
						M[i][M[i].size()-1] =  M[i][M[i].size()-1]*g ;
					}
	      
					if (i!=M.size()-1)
					{
						M[i+1][i] =  M[i+1][i] * f ;		  
					}
					else
					{
						for(int j=0; j<M.size(); j++)
							M[j][i] =  M[j][i]*f;
					}
	      
				} //if (c+r) < ...
			} //for(int i....

		} //while(notConverged)
	}

	Array<Complex> Polynomial::EigenHessenberg() 
	{
		double p,q,r;
		double w,x,y;
		double s,z;
		double t = 0.0;
    
		int n = M.size()-1;

		Array<Complex> roots;
    
	ROOT:
		while (n>=0) 
		{
			unsigned int iterations = 0;
			int na = n-1;
			const unsigned int maxIter = 60; //max # of iterations
			while( iterations < maxIter ) 
			{
	    
				//look for small subdiagonal element:
				unsigned int l;  // note l is lowercase L
				for (l = n; l >= 1; l--) 
				{
					if (Abs( M[l][l-1] ) <= epsilon * ( Abs( M[l-1][l-1]) + Abs(M[l][l]) ) )
						break;
				}
				x = M[n][n];
	    
				if(l==n) //one real root found
				{
					roots.AddElem(x+t);
					n--;
					goto ROOT;
				}
	    
				y = M[na][na];
				w = M[n][na] * M[na][n];
	    
				if(l == na)
				{
					p = (y-x) / 2;
					q = p*p + w;
					y = sqrt( Abs(q) );
					x += t;
	    
					if(q>0) //real pair 
					{
						if (p < 0) y = -y;
						y += p;
						roots.AddElem( x - w/y );
						roots.AddElem( x + y );
					}
					else //complex or twin pair
					{
						roots.AddElem( Complex(x + p, - y)   );
						roots.AddElem(  Complex(x + p,  y ) );
					}

					n -= 2;
					goto ROOT;
				}
	    
				if (iterations == maxIter) std::cerr<<"Too many iterations: "<<iterations<<" iterations @ n="<<n<<std::endl;

				//exceptional shift
				if((iterations>1) && (iterations % 10  == 0) ) 
				{
					t+=x;
					for(unsigned int i =0; i<=n; i++) 
					{
						M[i][i] -= x;
					}		
					s = Abs(M[n][na]) + Abs(M[na][n-2]);
					y = 0.75*x;
					x = y;
					w = -0.4375 * s*s;
				}
				iterations++;
	    
				//look for 2 consecutive small subdiagonal units
				unsigned int m;
				for(m=n-2; m >=l; m--) 
				{
					z = M[m][m];
					r = x - z;
					s = y - z;
					p = (r*s - w) / M[m+1][m] + M[m][m+1];
					q = M[m+1][m+1] - z - r - s;
					r = M[m+2][m+1];
		
					s = Abs(p) + Abs(q) + Abs(r);
					p /= s;
					q /= s;
					r /= s;
 
					if(m == l) 
						break;  //from for(m=n-1; ...
					if( Abs(M[m][m-1]) * ( Abs(q)+Abs(r) ) <=
					    epsilon  * Abs(p)  * ( Abs(M[m-1][m-1]) + Abs(z) + Abs(M[m+1][m+1]) )  )
						break; //from for(m=n-1; ...
				} //for(m=n-1; ...
	    
				for (unsigned int i = m + 2; i <= n; i++)
				{
					M[i][i-2] = 0.0;
				}
				for (unsigned int i = m+3; i <= n; i++)
				{
					M[i][i-3] =  0.0;
				}
	    
				//Double QR step for rows L-n and cols m-n 
				for(unsigned int k = m; k <= na; k++) 
				{
					bool notLast = (k!=na);
					if(k!=m)
					{
						p = M[k][k-1];
						q = M[k+1][k-1];
						r = notLast ? M[k+2][k-1] : 0;
		    
						x = Abs(p) + Abs(q) + Abs(r);
						if (x==0)
							break;

						p /= x;
						q /= x;
						r /= x;
					}
		
					s = sqrt(p*p + q*q + r*r);
					if(p<0)
						s = -s;

					if(k != m) 
						M[k][k-1] =  -s*x ;
					else if (l != m)
						M[k][k-1] =  M[k][k-1]*-1 ;
		
					p += s;
					x = p/s;
					y = q/s;
					z = r/s;
					q /= p;
					r /= p;

					//Row modification
					for (unsigned int j = k; j <= n; j++)
					{
						p = M[k][j] +  q * M[k+1][j];
		    
						if(notLast)
						{
							p += r * M[k+2][j];
							M[k+2][j] =  M[k+2][j] - p*z;
						}
		    
						M[k+1][j] =  M[k+1][j] - p*y;
						M[k][j] =  M[k][j] - p*x;
					}
		
					int j = k+3;
					if(j>n)
						j=n;
		
					//Column modification
					for(int i = l; i <= j; i++) 
					{
						p = x * M[i][k] + y * M[i][k+1];
						if(notLast)
						{
							p+= z * M[i][k+2];
							M[i][k+2] -= p*r;
						}
		    
						M[i][k+1] -=  p*q;
						M[i][k] -= p;
					}
		
				} //for k...
			} //while iterations...
		} //while n ...
		return roots;
	}
 
}

