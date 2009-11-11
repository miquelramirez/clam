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

	template<class TypedControlData>
	class TypedOutControl;

	/**
	* An InControl receives values of the template type in an asyncronous pace.
	* A processing method can be set as callback to respond to incoming events.
	* You can also consult GetLastValue to get the last received value.
	*/
	template<class TypedControlData>
	class TypedInControl : public InControlBase
	{
	private:
		typedef typename TypeInfo<TypedControlData>::StorableAsLeaf TokenIsStorableAsLeaf;
		class Callback
		{
			public:
				virtual ~Callback() {}
				virtual void DoControl(const TypedControlData & val) =0;
		};
		class NullCallback : public Callback
		{
			public:
				virtual void DoControl(const TypedControlData & val) {}
		};
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
			virtual void DoControl(const TypedControlData & value)
			{
				(_processing->*_method)(value);
			}
		};
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
			virtual void DoControl(const TypedControlData & value)
			{
				(_processing->*_method)(_id, value);
			}
		};

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
			virtual void DoControl(const TypedControlData & value)
			{
				(_processing->*_method)(value);
			}
		};
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
			virtual void DoControl(const TypedControlData & value)
			{
				(_processing->*_method)(_id, value);
			}
		};

		Callback * _callback;
	protected:
		TypedControlData mLastValue;
		
	public:
		/// Constructor to use when no callback is used
		TypedInControl(const std::string &name = "unnamed in control", Processing * proc = 0)
			: InControlBase(name,proc)
			, _callback(new NullCallback)
		{
		}
		/// Constructor to use a callback by const reference
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallback<ProcessingType,ParameterType>(proc, callback))
		{
		}
		/// Constructor to use a callback by const reference plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackWithId<ProcessingType,ParameterType>(proc, callback, id))
		{
		}
		/// Constructor to use a callback by copy
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(ParameterType))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackByCopy<ProcessingType,ParameterType>(proc, callback))
		{
		}
		/// Constructor to use a callback by copy plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, ParameterType))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackByCopyWithId<ProcessingType,ParameterType>(proc, callback, id))
		{
		}


		~TypedInControl()
		{
			delete _callback;
		}
		/// Method used to set the last received value. 
		/// This method triggers the associated callback if any.
		/// This is often called by the connected OutControls but 
		/// it can be also used directly when needed.
		virtual void DoControl(const TypedControlData& val) 
		{
			mLastValue = val;
			_hasBeenRead=false;
			_callback->DoControl(val);
		};
		/// Returns the last received value
		const TypedControlData& GetLastValue() const 
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
		virtual const std::type_info& GetTypeId() const { return typeid(TypedControlData); };
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
	};
	
// REFACTORING typed connections
typedef TypedInControl<float> FloatInControl;

	
} // End namespace CLAM
#endif //_InControl_

