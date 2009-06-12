#ifndef RichTextEditor_hxx
#define RichTextEditor_hxx

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QColorDialog>
#include <QtGui/QComboBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextCursor>
#include <QtGui/QFontDatabase>
#include <QtGui/QApplication>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>


class QFontComboBox;
class QTextCharFormat;
class QComboBox;

class RichTextEditor: public QWidget
{
	Q_OBJECT
	
	public:
		RichTextEditor();
		RichTextEditor(QWidget * parent=0);
		~RichTextEditor();
	
	public slots:
		void textBold();
		void textUnderline();
		void textItalic();
		void textAlign(QAction *a);
		void textColor();
		void textStyle(int styleIndex);
		void textFamily(const QString &f);
		void textSize(const QString &p);
		void currentCharFormatChanged(const QTextCharFormat &format);
		void cursorPositionChanged();
		QString toHtml();
		void setText(const QString & text);
		
	signals:
		void textChanged();

	private:
	
		void setupTextActions();
		void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
		void colorChanged(const QColor &c);
		void fontChanged(const QFont &f);
		void alignmentChanged(Qt::Alignment a);
	
		QTextEdit * _textEdit;
		QVBoxLayout * _layout;
				
		QAction * _actionTextBold;
		QAction * _actionTextUnderline;
		QAction * _actionTextItalic;
		QAction * _actionTextColor;
		QAction * _actionAlignLeft;
		QAction * _actionAlignCenter;
		QAction * _actionAlignRight;
		QAction * _actionAlignJustify;
		
		QComboBox *_comboStyle;
		QToolBar * _toolBar;
		QFontComboBox *_comboFont;
		QComboBox *_comboSize;
};

#endif
