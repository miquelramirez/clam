#include "ListViewItem.hxx"

void ListViewItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
    if (row_%2==0)
    {
	const QColorGroup *newCG=new QColorGroup(QBrush(cg.foreground()),QBrush(cg.button()),QBrush(cg.light()),QBrush(cg.dark()),QBrush(cg.mid()),QBrush(cg.text()),QBrush(cg.brightText()),QBrush(QColor(245,245,245)),QBrush(cg.background()));
	Q3ListViewItem::paintCell(p,*newCG,column,width,align);
    }
    else
    {
	const QColorGroup *newCG=new QColorGroup(QBrush(cg.foreground()),QBrush(cg.button()),QBrush(cg.light()),QBrush(cg.dark()),QBrush(cg.mid()),QBrush(cg.text()),QBrush(cg.brightText()),QBrush(cg.base()),QBrush(cg.background()));
	
	Q3ListViewItem::paintCell(p,*newCG,column,width,align);
    }
}

void ListViewItem::init()
{
    
}
