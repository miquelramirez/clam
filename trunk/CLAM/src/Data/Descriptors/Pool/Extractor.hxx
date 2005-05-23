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
	const std::string GetScope() const
	{
		return _scope;
	}
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
		const std::string & scope = Hook<AttributeType>::_scope;
		const std::string & attribute = Hook<AttributeType>::_attribute;
		_data = _pool->template GetReadPool<AttributeType>(scope,attribute);
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
		const std::string & scope = Hook<AttributeType>::_scope;
		return _current < _pool->GetNumberOfContexts(scope);
	}

protected:
	virtual unsigned GetCurrent() const
	{
		if (!_chained) return _current;

		unsigned indirection = _chained->GetForReading();
		const std::string & scope = Hook<AttributeType>::_scope;
		CLAM_ASSERT(indirection<_pool->GetNumberOfContexts(scope),
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
		const std::string & scope = Hook<AttributeType>::_scope;
		const std::string & attribute = Hook<AttributeType>::_attribute;
		_data = _pool->template GetWritePool<AttributeType>(scope,attribute);
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
		const std::string & scope = Hook<AttributeType>::_scope;
		return _current < _pool->GetNumberOfContexts(scope);
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

