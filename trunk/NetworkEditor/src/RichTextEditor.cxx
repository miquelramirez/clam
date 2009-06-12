#include <iostream>
#include "RichTextEditor.hxx"
#include <QtGui/QTextList>

using namespace std;

RichTextEditor::RichTextEditor()
{
	_layout=new QVBoxLayout(this);
	
	_textEdit = new QTextEdit(this);

	connect(_textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));

	connect(_textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
	        this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
	connect(_textEdit, SIGNAL(cursorPositionChanged()),
	        this, SLOT(cursorPositionChanged()));
	
	this->setupTextActions();

	_layout->addWidget(_textEdit);
}

RichTextEditor::RichTextEditor(QWidget * parent): QWidget(parent)
{
	_layout=new QVBoxLayout(this);
	
	_textEdit = new QTextEdit(this);

	connect(_textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));

	connect(_textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
	        this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
	connect(_textEdit, SIGNAL(cursorPositionChanged()),
	        this, SLOT(cursorPositionChanged()));
	
	this->setupTextActions();

	_layout->addWidget(_textEdit);
}

RichTextEditor::~RichTextEditor()
{
}

void RichTextEditor::setupTextActions()
{
	_toolBar = new QToolBar(this);
	_toolBar->setWindowTitle(tr("Format Actions"));
	_layout->addWidget(_toolBar);

	_actionTextBold = new QAction(QIcon(":/icons/images/text_bold.png"),tr("&Bold"), this);
	_actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
	_actionTextBold->setCheckable(true);
	_toolBar->addAction(_actionTextBold);
	connect(_actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));

	_actionTextUnderline = new QAction(QIcon(":/icons/images/text_under.png"),tr("&Underline"), this);
	_actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	_actionTextUnderline->setCheckable(true);
	_toolBar->addAction(_actionTextUnderline);
	connect(_actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));

	_actionTextItalic = new QAction(QIcon(":/icons/images/text_italic.png"),tr("&Italic"), this);
	_actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	_actionTextItalic->setCheckable(true);
	_toolBar->addAction(_actionTextItalic);
	connect(_actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));

	QActionGroup *grp = new QActionGroup(this);
	connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(textAlign(QAction *)));

	if (QApplication::isLeftToRight()) {
		_actionAlignLeft = new QAction(QIcon(":/icons/images/text_left.png"), tr("&Left"), grp);
		_actionAlignCenter = new QAction(QIcon(":/icons/images/text_center.png"), tr("C&enter"), grp);
		_actionAlignRight = new QAction(QIcon(":/icons/images/text_right.png"), tr("&Right"), grp);
	} else {
		_actionAlignRight = new QAction(QIcon(":/icons/images/text_right.png"), tr("&Right"), grp);
		_actionAlignCenter = new QAction(QIcon(":/icons/images/text_center.png"), tr("C&enter"), grp);
		_actionAlignLeft = new QAction(QIcon(":/icons/images/text_left.png"), tr("&Left"), grp);
	}
	_actionAlignJustify = new QAction(QIcon(":/icons/images/text_block.png"), tr("&Justify"), grp);

	_actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	_actionAlignLeft->setCheckable(true);
	_toolBar->addAction(_actionAlignLeft);
	_actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	_actionAlignCenter->setCheckable(true);
	_toolBar->addAction(_actionAlignCenter);
	_actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	_actionAlignRight->setCheckable(true);
	_toolBar->addAction(_actionAlignRight);
	_actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
	_actionAlignJustify->setCheckable(true);
	_toolBar->addAction(_actionAlignJustify);

	QPixmap pix(16, 16);
	pix.fill(Qt::black);
	_actionTextColor = new QAction(pix, tr("&Color..."), this);
	connect(_actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
	_toolBar->addAction(_actionTextColor);

	_toolBar = new QToolBar(this);
	_toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	_toolBar->setWindowTitle(tr("Format Actions"));
	_layout->addWidget(_toolBar);

	_comboStyle = new QComboBox(_toolBar);
	_toolBar->addWidget(_comboStyle);
	_comboStyle->addItem("Standard");
	_comboStyle->addItem("Bullet List (Disc)");
	_comboStyle->addItem("Bullet List (Circle)");
	_comboStyle->addItem("Bullet List (Square)");
	_comboStyle->addItem("Ordered List (Decimal)");
	_comboStyle->addItem("Ordered List (Alpha lower)");
	_comboStyle->addItem("Ordered List (Alpha upper)");
	connect(_comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

	_comboFont = new QFontComboBox(_toolBar);
	_toolBar->addWidget(_comboFont);
	connect(_comboFont, SIGNAL(activated(const QString &)),
	        this, SLOT(textFamily(const QString &)));

	_comboSize = new QComboBox(_toolBar);
	_comboSize->setObjectName("comboSize");
	_toolBar->addWidget(_comboSize);
	_comboSize->setEditable(true);

	QFontDatabase db;
	foreach(int size, db.standardSizes())
	    _comboSize->addItem(QString::number(size));

	connect(_comboSize, SIGNAL(activated(const QString &)),this, SLOT(textSize(const QString &)));
	_comboSize->setCurrentIndex(_comboSize->findText(QString::number(QApplication::font().pointSize())));
}

void RichTextEditor::textBold()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(_actionTextBold->isChecked()?QFont::Bold:QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditor::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(_actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditor::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(_actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditor::textAlign(QAction *a)
{
	if (a == _actionAlignLeft)
		_textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == _actionAlignCenter)
		_textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == _actionAlignRight)
		_textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == _actionAlignJustify)
		_textEdit->setAlignment(Qt::AlignJustify);
}

void RichTextEditor::textColor()
{
	QColor col = QColorDialog::getColor(_textEdit->textColor(), this);
	if (!col.isValid())
		return;
	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);
	colorChanged(col);
}

void RichTextEditor::colorChanged(const QColor &c)
{
	QPixmap pix(16, 16);
	pix.fill(c);
	_actionTextColor->setIcon(pix);
}

void RichTextEditor::textStyle(int styleIndex)
{
	QTextCursor cursor = _textEdit->textCursor();

	if (styleIndex != 0)
	{
			QTextListFormat::Style style = QTextListFormat::ListDisc;

		switch (styleIndex)
		{
			default:
			case 1:
				style = QTextListFormat::ListDisc;
			break;
			case 2:
				style = QTextListFormat::ListCircle;
			break;
			case 3:
				style = QTextListFormat::ListSquare;
			break;
			case 4:
				style = QTextListFormat::ListDecimal;
			break;
			case 5:
				style = QTextListFormat::ListLowerAlpha;
			break;
			case 6:
				style = QTextListFormat::ListUpperAlpha;
			break;
		}

		cursor.beginEditBlock();

		QTextBlockFormat blockFmt = cursor.blockFormat();

		QTextListFormat listFmt;

		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		}
		else
		{
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}

		listFmt.setStyle(style);

		cursor.createList(listFmt);

		cursor.endEditBlock();
	}
	else
	{
		// ####
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}

void RichTextEditor::textFamily(const QString &f)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditor::textSize(const QString &p)
{
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void RichTextEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
	QTextCursor cursor = _textEdit->textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	_textEdit->mergeCurrentCharFormat(format);
}

void RichTextEditor::currentCharFormatChanged(const QTextCharFormat &format)
{
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

void RichTextEditor::cursorPositionChanged()
{
	alignmentChanged(_textEdit->alignment());
}

void RichTextEditor::fontChanged(const QFont &f)
{
	_comboFont->setCurrentIndex(_comboFont->findText(QFontInfo(f).family()));
	_comboSize->setCurrentIndex(_comboSize->findText(QString::number(f.pointSize())));
	_actionTextBold->setChecked(f.bold());
	_actionTextItalic->setChecked(f.italic());
	_actionTextUnderline->setChecked(f.underline());
}

void RichTextEditor::alignmentChanged(Qt::Alignment a)
{
	if (a & Qt::AlignLeft) {
		_actionAlignLeft->setChecked(true);
	} else if (a & Qt::AlignHCenter) {
		_actionAlignCenter->setChecked(true);
	} else if (a & Qt::AlignRight) {
		_actionAlignRight->setChecked(true);
	} else if (a & Qt::AlignJustify) {
		_actionAlignJustify->setChecked(true);
	}
}

QString RichTextEditor::toHtml()
{
	return _textEdit->toHtml();
}

void RichTextEditor::setText(const QString & text)
{
	_textEdit->setText(text);
}
