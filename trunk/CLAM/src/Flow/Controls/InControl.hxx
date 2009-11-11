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

#include <sstream>
#include <string>
#include <list>
#include "InControlBase.hxx"

namespace CLAM {

// Forward Declaration:
class Processing;
class OutControl;

/**
* Processing in control class for floats.
* Controls are limited to emmit and receive TControlData (float) numbers. 
*/
class ToRemoveInControl : public InControlBase
{
private:
	class Callback
	{
		public:
			virtual ~Callback() {}
			virtual void DoControl(const TControlData & val) =0;
	};
	class NullCallback : public Callback
	{
		public:
			virtual void DoControl(const TControlData & val) {}
	};
	template <typename ProcessingType, typename ValueParameterType>
	class MethodCallback : public Callback
	{
	protected:
		ProcessingType * _processing;
		void (ProcessingType::*_method)(ValueParameterType);
	public:
		MethodCallback(ProcessingType * processing, void (ProcessingType::*method)(ValueParameterType) )
			: _processing(processing)
			, _method(method)
		{
		}
		virtual void DoControl(const TControlData & value)
		{
			(_processing->*_method)(value);
		}
	};
	template <typename ProcessingType, typename ValueParameterType>
	class MethodCallbackWithId : public Callback
	{
		ProcessingType * _processing;
		void (ProcessingType::*_method)(unsigned, ValueParameterType);
		unsigned _id;
	public:
		MethodCallbackWithId(ProcessingType * processing, void (ProcessingType::*method)(unsigned,ValueParameterType), unsigned id )
			: _processing(processing)
			, _method(method)
			, _id(id)
		{
		}
		virtual void DoControl(const TControlData & value)
		{
			(_processing->*_method)(_id, value);
		}
	};

	Callback * _callback;
// Attributes:
protected:
	TControlData mLastValue; 

public:
//Constructor/Destructor
	ToRemoveInControl(const std::string &name, Processing* parent=0)
		: InControlBase(name, parent)
		, _callback(new NullCallback)
	{
	}
	template <typename ProcessingType, typename ParameterType>
	ToRemoveInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(ParameterType))
		: InControlBase(name,proc)
		, _callback(new MethodCallback<ProcessingType,ParameterType>(proc, callback))
	{
	}
	template <typename ProcessingType, typename ParameterType>
	ToRemoveInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, ParameterType))
		: InControlBase(name,proc)
		, _callback(new MethodCallbackWithId<ProcessingType,ParameterType>(proc, callback, id))
	{
	}
	virtual ~ToRemoveInControl();
	const std::type_info & GetTypeId() const
	{
		return typeid(TControlData);
	}
// Methods:
public:
	/**
	 * Stores the incoming control value. It can be retrieved
	 * using \c GetLastValue
	 */
	virtual void DoControl(TControlData val) 
	{
		mLastValue = val;
		_hasBeenRead=false;
		_callback->DoControl(val);
	}
	/// Returns the last TControlData (float) received event
	const TControlData& GetLastValue() const 
	{
		_hasBeenRead=true;
		return mLastValue;
	}

	const std::string GetLastValueAsString() // TODO: remove duplicated code on TypedInControl (Natanael)
	{
		std::ostringstream valueStream;
		valueStream << GetLastValue();
		return valueStream.str();
	}

};

} // namespace CLAM


#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/TypeInfo.hxx>
#include <CLAM/OutControlBase.hxx>
#include <sstream>

namespace CLAM {
	class Processing;
	class OutControlBase;

	template<class TypedControlData>
	class TypedOutControl;

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
		TypedInControl(const std::string &name = "unnamed in control", Processing * proc = 0)
			: InControlBase(name,proc)
			, _callback(new NullCallback)
		{
		}
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallback<ProcessingType,ParameterType>(proc, callback))
		{
		}
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, const ParameterType&))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackWithId<ProcessingType,ParameterType>(proc, callback, id))
		{
		}
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(ParameterType))
			: InControlBase(name,proc)
			, _callback(new MethodCallbackByCopy<ProcessingType,ParameterType>(proc, callback))
		{
		}
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

		virtual void DoControl(const TypedControlData& val) 
		{
			mLastValue = val;
			_hasBeenRead=false;
			_callback->DoControl(val);
		};
		const TypedControlData& GetLastValue() const 
		{
			_hasBeenRead=true;
			return mLastValue; 
		};
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
	public:
		const std::string GetLastValueAsString() // TODO: Use plugins as soon we start to use non streamable types
		{
			return GetLastValueAsString((TokenIsStorableAsLeaf*)0);
		}
		// For the typed linking check
		virtual const std::type_info& GetTypeId() const { return typeid(TypedControlData); };
	};
	
// REFACTORING typed connections
typedef TypedInControl<float> FloatInControl;

	
} // End namespace CLAM
#endif //_InControl_

