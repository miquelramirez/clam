#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/TypeInfo.hxx>
#include <CLAM/InControlBase.hxx>
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
	
	
} // End namespace CLAM
#endif // _TypedInControl_
