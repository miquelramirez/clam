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

#ifndef _DescriptionScheme_hxx_
#define _DescriptionScheme_hxx_


#include "DescriptionScope.hxx"

/*
 You can find the doxygen of the SemanticalAnalysis group
 at the end of this file.
 */

namespace CLAM
{


	/**
	 * The description scheme defines the set of attributes (Attribute)
	 * to be used for feature extraction systems.
	 * @ingroup SemanticalAnalysis
	 *
	 * A DescriptionScheme is only an specification.
	 * The real data is held in a DescriptionDataPool,
	 * a container that fits its structure to the one defined on
	 * the DescriptionScheme specification.
	 * 
	 * Attributes in a DescriptionScheme have a name and a type
	 * and they are organized in scopes.
	 * Each  scope (see DescriptionScope) defines a kind of target for the attributes:
	 * (note scope, sample scope, frame scope, phrase scope, sample scope...)
	 *
	 * You can add Attributes to the DescriptionScheme by
	 * using the DescriptionScheme::AddAttribute method.
	 * The rest of the methods are utility for the pool classes.
	 *
	 * @see SemanticalAnalysis module, that describes
	 * 	the full usage of those classes.
	 * @see DescriptionDataPool class, which instanciates a DescriptionScheme
	 * 
	 */
	class DescriptionScheme : public Component
	{
	private:
		typedef std::map<std::string, unsigned> ScopeMap;
		typedef std::vector<DescriptionScope *> Scopes;
	private:
		Scopes _scopes;
		ScopeMap _scopeNameMap;
	public:
		DescriptionScheme()
		{
		}

		~DescriptionScheme();

		/**
		 * Adds a new attribute to the description scheme.
		 * If the scope does not exist it will be added.
		 * @pre The name is alphanumeric
		 * @pre The scope has no other attribute with the same name
		 * @param DataType The type of the argument to be added
		 * @param scope    The scope where the attributes applies to
		 * @param name     The name for the attribute
		 */
		template < typename DataType >
		void AddAttribute(const std::string &scope, const std::string & name)
		{
			DescriptionScope & theScope = SearchScopeOrAdd(scope);
			theScope.template Add<DataType>(name);
		}

		DescriptionScope & SearchScopeOrAdd(const std::string scopeName)
		{
			const unsigned nScopes = _scopes.size();
			std::pair<ScopeMap::iterator,bool> result = 
				_scopeNameMap.insert(std::make_pair(scopeName,nScopes));

			if (!result.second) return *_scopes[result.first->second];

			DescriptionScope * theScope = new DescriptionScope(scopeName);
			_scopes.push_back(theScope);
			return *theScope;
		}

		unsigned GetScopeIndex(const std::string & name) const
		{
			ScopeMap::const_iterator it = _scopeNameMap.find(name);
			CLAM_ASSERT(it!=_scopeNameMap.end(), ("Attribute scope '" + name + "' not found").c_str());
			return it->second;
		}

		const DescriptionScope & GetScope(unsigned scopeIndex) const
		{
			CLAM_ASSERT(scopeIndex < _scopes.size(), "Accessing an illegal scope index for the description scheme");
			return *_scopes[scopeIndex];
		}

		const DescriptionScope & GetScope(const std::string & name) const
		{
			unsigned scopeIndex = GetScopeIndex(name);
			return GetScope(scopeIndex);
		}
		unsigned GetNScopes() const 
		{
			return _scopes.size();
		}

		const std::string & GetScopeName(unsigned scopeIndex) const
		{
			const DescriptionScope & scope = GetScope(scopeIndex);
			return scope.GetName();
		}
		void StoreOn(Storage & storage) const;
		void LoadFrom(Storage & storage);
		const char * GetClassName() const { return "DescriptionScheme"; }
	};
}

/**
 * @defgroup SemanticalAnalysis Semantical Analysis
 *
 * This module explains how to use CLAM to handle audio description
 * extraction using the DescriptionScheme object and its relatives.
 * They will allow you to do the extraction in a modular and incremental way
 * taking from other projects the parts you are interested in and
 * adding your own descriptors in a incremental way.
 *
 * This module intends to implement the system described on
 * http://www.iua.upf.es/mtg/clam/devel/doc/descriptors/Descriptors.html
 * but there is still some way to achieve the full functionality
 * described in there.
 *
 * @section DescriptionSpecification Defining and instanciating descriptors
 *
 * The central object for description extraction is the DescriptionScheme.
 * The description scheme (CLAM::DescriptionScheme) defines 
 * which are the attributes (CLAM::Attribute) we want to compute.
 * You can relate attributes to a name and a type and you
 * can organize attributes in different ''scopes''.
 * You can understand a scope (CLAM::DescriptionScope) as 
 * the kind of target for a given set of attributes.
 * For example, we normaly talk about
 * note scope, sample scope, frame scope, phrase scope...
 * that means that a given attribute will have a value for
 * every single note, sample, frame, phrase...
 *
 * @code
 * CLAM::DescriptionScheme scheme;
 * scheme.AddAttribute <CLAM::TData>    ("AudioSample", "SignalLevel");
 * scheme.AddAttribute <CLAM::TData>    ("AudioSample", "FilteredSignal");
 * scheme.AddAttribute <SamplePosition> ("Frame", "Center");
 * scheme.AddAttribute <CLAM::TData>    ("Frame", "Energy");
 * scheme.AddAttribute <CLAM::TData>    ("Frame", "RMS");
 * scheme.AddAttribute <CLAM::Spectrum> ("Frame", "SpectralDistribution");
 * scheme.AddAttribute <FramePosition>  ("Note", "Onset");
 * scheme.AddAttribute <CLAM::Pitch>    ("Note", "Pitch");
 * @endcode
 * 
 * The description scheme only specifies the attribute organization.
 * The real values are hold into the data pool (CLAM::DescriptionDataPool).
 * An instance of a DescriptionDataPool will hold the attributes
 * extracted from a single description source (ie, an audio).
 * It take the structure defined by the description scheme.
 *
 * @code
 * CLAM::DescriptionDataPool pool(scheme);
 * @endcode
 *
 * So, summarizing:
 * - A description scheme defines attributes to be computed
 * - Every attribute has its name and type and is related to a given scope
 * - A scope specifies the kind of attribute target
 * - A description data pool is the real container for the values computed by extraction.
 * - A description data pool matches the structure specified by a given description scheme.
 *
 * @dot
 digraph lala 
 {
	 edge [fontname=Helvetica, fontsize=10]
	 node [shape=rectangle, fontname=Helvetica, fontsize=12, style=filled]
	 node [URL="classCLAM_1_1\N.html"]
	 {
		 edge[dir=back, arrowtail=diamond, taillabel=" *    "]
		 node [color=indigo,fillcolor="0.6 0.1 .9"]
		 DescriptionScheme -> DescriptionScope -> Attribute
		 node [color=darkgreen,fillcolor="0.2 0.1 1.0"]
		 DescriptionDataPool -> ScopePool -> AttributePool
	 }
	 {
		 edge[dir=back, constraint=false]
		 DescriptionScheme -> DescriptionDataPool
		 DescriptionScope -> ScopePool
		 Attribute -> AttributePool
	 }
 }
 * @enddot
 *
 * @section PoolByHand Accessing the pool by hand
 *
 * Accessing the pool by hand is not the ideal way of doing it but,
 * currently, extractor binding is not so complete so, by now,
 * it is the only way to do certain things.
 *
 * The scope provides interface to:
 * - Populate a given scope with a size.
 *   That is for the scope Note, telling how many notes there are.
 *   @code
 *   pool.SetNumberOfContexts("Note",60);
 *   @endcode
 * - Obtaining the attribute pool for writting, 
 *   so you will get a vector of pitches one for every note.
 *   @code
 *   CLAM::Pitch * pitches = pool.GetWritePool<CLAM::Pitch>("Note","Pitch");
 *   @endcode
 * - Obtaining the attribute pool for reading
 *   @code
 *   const CLAM::Pitch * pitches = pool.GetReadPool<CLAM::Pitch>("Note","Pitch");
 *   @endcode
 *
 * @warning A write access is required before any read access.
 * The pool will create the attribute pool memory only when
 * somebody requires to write in it.
 *
 * The access is templatized by the attribute type.
 * The pool user need not to handle generic types (void*, casts...)
 * and her code keeps typesafe. 
 * Some checking between the usage and the real type 
 * for the attribute is done on run-time.
 * So if you use a different value type an assertion will fail.
 *
 * @section XML
 *
 * Description data pools can be loaded or stored in XML, as any
 * other CLAM::Component, by using an CLAM::XmlStorage.
 *
 * @code
 * // Storing a description in XML
 * CLAM::XmlStorage::Dump(pool, "DescriptionPool", "mysong.xml");
 * @endcode
 *
 * @code
 * // Recovering an XML description
 * CLAM::XmlStorage::Restore(pool, "mysong.xml");
 * @endcode
 *
 *
 * @section ExtractorBinding Binding extractors 
 *
 * While you can use the pool simply as a container, 
 * like it has been explained above,
 * the aim of this system is to be able to deploy the extraction
 * system from an XML file that describes the Description Scheme and the Extraction Scheme.
 * This would be done by encapsulating algorithms that compute 
 * attributes on an abstraction called CLAM::Extractors.
 * They should be something very close to what a CLAM::Processing is
 * and, in fact, they will likely converge as the iterations go on.
 * An extractor has hooks for input and output data that are
 * fetched from the data pool.
 * The way data is fetched and droped is determined by the binding.
 * 
 * So, this part of the module is work on progress but
 * there are some parts already implemented and usable.
 * By now, what we have is some kinds of hook binding.
 * Current implemented binding operations are bindings on the same context, 
 * and indirection, that is, using an attribute to point another one even on a different scope.
 *
 * By now, there is no such abstract CLAM::Extractor but you can take a look
 * to some Extractors CLAMTest::CharCopierExtractor and CLAMTest::CharJoinExtractor.
 * Also you can see how binding is done by looking at the ExtractorTest.cxx file.
 *
 * @section DescriptionPoolTodo What is left to implement
 *
 * - An abstract CLAM::Extractor to derive from
 * - An special kind of extractor for scope population (how many items in a scope?)
 * - Bindings extension: relative position
 * - Solving Range and Relative bindings when outside the scope space
 * - The type system
 *   - Defining units friendly types for using them in attributes
 *   - Solving the creation of concrete Attribute specification from the type name
 * - XML Serialization for schemes
 * - Exploring new hook binding functionalities driven by real cases
 * 
 */

#endif// _DescriptionScheme_hxx_

