/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __CONFIGURATIONVISITOR__
#define __CONFIGURATIONVISITOR__

namespace CLAM{
	class ConfigurationVisitor {
	public:
		virtual ~ConfigurationVisitor() {}
		virtual void VisitConfig()=0;
	};

	template <typename Configuration, typename Builder>
	class ConfigurationGetter : public ConfigurationVisitor {
	public:
		ConfigurationGetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~ConfigurationGetter() {}
		
		template <typename T>
		void Accept(const char *name, T &value) { 
			mBuilder->AddWidget(name, &value, value); 
		}

		virtual void VisitConfig() {
			mConfig->VisitAll(*this);
		}

	private:
		Builder * mBuilder;
		Configuration * mConfig;
		
	};
	template <typename Configuration, typename Builder>
	class ConfigurationSetter : public ConfigurationVisitor {
	public:
		ConfigurationSetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~ConfigurationSetter() {}
		
		template <typename T>
		void Accept(const char *name, T &value) { 
			mBuilder->RetrieveValue(name, &value, value); 
		}

		virtual void VisitConfig() {
			mConfig->VisitAll(*this);
		}

	private:
		Builder * mBuilder;
		Configuration * mConfig;
		
	};

}
#endif//__CONFIGURATIONVISITOR__
