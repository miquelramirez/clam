/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


#include <iostream>

void AnalyzeWindowBase::init()
{
}

void AnalyzeWindowBase::connect(const QObject * sender, const char * signal, const QObject * receiver, const char * member )
{
}

void AnalyzeWindowBase::emit_analyze()
{
    std::cout<<"emit analyze"<<std::endl;
}

void AnalyzeWindowBase::emit_analyzeAll()
{
    std::cout<<"emit analyze all"<<std::endl;
}






void AnalyzeWindowBase::close()
{

}


void AnalyzeWindowBase::newFunction()
{

}
