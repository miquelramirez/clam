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

#ifndef __FLTKCONFIGURATOR__
#define __FLTKCONFIGURATOR__

#include "ConfigurationVisitor.hxx"
#include <map>
#include <list>
#include <string>

#include "Assert.hxx"
#include "Enum.hxx"
#include "Filename.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"

#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Valuator.H> // For the FL_HORIZONTAL macro
#include <FL/Fl_Window.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/fl_file_chooser.H>


#undef GetClassName

#include "CBL.hxx"

namespace CLAM{

	/**
	 * A generic DynamicType edition dialog for FLTK.
	 * @ingroup Configurators
	 * @see DynamicType, QTConfigurator
	 */
	class FLTKConfigurator : public Fl_Window {
	// Inner classes
	private:
		class ConfigurationHolderBase
		{
		protected:
			ConfigurationHolderBase(DynamicType & holded)
				: mConfiguration(holded)
			{}
			DynamicType & mConfiguration;
		public:
			virtual void ApplyDataTo(FLTKConfigurator & configurator)=0;
			virtual ~ConfigurationHolderBase() {}
		};

		template <class HoldedType>
		class ConfigurationHolder : public ConfigurationHolderBase
		{
		public:
			ConfigurationHolder(HoldedType & configuration)
				: ConfigurationHolderBase(configuration)
			{}
			void ApplyDataTo(FLTKConfigurator & configurator)
			{
				configurator.SetConfig(static_cast<HoldedType&>(mConfiguration));
			}
		};
	// Own types
		typedef Fl_Window super;
		typedef std::map<std::string, Fl_Widget*> tWidgets;
		typedef std::list<ConfigurationHolderBase *> tConfigHolders;

	public:
		FLTKConfigurator()
			: super(360, 540, "Edit the configuration")
		{
			mSetter = NULL;
			mGetter = NULL;
			mWidgetPack = NULL;
			mScrollGroup = NULL;
			mWidgetNum = 0;
			mApplyCallback= makeMemberFunctor0((*this),FLTKConfigurator,noAction);
			end();
		}

		virtual ~FLTKConfigurator() {
			if (mSetter) delete mSetter;
			if (mGetter) delete mGetter;
			if (mWidgetPack) delete mWidgetPack;
			if (mScrollGroup) delete mScrollGroup;

			tConfigHolders::iterator it =
				mSubConfigurationHolders.begin();
			for (;it!=mSubConfigurationHolders.end(); it++)
				delete *it;
		}

		void SetApplyCallback(CBL::Functor0 functor) {
			mApplyCallback=functor;
		}

		template <class Config>
		void SetConfig(Config & config) {
			begin();
			CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
			CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
			mSetter = new ConfigurationSetter<Config,FLTKConfigurator>(&config, this);
			mGetter = new ConfigurationGetter<Config,FLTKConfigurator>(&config, this);

			mScrollGroup = new Fl_Scroll( 0, 0, w() , h()-25 );

			mWidgetPack = new Fl_Pack( 0 , 0, 0, 0 );
			mWidgetPack->type(FL_VERTICAL);
			mWidgetPack->align(FL_ALIGN_LEFT);
			mWidgetPack->spacing(4);

			GetInfo();

 			mWidgetPack->size( 340, mWidgetNum * 25 );
 			mWidgetPack->end();

			mScrollGroup->end();

			int widgetsOnColumn = mWidgetNum;
			if ( mWidgetNum > 20 )
				widgetsOnColumn = 20;

			Fl_Pack* buttons = new Fl_Pack( 20 , (widgetsOnColumn+1)*25, w(), 20 );

			buttons->type(FL_HORIZONTAL);
			buttons->spacing(4);

			Fl_Button *applyButton=new Fl_Button(1,1,(w()-40)/3,20);
			applyButton->label( "Apply" );
			applyButton->labelsize(12);
			applyButton->callback(Apply,this);

			Fl_Button *discardButton=new Fl_Button(1,1,(w()-40)/3,20);
			discardButton->label( "Discard" );
			discardButton->labelsize(12);
			discardButton->callback(Discard, this);

			Fl_Return_Button *okButton=new Fl_Return_Button(1,1,(w()-40)/3,20);
			okButton->label( "Ok" );
			okButton->labelsize(12);
			okButton->callback(Ok, this);

			buttons->end();

			size( w(), 25*(widgetsOnColumn+2));

			end();
			//show();
		}
	private:

		void GetInfo() {
			CLAM_ASSERT(mGetter,"Configurator: No config to set");
			mGetter->VisitConfig();
		}
		void SetInfo() {
			CLAM_ASSERT(mSetter,"Configurator: No config to set");
			mSetter->VisitConfig();
		}

		Fl_Widget * GetWidget(const char * name) {
			tWidgets::iterator found=mWidgets.find(name);
			if (found==mWidgets.end()) return NULL;
			return found->second;
		}

	public:

		/** Default implementation, do nothing */
		template <typename T>
		void AddWidget(const char *name, void *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
		        Fl_Box* b = new Fl_Box(0, 0, 155, 20, name);
			Fl_Box* notEditable = new Fl_Box( 155, 0, 170, 20, "Not Editable");
			o->end();

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			notEditable->labelsize(12);
			notEditable->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
			notEditable->box(FL_FLAT_BOX);

			mWidgetNum++;
		}
		/** Default implementation, do nothing */
		template <typename T>
		void RetrieveValue(const char *name, void *foo, T& value) {
		}

		template <typename T>
		void AddWidget(const char *name, std::string *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
		        Fl_Box* b = new Fl_Box(0, 0, 155, 20, name);
			Fl_Input * mInput = new Fl_Input( 155, 0, 170, 20);
			o->end();

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			mInput->value( value.c_str() );
			mInput->textsize(12);
			mInput->align(FL_ALIGN_LEFT);

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, std::string *foo, T& value) {
			Fl_Input * mInput = dynamic_cast<Fl_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, Filename *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
		        Fl_Box* b = new Fl_Box(0, 0, 155, 20, name);

			Fl_Pack* fileWidget = new Fl_Pack( 155, 0, 170, 20);

			fileWidget->type(FL_HORIZONTAL);
			fileWidget->spacing(4);

			Fl_Input * mInput = new Fl_Input( 0, 0, 150, 20);
			Fl_Button * fileBrowserLauncher = new Fl_Button (0,0,20,20);
			fileBrowserLauncher->label("...");
			fileBrowserLauncher->callback(LaunchFileBrowser, mInput);

			fileWidget->end();
			o->end();

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			mInput->value( value.c_str() );
			mInput->textsize(12);
			mInput->align(FL_ALIGN_LEFT);

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, Filename *foo, T& value) {
			Fl_Input * mInput = dynamic_cast<Fl_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, TData *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
			Fl_Box* b = new Fl_Box(0, 0, 250, 20, name);
			Fl_Float_Input * mInput = new Fl_Float_Input(250, 0, 75, 20);
			o->end();

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			mInput->align(FL_ALIGN_LEFT);

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TData *foo, T& value) {
			Fl_Float_Input * mInput = dynamic_cast<Fl_Float_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const char * readValue=mInput->value();
			std::stringstream s(readValue);
			s >> value;
		}

		template <typename T>
		void AddWidget(const char *name, TSize *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
			Fl_Box * b = new Fl_Box(0, 0, 250, 20, name);
			Fl_Int_Input * mInput = new Fl_Int_Input(250, 0, 75, 20);
			Fl_Box * padding = new Fl_Box(325, 0, 5, 20);
			padding->box(FL_FLAT_BOX);
			o->end();

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			mInput->align(FL_ALIGN_LEFT);

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TSize *foo, T& value) {
			Fl_Int_Input * mInput = dynamic_cast<Fl_Int_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const char * readValue=mInput->value();
			std::stringstream s(readValue);
			s >> value;
		}

		template <typename T>
		void AddWidget(const char *name, bool *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Light_Button * mInput = new Fl_Light_Button(0, 0, 330, 20);

			mInput->label( name );
			mInput->labelsize(12);
			mInput->value( value );

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, bool *foo, T& value) {
			Fl_Light_Button * mInput = dynamic_cast<Fl_Light_Button*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=(mInput->value()!=0);
		}

		template <typename T>
		void AddWidget(const char *name, Enum *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
			Fl_Box * b = new Fl_Box(0, 0, 180, 20, name);
			Fl_Choice* mChoice = new Fl_Choice(180, 0, 145, 20);
			Fl_Box * padding = new Fl_Box(325, 0, 5, 20);
			padding->box(FL_FLAT_BOX);
			o->end();

			b->labelsize(12);
			b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			b->box(FL_FLAT_BOX);

			mChoice->textsize(12);
			mChoice->align(FL_ALIGN_LEFT);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mChoice->add( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mChoice->value(i);
			}

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mChoice));
		}
		template <typename T>
		void RetrieveValue(const char *name, Enum *foo, T& value) {
			Fl_Choice * mInput = dynamic_cast<Fl_Choice*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			CLAM_BEGIN_CHECK
				int i;
				for (i = 0; mapping[i].name; i++);
				CLAM_ASSERT(mInput->value()<i, "Configurator: Choice value out of range");
			CLAM_END_CHECK
			value=mapping[mInput->value()].value;
		}

		template <typename T>
		void AddWidget(const char *name, DynamicType *foo, T&value) {
			const unsigned int verPos = 0;
			const unsigned int nameWidth = (int) fl_width(name) + 1;
			const unsigned int detailsWidth = (int) fl_width( "Details..." ) + 1;
			fl_font(FL_HELVETICA,12);

			Fl_Group* o = new Fl_Group(0, 0, 330, 20);
			Fl_Box * mBox = new Fl_Box(nameWidth, verPos, nameWidth, 20);
			Fl_Button * mInput = new Fl_Button( 330-55-detailsWidth, verPos, detailsWidth + 55, 20, "Details...");
			o->end();

			mInput->labelsize(12);

			mBox->labelsize(12);
			mBox->label(name);
			mBox->align(FL_ALIGN_LEFT);

			mInput->callback(ShowSubConfig);
			ConfigurationHolderBase * holder = new ConfigurationHolder<T>(value);
			mInput->user_data(holder);
			mSubConfigurationHolders.push_back(holder);

			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, DynamicType *foo, T&value) {
		}

	public: // Callbacks
		static void Apply(Fl_Widget* o, void* v) {
			FLTKConfigurator * owner = dynamic_cast<FLTKConfigurator*>(o->window());
			CLAM_ASSERT(owner,"The given widget is not a FLTKConfigurator");
			owner->SetInfo();
			owner->mApplyCallback();
		}
		static void Discard(Fl_Widget* o, void* v) {
			o->window()->hide();
			delete o->window();
		}
		static void Ok(Fl_Widget* o, void* v) {
			Apply(o,v);
			Discard(o,v);
		}
		static void ShowSubConfig(Fl_Widget* o, void* v) {
			FLTKConfigurator * subConfigurator = new FLTKConfigurator;

			ConfigurationHolderBase * handler = (ConfigurationHolderBase*)(v);
			handler->ApplyDataTo(*subConfigurator);
			subConfigurator->show();
		}

		static void LaunchFileBrowser(Fl_Widget* o, void * data)
		{
			Fl_Input * fileInput = (Fl_Input*) data;
			fileInput->value(fl_file_chooser("Select filename","*",fileInput->value(),1 ));
		}

		void show() {
			set_modal();
			super::show();
		}

		void noAction() {
		}

	private:
		int mWidgetNum;

		Fl_Pack * mWidgetPack;
		Fl_Scroll * mScrollGroup;

		ConfigurationVisitor * mGetter;
		ConfigurationVisitor * mSetter;
		tWidgets mWidgets;
		CBL::Functor0 mApplyCallback;
		tConfigHolders mSubConfigurationHolders;

	};
}
#endif//__FLTKCONFIGURATOR__

