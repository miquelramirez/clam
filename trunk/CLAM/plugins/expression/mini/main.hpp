/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Not a calculator anymore, right? :-)
//
//  [ JDG April 10, 2007 ]       spirit2
//
///////////////////////////////////////////////////////////////////////////////
#include "definitions.hpp"

///////////////////////////////////////////////////////////////////////////////
//  Our main compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Grammar>
bool compile(Grammar const& prog, std::string const& expr)
{
    typedef white_space<std::string::const_iterator> white_space;
    white_space ws; //  Our skipper

    std::string::const_iterator iter = expr.begin();
    std::string::const_iterator end = expr.end();
    bool r = phrase_parse(iter, end, prog, ws);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n\n" << expr;
        std::cout << "-------------------------\n";
        return true;
    }
    else
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n\n" << expr;
        std::cout << "-------------------------\n";
        return false;
    }
}

