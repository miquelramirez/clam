#include "MDPModeler.hxx"
#include <iostream>

namespace CLAM
{
	inline const TData factorial( const TData n )
	{
		unsigned int i = 1;
		TData fact = n;

		for( ; i < n ; i++ )
			fact *= (n - i);

		return fact;
	}

	void MDPModelerConfig::DefaultInit( void )
	{
		AddAll();
		UpdateData();

		SetSemitoneRange( TData(.2599) );  // 4 semitones
		SetAmplitudeRange( 2. );
		SetNFrames(0);
	}

	MDPModeler::MDPModeler() :
		mNFrames( 0 )
	{
		MDPModelerConfig cfg;

		Configure( cfg );
	}

	MDPModeler::MDPModeler( MDPModelerConfig &cfg ) :
		mNFrames( 0 )
	{
		Configure( cfg );
	}

	bool MDPModeler::ConcreteConfigure( const ProcessingConfig& cfg ) throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const MDPModelerConfig& > (cfg);

		if( mConfig.HasSemitoneRange() )
			mSemitoneRange = mConfig.GetSemitoneRange();
		if( mConfig.HasAmplitudeRange() )
			mAmplitudeRange = mConfig.GetAmplitudeRange();
		if( mConfig.HasNToTrack() )
			mNToTrack = mConfig.GetNToTrack();
		if( mConfig.HasNCandidates() )
			mNCandidates = mConfig.GetNCandidates();
		if( mConfig.HasNFrames() )
			mNFrames = mConfig.GetNFrames();
			

		return true;
	}

	bool MDPModeler::ConcreteStart(void)
	{
		return true;
	}

	bool MDPModeler::ConcreteStop(void)
	{
		return true;
	}

	void MDPModeler::CalculateNStates(void)
	{
		int denom = ( factorial( mNCandidates - mNToTrack ) * factorial( mNToTrack ) );

		mNStates = factorial( mNCandidates ) / denom;
	}

	inline void MDPModeler::CheckTypes( const Candidates &candidates, const MDP &mdp ) const
	{
		CLAM_BEGIN_CHECK

		if( candidates.GetNFrames() != mNFrames )
		{
			std::stringstream ss;

			ss << "MDPModeler::Do : Wrong columns size in matrix of formants\n"
			   << "	Expected: " << mNFrames << "	, used " << candidates.GetNFrames();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( candidates.GetNCandidates() != mNCandidates )
		{
			std::stringstream s;

			s << "MDPModeler::Do: Wrong rows size in matrix of formants\n"
			  << "	Expected: " << mNCandidates << "	, used " << candidates.GetNCandidates();
			CLAM_ASSERT( 0, s.str().c_str() );		
		}

		CLAM_END_CHECK
	}

	void MDPModeler::Permute( const unsigned &ntrack, const unsigned &ncandidates, MDP& mdp, 
							  MatrixTmpl<bool> &binState, unsigned &nstates, Array<bool> s )
	{
		unsigned i=0;
		for ( unsigned p = ntrack-1; p < ncandidates-1; p++ )
			{
				std::swap( s[p] , s[p+1]);
				for( unsigned j=0; j<mNCandidates; j++ )
				{
					binState( nstates, j) = s[j];
					if( s[j]==1 )
					{
						mdp.SetStateTable( nstates, i, j+1);
						i++;
					}
				}
				i=0;
				nstates++;
				Permute(ntrack-1,p+1,mdp, binState, nstates,s);
			}
	}

	void MDPModeler::GenerateStates( const Candidates& candidates, MDP& mdp )
	{
		unsigned int f, ncand, s, nstates, cand;
		bool accState;
		Array< bool > c, o;

		c.Resize( mNCandidates );
		c.SetSize( mNCandidates );
		o.Resize( mNCandidates );
		o.SetSize( mNCandidates );

		MatrixTmpl<bool> binState( mNStates, mNCandidates);

		nstates = 1;

		for( s = 0; s < mNToTrack; s++ ) // 1100
		{
			c[ s ] = 1;
			binState( 0, s ) = 1;
			mdp.SetStateTable( 0, s, s+1);
		}
		for( ; s < mNCandidates; s++ )
		{
			c[ s ] = 0;
			binState( 0, s ) = 0;
		}

		Permute( mNToTrack, mNCandidates, mdp, binState, nstates, c );

		for( f = 0; f < mNFrames; f++ )
		{
			ncand = candidates.GetNCandPerFrame()[ f ];
			for( cand = 0; cand < ncand; cand++ )
				c[cand]=1;
			for(;cand<mNCandidates;cand++)
				c[cand]=0;
			for( s=0; s<mNStates; s++ )
			{
				cand=0;
				accState=true;
				while( ( cand< mNCandidates) && ( accState ) )
				{
					bool tmp=binState( s, cand );
					o[cand] = c[ cand ]||binState( s, cand );										
					
					if( o[cand]==c[cand] ) accState = true;
					else					accState = false;
					cand++;
				}
				if( accState ) mdp.SetAccStates( f, s );
			}
		}
	}

	TData MDPModeler::GetProbTrans( const unsigned int s1, const unsigned int s2,
									const unsigned int k, const Candidates &c, 
									const MDP &mdp )
	{
		TData f1, f2, a1, a2, df, da;
		unsigned int n, cand1, cand2;
		TData prob = 1;

		for( n=0 ; n<mNToTrack; n++ )
		{
			cand1 = mdp.GetStateTable( s1, n);
			f1 = c.GetFreqMatrix()( cand1-1, k-1);
			a1 = c.GetMagMatrix()( cand1-1, k-1);

			df = f1*mSemitoneRange;
			//if( df==0 ) df=1E10;
			da = a1*mAmplitudeRange;
			//if( da==0 ) da=1E10;

			cand2 = mdp.GetStateTable( s2, n);
			f2 = c.GetFreqMatrix()( cand2-1, k);
			a2 = c.GetMagMatrix()( cand2-1, k);

		/*	if( n==0 )
			{
				if((f1<150)||(f1>900))
					prob = 0.00001;
				else if(f1<=250)
					prob = 1./100.*f1 - 1.5;
				else if(f1<800)
					prob = 1;
				else if(f1<=900)
					prob= - 1./100.*f1 + 9.;
		
				
				if((f2<150)||(f2>900))
					prob *= 0.00001;
				else if(f2<=250)
					prob *= 1./100.*f2 - 1.5;
				else if(f2<=900)
					prob*= - 1./100.*f2 + 9.;
			}
			else
			{
				if((f1<200)||(f1>2400))
					prob *= 0.00001;
				else if(f1<=400)
					prob *= 1./200.*f1 - 1;
				else if((f1>=2200)&&(f1<=2400))
					prob*= - 1./200.*f1 + 12.;

				if((f2<200)||(f2>2400))
					prob *= 0.00001;
				else if(f2<=400)
					prob *= 1./200.*f2 - 1;
				else if((f2>=2200)&&(f2<=2400))
					prob*= - 1./200.*f2 + 12.;
			}*/

			prob *= exp(-(((f2-f1)*(f2-f1))/(53*53)));
					    //-(((a2-a1)*(a2-a1))/(da*da)) );
			//prob *= exp(-(((a2-a1)*(a2-a1))/(3.3488*3.3488)));
			//if( prob==0 ) prob=1E-10;

		}
		
		return prob;
	}



	bool MDPModeler::HandleMidFrames( const Candidates &candidates, MDP &mdp  )
	{
		unsigned int f, s1, s2, s3, nstates1, nstates2, nstates3;
		TData p, factor;
		TData beta = 0;
		TData sumBeta = 0;

		for( f = mNFrames-2; f >= 1; f-- )
		{
			nstates1 = mdp.GetNStatesPerFrame( f-1 );
			nstates2 = mdp.GetNStatesPerFrame( f );
			nstates3 = mdp.GetNStatesPerFrame( f+1 );

			for( s2 = 0; s2 < nstates2; s2++ ) // states of the current frame f
			{
				for( s1 = 0; s1 < nstates1; s1++ ) // states of the frame f-1
				{
					p = GetProbTrans( s1, s2, f, candidates, mdp );
					mdp.SetProbTransition( s1, s2, f, p );
					//beta += mdp.GetBeta(s1, f+1)*mdp.GetProbTransition(s1,s2,f+1);
				}

				for( s3 = 0; s3 < nstates3; s3++ )
				{
					beta += mdp.GetBeta(s3, f+1)*mdp.GetProbTransition(s2,s3,f+1);
				}

				sumBeta += beta;
				mdp.SetBeta( s2, f, beta );
				beta = 0;
			}

			if( sumBeta != 0 )
			{
				factor = 1./sumBeta;

				for( s2 = 0; s2 < nstates2; s2++ )
					mdp.SetBeta( s2, f, mdp.GetBeta(s2,f)*factor );
				sumBeta = 0;
			}
		}

		return true;
	}

	bool MDPModeler::HandleLastFrame( const Candidates &candidates, MDP &mdp  )
	{
		unsigned int s1, s2, i, nstates1,nstates2;
		TData p, beta, factor;
		TSize ncandidates = candidates.GetNCandPerFrame()[mNFrames-1];
		unsigned f = mNFrames;
		
		while( ncandidates == 0 )
		{
			f = f-1;
			ncandidates = candidates.GetNCandPerFrame()[f-1];
		}
		
		mNFrames = f;
		nstates1 = mdp.GetNStatesPerFrame( f-2 );
		nstates2 = mdp.GetNStatesPerFrame( f-1 );

		factor = 1./TData(ncandidates);
		beta = 1;
		for( i = 0; i < mNToTrack; i++ )
			beta *= factor;
			
		for( s2 = 0; s2 < nstates2; s2++ ) // states of the last frame
		{
			for( s1 = 0; s1 < nstates1; s1++ ) // states of the previous frame
			{
				p = GetProbTrans( s1, s2, f-1, candidates, mdp );
				mdp.SetProbTransition( s1, s2, f-1, p );
			}

			mdp.SetBeta( s2, f-1, beta );
		}

		return true;
	}

	bool MDPModeler::HandleFirstFrame( const Candidates &candidates, MDP &mdp  )
	{
		unsigned int s1, s2, n;
		TData p, factor, beta, sumBeta;
		unsigned int nstates1 = mdp.GetNStatesPerFrame( 0 );
		unsigned int nstates2 = mdp.GetNStatesPerFrame( 1 );
		TSize ncandidates = candidates.GetNCandPerFrame()[0];

		if( ncandidates == 0 ) return false; // no candidates in first frame

		beta = 0;
		sumBeta = 0;
		factor = 1. / TData( ncandidates );
		p = 1;

		for( n = 0; n < mNToTrack; n++ )
			p*=factor;
		
		for( s1 = 0; s1 < nstates1; s1++ ) // states of the first frame
		{
			for( s2 = 0; s2 < nstates2; s2++ ) // states of the second frame
			{
				mdp.SetProbTransition( s1, s2, 0, p );
				beta += mdp.GetBeta(s2, 1)*mdp.GetProbTransition(s1,s2,1);
			}
			
			sumBeta += beta;
			mdp.SetBeta( s1, 0, beta);
			beta = 0;
		}

		if( sumBeta != 0 )
		{
			factor = 1. / sumBeta;
		
			for( s1 = 0; s1 < nstates1; s1++ )
				mdp.SetBeta( s1, 0, mdp.GetBeta(s1,0)*factor );
		}


		return true;
	}

	bool MDPModeler::Do( const Candidates &candidates, MDP &mdp )
	{
		if( !AbleToExecute() ) return true;

	//	CheckTypes( candidates, mdp );

		TData f=candidates.GetFreqMatrix()(0,3);
		TData m=candidates.GetMagMatrix()(0,3);

		CalculateNStates();

		mdp.SetNCandidates( mNCandidates );
		mdp.SetNToTrack( mNToTrack );
		mdp.SetNStates( mNStates );
		mdp.SetNFrames( mNFrames );

		mdp.AllocateMem();

		GenerateStates(candidates, mdp );

		HandleLastFrame( candidates, mdp );

		HandleMidFrames( candidates, mdp );

		HandleFirstFrame( candidates, mdp );


/*		for(unsigned f=0; f<mNFrames; f++)
		{
			for( unsigned c=0; c<mNCandidates; c++ )
				std::cout << candidates.GetFreqMatrix()(c,f) << " ";
			std::cout << std::endl;
		}
*/		
	/*	for(unsigned s=0; s<mNStates; s++)
		{
			for( unsigned cand=0; cand<mNToTrack; cand++ )
				std::cout << mdp.GetStateTable( s, cand )<< "  ";
			std::cout<<std::endl;
		}

*/
/*		for(unsigned f=0; f<mNFrames; f++)
		{
			std::cout << mdp.GetNStatesPerFrame(f) <<std::endl;
		}*/

/*		for(unsigned s=0; s<mNStates; s++)
		{
			for(unsigned f=0; f<mNFrames; f++)
			{
				std::cout << mdp.GetBeta(s,f) <<"  ";
			}

			std::cout << std::endl;
		}
*/
/*		for(unsigned s1=0;s1<mNStates;s1++)
		{
			for(unsigned s2=0;s2<mNStates;s2++)
			{
				for(unsigned f=0;f<mNFrames;f++)
					std::cout << mdp.GetProbTransition(s1,s2,f)<<"  ";
				std::cout<<std::endl;
			}
			std::cout<<std::endl<<std::endl;
		}*/
		return true;
	}

}	// namespace
