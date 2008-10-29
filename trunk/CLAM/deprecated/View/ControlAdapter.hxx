/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __CONTROLADAPTER__
#define __CONTROLADAPTER__

#include "InControl.hxx"
#include "OutControl.hxx"
#include "ModelController.hxx"
#include "InputControlModel.hxx"

namespace CLAMVM
{
		using CLAM::InControlBase;
		using CLAM::OutControl;

		class ControlAdapter
				: public  ModelController, public  InputControlModel
		{
		private:

				struct tManagedEmitter
				{

						OutControl              mControl;
						CLAM::TControlData            mValueToEmit;
						bool                    mIsDirty;

						tManagedEmitter();

						void Emit(  );

				};

				tManagedEmitter         mEmitter;
				bool                    mIsBound;
				bool                    mValueRangeChanged;
				CLAM::TControlData            mMinValue;
				CLAM::TControlData            mMaxValue;
		protected:

				void DispatchControlValue( CLAM::TControlData value );

				void DeleteSubAdapters();

		public:
				ControlAdapter();

				virtual ~ControlAdapter();

				virtual const char* GetClassName() const
				{
						return "ControlAdapter";
				}

				virtual bool Publish();

				virtual bool Update();

				void SetValueRange( CLAM::TControlData min, CLAM::TControlData max )
				{
						mMinValue = min;
						mMaxValue = max;
						mValueRangeChanged = true;
				}

				virtual bool BindTo( InControlBase&  adaptee );

				bool IsBound() const
				{
						return mIsBound;
				}


		};

}

#endif // ControlAdapter.hxx

