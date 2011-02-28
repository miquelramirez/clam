#ifndef PluginGui_h
#define PluginGui_h

#include <gtk/gtk.h>
#include <lv2.h>
#include "lv2_ui.h"
#include <QtGui/QX11EmbedWidget>
#include <QtGui/QDoubleSpinBox>
#include <CLAM/Assert.hxx>

class Lv2GuiContainer;

class PluginGui : public QX11EmbedWidget
{
	Q_OBJECT

public:
	QWidget   *	qtWidget;
	GtkWidget *	widgetSocket;

	std::vector<Lv2GuiContainer*> containers;

	LV2UI_Write_Function 	write_function;
	LV2UI_Controller 	controller;
	const char * 		bundlePath;
	
	PluginGui()
	{}
	
	void setValueControlAtPlugin(unsigned, double);
	void setValueControlAtWidget(unsigned, double);
};

/**************************************************/

class Lv2GuiContainer : public QObject
{
	Q_OBJECT

public:
	Lv2GuiContainer(QObject * parent = 0)
			: QObject(parent)
	{}

public slots:
	virtual void setValueAtPlugin(double)=0;
	virtual void setValueAtWidget(unsigned,double)=0;
};

/**************************************************/

class Lv2DoubleSpinBox : public Lv2GuiContainer
{
	Q_OBJECT
	
	unsigned _id;
	PluginGui * _pgui;
	QDoubleSpinBox * _doubleSpinBox;

public:
	Lv2DoubleSpinBox( unsigned id
			, QDoubleSpinBox * _doubleSpinBox
			, PluginGui * pgui)
			: Lv2GuiContainer(pgui)
			, _id(id)
			, _pgui(pgui)
			, _doubleSpinBox(_doubleSpinBox) 
	{
		CLAM_ASSERT(pgui,"The PluginGui cannot be 0");		

		connect(_doubleSpinBox, SIGNAL(valueChanged(double)),
			this, SLOT(setValueAtPlugin(double)));
	}

public slots:
	virtual void setValueAtPlugin(double value)
	{
		_pgui->setValueControlAtPlugin(_id, value);
	}
	virtual void setValueAtWidget(unsigned id, double value)
	{
		if(id==_id)
			_doubleSpinBox->setValue(value);
	}
};

#endif
