/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "LPC.hxx"

namespace CLAM
{
	void LPCConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetOrder( 0 );
	}

	LPC::LPC() :
		mOrder( 0 )
	{
		LPCConfig cfg;
	
		AttachChildren();
		Configure( cfg );
	}

	LPC::LPC( LPCConfig& cfg ) :
		mOrder( 0 )
	{
		AttachChildren();
		Configure( cfg );
	}

	LPC::~LPC()
	{
	}

	bool LPC::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		bool  res = true;

		try{
			mConfig = dynamic_cast< const LPCConfig& > ( cfg );

			if( mConfig.HasOrder() )
			{
				if( mConfig.GetOrder() < 0 )
					throw( ErrProcessingObj( "Wrong (negative) Size in LPC Configuration", this ) );

				mOrder = mConfig.GetOrder();

				if( !ConfigureChildren() )
				{
					AddConfigErrorMessage("\nFailed to configure children: ");
					res = false;
				}
			}
		}
		catch( std::exception &e )
		{
			AddConfigErrorMessage("Failed to configure children: \n");
			AddConfigErrorMessage( e.what() );
			return false;
		}

		return res;
	}

	bool LPC::ConcreteStart()
	{
		ProcessingComposite::ConcreteStart();
		
		return true;
	}

	bool LPC::ConcreteStop()
	{
		ProcessingComposite::ConcreteStop();
		
		return true;
	}

	void LPC::AttachChildren()
	{
		mAutocorrelationEval.SetParent(this);
		mLevinson_Durbin.SetParent(this);
	}

	bool LPC::ConfigureChildren()
	{
		AutocorrelationEvaluatorConfig autoCfg;

		autoCfg.SetName( "AutoCorrelationEvaluator" );
		autoCfg.SetCorrelationSize( mOrder );
		
		if( !mAutocorrelationEval.Configure( autoCfg ) )
		{
			AddConfigErrorMessage( "mAutocorrelationEval: ");
			AddConfigErrorMessage( mAutocorrelationEval.GetConfigErrorMessage() );
			return false;
		}

		Levinson_DurbinConfig LDcfg;

		LDcfg.SetName( "Levinson_Durbin" );
		LDcfg.SetOrder( mOrder );

		if( !mLevinson_Durbin.Configure( LDcfg ) )
		{
			AddConfigErrorMessage("mLevinson_Durbin: ");
			AddConfigErrorMessage( mLevinson_Durbin.GetConfigErrorMessage() );
			return false;
		}

		return true;
	}

	bool LPC::Do( const Audio& in, DataArray& A, DataArray& K, TData& E )
	{
		Correlation R;
		Audio r;

		r.SetSize(in.GetSize());

		if( !AbleToExecute() ) return true;

		R.SetSize( mOrder );

		mAutocorrelationEval.Do( in, r );

		TData* outBuffer = r.GetBuffer().GetPtr();

		if( outBuffer[0] == 0 )
		{
			Audio tmpAudio;
			tmpAudio.SetSize(r.GetSize()-1);
			r.GetAudioChunk( 1,r.GetSize(),tmpAudio );
			mLevinson_Durbin.Do( tmpAudio, A, K, E );
		}
		else
			mLevinson_Durbin.Do( r, A, K, E );

		return true;
	}
} //namespace
