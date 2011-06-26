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

#ifndef __XMLWRITECONTEXT_HXX__
#define __XMLWRITECONTEXT_HXX__

#include <ostream>
#include <string>
#include "Assert.hxx"

namespace CLAM
{
/**
 * This class provides primitives to write an XML document to a target
 * stream and to keep track of the writting state.
 * You can only have an instance of that class at a time.
 * Instances of this class are created normally by as members of a
 * root Scoped object like XmlDocument or XmlFragment.
 * You can also instanciate this class directly whenever you want
 * to use its primitives without intermediates.
 * @todo Indentation
 * @todo Explicit namespaces (now are supported by hand)
 * @todo Checks on element levels
 */
class XmlWriteContext
{
	public:
		XmlWriteContext(std::ostream & targetStream) :
			mTarget(targetStream)
		{
			CLAM_ASSERT(!sCurrentContext,
				"An XML generation context is already defined");
			sCurrentContext = this;
			mLastWasAContent=false;
			mElementInserted=false;
			mTagOpened=false;
			mCurrentLevel = 0;
			mUseIndentation=false;
		}

		~XmlWriteContext()
		{
			sCurrentContext = 0;
		}

		static XmlWriteContext & CurrentContext()
		{
			CLAM_ASSERT(sCurrentContext,"XML generation context not created");
			return *sCurrentContext;
		}

		std::ostream & GetTarget()
		{
			return mTarget;
		}

		template <typename T>
		void InsertContent(const T & content)
		{
			if (mTagOpened) EndOpenTag();
			if (mElementInserted)
				NewIndentedLineIfEnabled();
			mTarget << content;
			mLastWasAContent = true;
		}

		void OpenElement(const std::string & name)
		{
			if (mTagOpened) EndOpenTag();
			NewIndentedLineIfEnabled();
			// TODO: Change the temporary to normal insertion with the upgrade to 0.5.2
			mTarget << std::string("<") << name;
			mTagOpened = true;
			mElementInserted = false;
			mLastWasAContent = false;
			mCurrentLevel++;
		}

		void CloseElement(const std::string & name)
		{
			mCurrentLevel--;
			if (mLastWasAContent || mElementInserted)
				CloseFullElement(name);
			else
				CloseEmptyElement();
			mElementInserted = true;
			mLastWasAContent = false;
		}

		template <typename T>
		void InsertAttribute(const std::string &name, const T & content)
		{
			CLAM_ASSERT(mTagOpened, "Appending attribute outside a tag");
			mTarget << std::string(" ") << name << std::string("='") 
				<< content << std::string("'");
		}

		/**
		 * @return true when the last thing inserted on the current node
		 * has been plain content.
		 */
		bool LastWasAContent()
		{
			return mLastWasAContent;
		}

		/**
		 * @return true when an element has been inserted on the current node.
		 */
		bool ElementInserted()
		{
			return mElementInserted;
		}

		unsigned int CurrentLevel()
		{
			return mCurrentLevel;
		}

		void UseIndentation(bool useIt)
		{
			mUseIndentation = useIt;
		}

	private:

		void CloseFullElement(const std::string & name)
		{
			if (mElementInserted)
				NewIndentedLineIfEnabled();
			// TODO: Change the temporary to normal insertion with the upgrade to 0.5.2
			mTarget << std::string("</") << name << std::string(">");
		}

		void CloseEmptyElement()
		{
			CLAM_ASSERT(mTagOpened,"No open tag to close");
			// TODO: Change the temporary to normal insertion with the upgrade to 0.5.2
			mTarget << std::string(" />");
			mTagOpened = false;
		}

		void EndOpenTag()
		{
			CLAM_ASSERT(mTagOpened,"No open tag to close");
			// TODO: Change the temporary to normal insertion with the upgrade to 0.5.2
			mTarget << std::string(">");
			mTagOpened = false;
		}

		void NewIndentedLineIfEnabled()
		{
			if (!mUseIndentation) return;
			// TODO: Change the temporary to normal insertion with the upgrade to 0.5.2
			if (mCurrentLevel!=0 || mLastWasAContent || mElementInserted)
				mTarget << std::string("\n");
			mTarget << std::string(mCurrentLevel,'\t');
		}

	private:

		std::ostream & mTarget;
		bool mLastWasAContent;
		bool mElementInserted;
		bool mTagOpened;
		bool mUseIndentation;
		unsigned int mCurrentLevel;

		static XmlWriteContext * sCurrentContext;
		
};

} // ns CLAM

#endif//__XMLWRITECONTEXT_HXX__

