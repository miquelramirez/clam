#ifndef _Extractor_hxx_
#define _Extractor_hxx_

#include <string>
#include "Pool.hxx"

namespace CLAM
{

/** @ingroup SemanticalAnalysis */
template <typename AttributeType>
class Hook 
{
public:
	virtual ~Hook(){}
	void Bind(
			const std::string & scope,
			const std::string & attribute)
	{
		_scope = scope;
		_attribute = attribute;
	}

	virtual void Next() = 0;
	virtual bool IsInsideScope() const = 0;

protected:

	std::string _attribute;
	std::string _scope;
};

/** @ingroup SemanticalAnalysis */
template <typename AttributeType>
class ReadHook : public Hook<AttributeType>
{
public:
	ReadHook()
	{
		_chained=0;
	}
	~ReadHook()
	{
		if (_chained) delete _chained;
	}
	ReadHook & Bind(
		const std::string & scope, 
		const std::string & attribute)
	{
		Hook<AttributeType>::Bind(scope,attribute);
		return *this;
	}
	ReadHook & Indirect(
		const std::string & scope, 
		const std::string & attribute)
	{
		if (_chained) 
		{
			_chained->Indirect(scope,attribute);
		}
		else
		{
			_chained = new ReadHook<unsigned>;
			_chained->Bind(scope,attribute);
		}
		return *this;
	}

	const AttributeType & GetForReading() const
	{
		return _data [GetCurrent()];
	}

	void Init(const DescriptionDataPool & pool) 
	{
		_current = 0;
		_pool = &pool;
		_data = _pool->template GetReadPool<AttributeType>(_scope,_attribute);
		if (_chained) _chained->Init(pool);
	}

	virtual void Next()
	{
		if (_chained) _chained->Next();
		else _current++;
	}

	virtual bool IsInsideScope() const
	{
		if (_chained) return _chained->IsInsideScope();
		return _current < _pool->GetNumberOfContexts(_scope);
	}

protected:
	virtual unsigned GetCurrent() const
	{
		if (!_chained) return _current;

		unsigned indirection = _chained->GetForReading();
		CLAM_ASSERT(indirection<_pool->GetNumberOfContexts(_scope),
			"Invalid cross-scope reference");
		return indirection;
	}

protected:
	const DescriptionDataPool * _pool;
	const AttributeType * _data;
	ReadHook<unsigned> * _chained;
private:
	unsigned _current;
};

/** @ingroup SemanticalAnalysis */
template <typename AttributeType>
class ReadRangedHook : public ReadHook<AttributeType>
{
public:
	void GetRangeForReading(
		const AttributeType*& begin,
		const AttributeType*& end) const
	{
		begin = & (ReadHook<AttributeType>::GetForReading());
		end = begin + _range;
	}

	ReadHook<AttributeType> & Range(unsigned range)
	{
		_range = range;
		return *this;
	}
private:
	unsigned _range;
};

/** @ingroup SemanticalAnalysis */
template <typename AttributeType>
class WriteHook : public Hook<AttributeType>
{
public:
	void Init(DescriptionDataPool & pool) 
	{
		_pool = &pool;
		_current = 0;
		_data = _pool->template GetWritePool<AttributeType>(_scope,_attribute);
	}

	AttributeType & GetForWriting() const
	{
		return _data [GetCurrent()];
	}

	virtual void Next()
	{
		_current++;
	}

	virtual bool IsInsideScope() const
	{
		return _current < _pool->GetNumberOfContexts(_scope);
	}

protected:
	unsigned GetCurrent() const
	{
		return _current;
	}
private:
	DescriptionDataPool * _pool;
	AttributeType * _data;
	unsigned _current;
};

}





#endif // _Extractor_hxx_

