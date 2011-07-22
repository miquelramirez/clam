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


#include "Assert.hxx"
#include "TypeInfoStd.hxx"
#include "StaticBool.hxx"
#include <typeinfo>

//////////////////////////////////////////////////////////////////////
// Reimplementation using chained template methods
// These macros expand functions in the concrete (derived) class of DynamicType.
//
// Related macros: 
// * DYNAMIC_TYPE
// * DYNAMIC_TYPE_USING_INTERFACE
// * DYN_ATTRIBUTE
// * DYN_CONTAINER_ATTRIBUTE
//////////////////////////////////////////////////////////////////////


#define __COMMON_DYNAMIC_TYPE(CLASS_NAME,N) \
public: \
	virtual const char* GetClassName() const { \
		return #CLASS_NAME; \
	}\
	enum { eNumAttr= N }; \
	CLAM::DynamicType& GetDynamicTypeCopy() const\
	{ \
		return *new CLASS_NAME(*this); \
	}\
	CLAM::Component * Species() const\
	{ \
		return new CLASS_NAME(); \
	}\
protected: \
	static TAttr * TypeDescriptionTable() \
	{\
		static bool alreadyInitialized = false;\
		static TAttr staticTypeDescTable[N+1];\
		if (not alreadyInitialized) \
			InformChainedAttr((AttributePosition<0>*)NULL, staticTypeDescTable, 0); \
		return staticTypeDescTable; \
	}\
public: \
	/** Returns the type id of attribute n */ \
	const std::type_info & GetTypeId(unsigned n) const \
	{ \
		return GetChainedTypeId((AttributePosition<0>*)NULL,n); \
	} \
	/** Visit all Dynamic Attributes */ \
	template <typename Visitor> \
	void VisitAll (Visitor & visitor) { \
		VisitChainedAttr((AttributePosition<0>*)NULL, visitor); \
	} \
protected: \
	/** Store all Dynamic Attributes */ \
	virtual void StoreDynAttributes(CLAM::Storage & s) const { \
		StoreChainedAttr((AttributePosition<0>*)NULL,s); \
	} \
	/** Load all Dynamic Attributes */ \
	virtual void LoadDynAttributes(CLAM::Storage & s) { \
		AddAll(); \
		UpdateData(); \
		LoadChainedAttr((AttributePosition<0>*)NULL,s); \
		UpdateData(); \
	} \
private: \
	template <unsigned int NAttrib> \
	class AttributePosition : public CLAM::DynamicType::AttributePositionBase<NAttrib> { \
		public: \
			typedef StaticBool<!(NAttrib>=N)> InboundsCheck; \
	}; \
	/** Instantiated whenever a Attribute number is out of range. \
	 * Gives a compilation error message. \
	 */ \
	template <unsigned int NAttrib> \
	void CheckAttribute (StaticFalse*inRange,AttributePosition<NAttrib>*a) { \
		AttributePosition<(NAttrib)-1>* previous; \
		previous->CompilationError_AttributePositionOutOfBounds(); \
	}\
	/** \
	 * Instantiated whenever a Attribute number is left. \
	 * Gives a compilation error message. \
	 */ \
	template <unsigned int NAttrib> \
	void CheckAttribute (StaticTrue*inRange,AttributePosition<NAttrib>*a) { \
		a->CompilationError_AttributeNotDefined(); \
	}\
	/** Undefined link for the Visit method chain (Visit)*/  \
	/* \
	template <unsigned int NAttrib, typename Visitor> \
	void VisitChainedAttr (AttributePosition<NAttrib>*a, Visitor & visitor) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}*/\
	/** Undefined link for the Inform method chain (Inform) */ \
	template <unsigned int NAttrib> \
	static void InformChainedAttr (AttributePosition<NAttrib>*a, TAttr * typeDescTable, unsigned offset) { \
		typedef typename AttributePosition<NAttrib>::InboundsCheck InboundsCheck; \
		CheckAttribute ((InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link on the method chain (Store) */ \
	template <unsigned int NAttrib> \
	void StoreChainedAttr (AttributePosition<NAttrib>*a,CLAM::Storage & s) const { \
		typedef typename AttributePosition<NAttrib>::InboundsCheck InboundsCheck; \
		CheckAttribute ((InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link on the method chain (Load) */ \
	template <unsigned int NAttrib> \
	void LoadChainedAttr (AttributePosition<NAttrib>*a,CLAM::Storage & s) { \
		typedef typename AttributePosition<NAttrib>::InboundsCheck InboundsCheck; \
		CheckAttribute ((InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
private: \
	/** Method chain terminator */ \
	template <typename Visitor> \
	void VisitChainedAttr (AttributePosition<N>*, Visitor & visitor) {} \
	/** Method chain terminator */ \
	static void InformChainedAttr (AttributePosition<N>*a, TAttr * typeDescTable, unsigned offset)\
	{ \
		typeDescTable[N].size = offset; \
	} \
	/** Method chain terminator */ \
	void StoreChainedAttr (AttributePosition<N>*pos, CLAM::Storage &s) const {} \
	/** Method chain terminator */ \
	void LoadChainedAttr (AttributePosition<N>*pos, CLAM::Storage &s) {} \
	/** Undefined link on the method chain (GetTypeId) */ \
	const std::type_info & GetChainedTypeId(AttributePosition<N>*pos, unsigned n) const \
	{ \
		return typeid(void); \
	} \



#define DYNAMIC_TYPE(CLASS_NAME, N)\
public: \
	CLASS_NAME() \
		: CLAM::DynamicType(N, TypeDescriptionTable()) \
	{\
		DefaultInit();\
	}\
	CLASS_NAME(const CLASS_NAME& prototype)\
		: CLAM::DynamicType(prototype) { \
		CopyInit(prototype);\
		}\
	__COMMON_DYNAMIC_TYPE(CLASS_NAME,N); \


#define DYNAMIC_TYPE_USING_INTERFACE(CLASS_NAME, N, INTERFACE_NAME)\
public: \
	CLASS_NAME() : INTERFACE_NAME(N, TypeDescriptionTable())\
	{\
		DefaultInit();\
	}\
	CLASS_NAME(const CLASS_NAME& prototype)\
		: INTERFACE_NAME(prototype) { \
		CopyInit(prototype); \
		}\
	__COMMON_DYNAMIC_TYPE(CLASS_NAME,N); \


#define __COMMON_DYN_ATTRIBUTE(N,ACCESS,TYPE,NAME) \
private: \
/** This declaration to detect compile-time-err of repeated attribute IDs(num), without having to relay in templates*/\
	struct {} CLAM_compile_time_error_Duplicated_Attribute_Index_##N;\
	\
ACCESS: \
	inline TYPE& Get##NAME() const {\
		CLAM_DEBUG_ASSERT((N<GetNDynamicAttributes()), \
			"There are more registered Attributes than the number " \
		        "defined in the DYNAMIC_TYPE macro.");\
		CLAM_ASSERT(HasAttribute(N),\
			"You are trying to access attribute " #NAME \
			" that is not Added or not Updated.");\
		const void *p=GetAttributeAsVoidPtr(N);\
		return *static_cast<TYPE*>(const_cast<void*>(p)); \
	}\
\
	/** @pre already exist an object of the type in that position (that will be deleted)*/\
	inline void Set##NAME(TYPE const & arg) {\
		CLAM_DEBUG_ASSERT((N<GetNDynamicAttributes()), \
			"There are more registered Attributes than the number " \
		        "defined in the DYNAMIC_TYPE macro.");\
		CLAM_ASSERT(HasAttribute(N),\
			"You are trying to access attribute " #NAME \
			" that is not Added or not Updated.");\
		void * orig = (void*)(&arg);\
		void * pos = GetAttributeAsVoidPtr(N);\
		_attributeDestruct_<TYPE>(pos);\
		_attributeCopyConstruct_<TYPE>(pos, orig);\
	}\
	inline void Add##NAME() {\
		DynamicType::AddAttribute(N);\
	}\
	template <typename Visitor> \
	inline void Visit##NAME(Visitor & visitor) { \
		if (Has##NAME()) \
			visitor.Accept(#NAME,Get##NAME()); \
	}\
	inline void Remove##NAME() { \
		DynamicType::RemoveAttribute(N); \
	}\
	inline bool Has##NAME() const { \
		return DynamicType::HasAttribute(N); \
	} \
private: \
	template <typename Visitor> \
	void VisitChainedAttr(AttributePosition<N>*, Visitor & visitor) { \
		Visit##NAME(visitor); \
		VisitChainedAttr((AttributePosition<(N)+1>*)NULL, visitor); \
	} \
	static void InformChainedAttr( \
		AttributePosition<N>*, TAttr * typeDescTable, unsigned offset)\
	{ \
		InformAttr_<TYPE>(typeDescTable, N, #NAME, #TYPE);\
		InformChainedAttr((AttributePosition<(N)+1>*)NULL, typeDescTable, offset+sizeof(TYPE)); \
	} \
	void StoreChainedAttr(AttributePosition<N>*, CLAM::Storage & s) const { \
		Store##NAME(s); \
		StoreChainedAttr((AttributePosition<(N)+1>*)NULL,s); \
	} \
	void LoadChainedAttr(AttributePosition<N>*, CLAM::Storage & s) { \
		Load##NAME(s); \
		LoadChainedAttr((AttributePosition<(N)+1>*)NULL,s); \
	} \
	const std::type_info & GetChainedTypeId(AttributePosition<N>*, unsigned n) const { \
		if (n==N) return typeid(TYPE); \
		return GetChainedTypeId((AttributePosition<(N)+1>*)NULL,n); \
	} \


#define DYN_ATTRIBUTE(N,ACCESS,TYPE,NAME) \
	__COMMON_DYN_ATTRIBUTE(N,ACCESS,TYPE,NAME) \
protected: \
	void Store##NAME(CLAM::Storage & s) const \
	{ \
		if (not Has##NAME())  return; \
		DynamicType::StoreAttribute( \
			(CLAM::TypeInfo<TYPE >::StorableAsLeaf*)NULL, \
			s, Get##NAME(), #NAME); \
	} \
	bool Load##NAME(CLAM::Storage & s) { \
		TYPE obj; \
		if (!DynamicType::LoadAttribute( \
			(CLAM::TypeInfo<TYPE >::StorableAsLeaf*)NULL, \
			s, obj, #NAME)) \
		{ \
			Remove##NAME(); \
			return false; \
		} \
		Set##NAME(obj); \
		return true; \
	} \
ACCESS: \


#define DYN_CONTAINER_ATTRIBUTE(N,ACCESS,TYPE,NAME,ENAME) \
	__COMMON_DYN_ATTRIBUTE(N,ACCESS,TYPE,NAME) \
protected: \
	void Store##NAME(CLAM::Storage & s) const { \
		if (Has##NAME()) \
			StoreIterableAttribute(s, Get##NAME(), #NAME, #ENAME); \
	} \
	bool Load##NAME(CLAM::Storage & s) { \
		Add##NAME(); \
		UpdateData(); \
		if (LoadIterableAttribute(s, Get##NAME(), #NAME, #ENAME)) \
			return true; \
		Remove##NAME(); \
		return false; \
	} \
ACCESS: \



