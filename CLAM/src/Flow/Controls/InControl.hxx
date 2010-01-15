/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _InControl_
#define _InControl_

#include <string>
#include <list>
#include <typeinfo>
#include <sstream>
#include "TypeInfo.hxx"
#include "InControlBase.hxx"
#include "OutControlBase.hxx"

namespace CLAM {
	class Processing;
	class OutControlBase;

	template<class ControlDataType>
	class OutControl;

	/**
	* An InControl receives values of the template type in an asyncronous pace.
	* A processing method can be set as callback to respond to incoming events.
	* You can also consult GetLastValue to get the last received value.
	*/
	template<class ControlDataType>
	class InControl : public InControlBase
	{
		class Callback;
	private:
		Callback * _callback;
	protected:
		ControlDataType mLastValue;
		
	public:
		/// Constructor to use when no callback is used
		InControl(const std::string &name = "unnamed in control", Processing * proc = 0)
			: InControlBase(name,proc)
			, _callback(new NullCallback)
		{
		}
		/// Constructor to use a callback by const reference
		template <typename ProcessingType, typename ParameterType>
		InControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallback<ProcessingType,ParameterType>(proc, callback))
		{
		}
		/// Constructor to use a callback by const reference plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		InControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackWithId<ProcessingType,ParameterType>(proc, callback, id))
		{
		}
		/// Constructor to use a callback by copy
		template <typename ProcessingType, typename ParameterType>
		InControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(ParameterType))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackByCopy<ProcessingType,ParameterType>(proc, callback))
		{
		}
		/// Constructor to use a callback by copy plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		InControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, ParameterType))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackByCopyWithId<ProcessingType,ParameterType>(proc, callback, id))
		{
		}


		virtual ~InControl()
		{
			delete _callback;
		}

		/// The control receives a value when this method gets called.
		/// Associated callback if any, gets triggered on result.
		/// Connected OutControl may trigger it but it also may be
		/// called directly, for example to set the initial value.
		virtual void DoControl(const ControlDataType& val) 
		{
			mLastValue = val;
			_hasBeenRead=false;
			_callback->DoControl(val);
		};

		/// Returns the last received value
		virtual const ControlDataType& GetLastValue() const 
		{
			_hasBeenRead=true;
			return mLastValue; 
		};
		/// Convenience method to get the string representation of the last value.
		/// This just works if the token is storable as XML leaf, if not a "Not printable"
		/// string is given.
		const std::string GetLastValueAsString() // TODO: Use plugins as soon we start to use non streamable types
		{
			return GetLastValueAsString((TokenIsStorableAsLeaf*)0);
		}
		// For the typed linking check
		virtual const std::type_info& GetTypeId() const { return typeid(ControlDataType); };
	private:
		std::string GetLastValueAsString(StaticFalse* /*isStreamable*/) const
		{
			return "Not printable";
		}
		/** @return A string with the extracted XML content */
		std::string GetLastValueAsString(StaticTrue* /*isStreamable*/) const
		{
			std::ostringstream valueStream;
			valueStream << GetLastValue();
			return valueStream.str();
		}

	private:
		// Callback wrappers

		/// Base control callback wrapper. It defines the interface for callback wrappers.
		typedef typename TypeInfo<ControlDataType>::StorableAsLeaf TokenIsStorableAsLeaf;
		class Callback
		{
			public:
				virtual ~Callback() {}
				virtual void DoControl(const ControlDataType & val) =0;
		};

		/// Null control callback wrapper. Just do nothing.
		class NullCallback : public Callback
		{
			public:
				virtual void DoControl(const ControlDataType & val) {}
		};

		/// Processing method callback wrapper.
		/// Calls a processing method that receives the value as const reference.
		template <typename ProcessingType, typename ValueParameterType>
		class MethodCallback : public Callback
		{
		protected:
			ProcessingType * _processing;
			void (ProcessingType::*_method)(const ValueParameterType& );
		public:
			MethodCallback(ProcessingType * processing, void (ProcessingType::*method)(const ValueParameterType &) )
				: _processing(processing)
				, _method(method)
			{
			}
			virtual void DoControl(const ControlDataType & value)
			{
				(_processing->*_method)(value);
			}
		};

		/// Processing method callback wrapper with control id.
		/// Calls a processing method that receives the value as const reference.
		/// The id enables reusing the same callback for different controls, 
		/// but still knowing the originating control.
		template <typename ProcessingType, typename ValueParameterType>
		class MethodCallbackWithId : public Callback
		{
			ProcessingType * _processing;
			void (ProcessingType::*_method)(unsigned, const ValueParameterType &);
			unsigned _id;
		public:
			MethodCallbackWithId(ProcessingType * processing, void (ProcessingType::*method)(unsigned,const ValueParameterType &), unsigned id )
				: _processing(processing)
				, _method(method)
				, _id(id)
			{
			}
			virtual void DoControl(const ControlDataType & value)
			{
				(_processing->*_method)(_id, value);
			}
		};

		/// Processing method callback wrapper by copy.
		/// Calls a processing method that receives the control value by copy.
		/// To use with basic (cheap copy) objects (ints, bools, floats...)
		template <typename ProcessingType, typename ValueParameterType>
		class MethodCallbackByCopy : public Callback
		{
		protected:
			ProcessingType * _processing;
			void (ProcessingType::*_method)(ValueParameterType);
		public:
			MethodCallbackByCopy(ProcessingType * processing, void (ProcessingType::*method)(ValueParameterType) )
				: _processing(processing)
				, _method(method)
			{
			}
			virtual void DoControl(const ControlDataType & value)
			{
				(_processing->*_method)(value);
			}
		};

		/// Processing method callback wrapper by copy with control id.
		/// Calls a processing method that receives the control value by copy.
		/// To use with basic (cheap copy) objects (ints, bools, floats...)
		/// The id enables reusing the same callback for different controls, 
		/// but still knowing the originating control.
		template <typename ProcessingType, typename ValueParameterType>
		class MethodCallbackByCopyWithId : public Callback
		{
			ProcessingType * _processing;
			void (ProcessingType::*_method)(unsigned, ValueParameterType);
			unsigned _id;
		public:
			MethodCallbackByCopyWithId(ProcessingType * processing, void (ProcessingType::*method)(unsigned,ValueParameterType), unsigned id )
				: _processing(processing)
				, _method(method)
				, _id(id)
			{
			}
			virtual void DoControl(const ControlDataType & value)
			{
				(_processing->*_method)(_id, value);
			}
		};
	};
	
/// Alias provided by convenience to ease the transitions to typed controls
typedef InControl<float> FloatInControl;

} // End namespace CLAM
#endif //_InControl_

