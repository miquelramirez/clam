#include "AnalyzeWindow.hxx"

AnalyzeWindow::AnalyzeWindow(QWidget * parent, const char * name, WFlags f) : AnalyzeWindowBase (parent, name, f)
{
	table1->setLeftMargin(0);
	QCheckTableItem * checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(0,0,"Pitch");
	table1->setText(0,1,"Not analyzed");
	table1->setItem(0,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(1,0,"Energy");
	table1->setText(1,1,"Not Analyzed");
	table1->setItem(1,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(2,0,"Centroid");
	table1->setText(2,1,"Not Analyzed");
	table1->setItem(2,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(3,0,"SpecSpread");
	table1->setText(3,1,"Not Analyzed");
	table1->setItem(3,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(4,0,"SpecDev");
	table1->setText(4,1,"Not Analyzed");
	table1->setItem(4,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(5,0,"SpecVar");
	table1->setText(5,1,"Not Analyzed");
	table1->setItem(5,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(6,0,"SpecFlatness");
	table1->setText(6,1,"Not Analyzed");
	table1->setItem(6,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(7,0,"SpecKurtosis");
	table1->setText(7,1,"Not Analyzed");
	table1->setItem(7,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);
	table1->setText(8,0,"SpecSkewness");
	table1->setText(8,1,"Not Analyzed");
	table1->setItem(8,2,checkitem);
	checkitem=new QCheckTableItem(table1,"");
	checkitem->setChecked(true);	
	table1->setText(9,0,"SpecRolloff");
	table1->setText(9,1,"Not Analyzed");
	table1->setItem(9,2,checkitem);


}

