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

#ifndef __FLTKCONFIGURATOR__
#define __FLTKCONFIGURATOR__

#include "ConfigurationVisitor.hxx"
#include <map>
#include <string>

#include "Assert.hxx"
#include "Enum.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"

#include <FL/fl_draw.H>
#include <FL/Fl_Valuator.H> // For the FL_HORIZONTAL macro
#include <FL/Fl_Window.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Item.H>

#define HorPos fl_width(name)+5+(mWidgetNum/20)*340
#define VerPos 5+25*(mWidgetNum%20)

namespace CLAM{
	class FLTKConfigurator : public Fl_Window {

		typedef Fl_Window super;
		typedef std::map<std::string, Fl_Widget*> tWidgets;
	public:
		FLTKConfigurator() 
			: super(345, 40, "Edit the configuration")
			
		{
			mSetter = 0;
			mGetter = 0;
			mWidgetNum = 0;
		}

		virtual ~FLTKConfigurator() {
			if (mSetter) delete mSetter;
			if (mGetter) delete mGetter;
		}

		template <class Config>
		void SetConfig(Config & config) {
			CLAM_ASSERT(!mSetter, "Configurator: Configuration assigned twice");
			CLAM_ASSERT(!mGetter, "Configurator: Configuration assigned twice");
			mSetter = new ConfigurationSetter<Config,FLTKConfigurator>(&config, this);
			mGetter = new ConfigurationGetter<Config,FLTKConfigurator>(&config, this);

			GetInfo();
			
			Fl_Pack * buttons = new Fl_Pack(w()-3*((w()-40)/3)-10,(mWidgetNum-1)*20,600,20);
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

			size(345+(mWidgetNum/20)*340,35+25*(mWidgetNum>20 ? 21 : mWidgetNum%20+1));

			end();
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
		}
		/** Default implementation, do nothing */
		template <typename T>
		void RetrieveValue(const char *name, void *foo, T& value) {
		}

		template <typename T>
		void AddWidget(const char *name, std::string *foo, T& value) {
			fl_font(FL_HELVETICA,12);
			Fl_Input * mInput = new Fl_Input(HorPos, VerPos, 330-fl_width(name), 20);
			mInput->label( name );
			mInput->value( value.c_str() );
			mInput->labelsize(12);
			mInput->textsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
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
		void AddWidget(const char *name, TData *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Float_Input * mInput = new Fl_Float_Input(HorPos, VerPos, 330-fl_width(name), 20);

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			mInput->label( name );
			mInput->labelsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
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

			Fl_Int_Input * mInput = new Fl_Int_Input(HorPos, VerPos, 330-fl_width(name), 20, name);

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			mInput->labelsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
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
			Fl_Light_Button * mInput = new Fl_Light_Button(70+(mWidgetNum/20)*340, VerPos, 200, 20);
			mInput->label( name );
			mInput->labelsize(12);
			mInput->value( value );
			add(*mInput);
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

			Fl_Choice* mChoice = new Fl_Choice(HorPos, VerPos, 330-fl_width(name), 20);
			mChoice->label( name );
			mChoice->labelsize(12);
			mChoice->textsize(12);
			mChoice->align(FL_ALIGN_LEFT);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mChoice->add( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mChoice->value(i);
			}
			add(*mChoice);
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
			fl_font(FL_HELVETICA,12);
			Fl_Button * mInput = new Fl_Button( /* 70+(mWidgetNum/20)*340*/ HorPos, VerPos, 330-fl_width(name), 20, "Details...");
			Fl_Box * mBox = new Fl_Box(3+(mWidgetNum/20)*340, VerPos, fl_width(name), 20, name);
			mInput->labelsize(12);
			mBox->labelsize(12);
			mInput->callback(ShowSubConfig);
			FLTKConfigurator * subConfigurator = new FLTKConfigurator;
			subConfigurator->SetConfig(value);
			mInput->user_data(subConfigurator);
			begin();
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
		}
		static void Discard(Fl_Widget* o, void* v) {
			o->window()->hide();
		}
		static void Ok(Fl_Widget* o, void* v) {
			Apply(o,v);
			Discard(o,v);
		}
		static void ShowSubConfig(Fl_Widget* o, void* v) {
			FLTKConfigurator * sub = (FLTKConfigurator*)(v);
			sub->show();
		}

		void FLTKConfigurator::show() {
			set_modal();
			super::show();
		}

	private:
		int mWidgetNum;
		ConfigurationVisitor * mGetter;
		ConfigurationVisitor * mSetter;
		tWidgets mWidgets;

	};
}
#endif//__FLTKCONFIGURATOR__
