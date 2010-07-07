/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINI_C)
#define BOOST_SPIRIT_MINI_C

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_lit.hpp>

#include <boost/lexical_cast.hpp>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using boost::phoenix::function;
using boost::phoenix::ref;
using boost::phoenix::size;

using namespace boost::spirit;
using namespace boost::spirit::qi;

/**
 *
 */
typedef float operand_t;

typedef std::vector<operand_t> code_t;

typedef std::vector<operand_t> frame_t;
typedef std::vector<operand_t> stack_t;

typedef symbols<char, operand_t> vars_t;

struct function_info;
typedef symbols<char, function_info> functions_t;

///////////////////////////////////////////////////////////////////////////////
//  The Virtual Machine
///////////////////////////////////////////////////////////////////////////////
enum byte_code
{	
    op_neg,         //  negate the top stack entry
    op_add,         //  add top two stack entries
    op_sub,         //  subtract top two stack entries
    op_mul,         //  multiply top two stack entries
    op_div,         //  divide top two stack entries
    op_not,         //  boolean negate the top stack entry
    op_eq,          //  compare the top two stack entries for ==
    op_neq,         //  compare the top two stack entries for !=
    op_lt,          //  compare the top two stack entries for <
    op_lte,         //  compare the top two stack entries for <=
    op_gt,          //  compare the top two stack entries for >
    op_gte,         //  compare the top two stack entries for >=
    op_and,         //  logical and top two stack entries
    op_or,          //  logical or top two stack entries
    op_load,        //  load a variable
    op_store,       //  store a variable
    op_float,       //  push constant float into the stack
    op_true,        //  push constant 0 into the stack
    op_false,       //  push constant 1 into the stack
    op_jump_if,     //  jump to an absolute position in the code if top stack evaluates to false
    op_jump,        //  jump to an absolute position in the code
    op_stk_adj,     //  adjust the stack (for args and locals)
    op_call,        //  function call
    op_return,      //  return from function
		op_sin,
		op_cos,
		op_pow,
};

class vmachine
{
public:

    vmachine(unsigned stackSize = 4096) // lower this to 16 or so to for redeable debugging
      : stack(stackSize)
    {
    }

    operand_t execute(
        code_t const& code            // the program code
      , code_t::const_iterator pc     // program counter
      , frame_t::iterator frame_ptr   // start of arguments and locals
    );

    stack_t stack;
};

///////////////////////////////////////////////////////////////////////////////
//  A generic compiler that compiles 1 to 3 codes
///////////////////////////////////////////////////////////////////////////////
struct function_info
{
    int arity;
    int address;
};

struct compile_op
{
    template <typename A, typename B = unused_type, typename C = unused_type>
    struct result { typedef void type; };

    compile_op(code_t& code)
      : code(code)
    {
    }

    void operator()(int a) const
    {
        code.push_back(a);
    }

    void operator()(int a, int b) const
    {
        code.push_back(a);
        code.push_back(b);
		}

    void operator()(int a, float b) const
    {
        code.push_back(a);
        code.push_back(b);
    }

    void operator()(int a, int b, int c) const
    {
        code.push_back(a);
        code.push_back(b);
        code.push_back(c);
    }

    // special overload for function calls
    void operator()(function_info const& info, int got_nargs, bool& parse_result) const
    {
        if (got_nargs == info.arity)
        {
            code.push_back(op_call);
            code.push_back(info.arity);
            code.push_back(info.address);
        }
        else
        {
            parse_result = false; // fail the parse
            std::cerr << "wrong number of args" << std::endl;
        }
    }

    code_t& code;
};


///////////////////////////////////////////////////////////////////////////////
//  Our error handler
///////////////////////////////////////////////////////////////////////////////
struct error_handler_
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    template <typename Iterator>
    void operator()(
        info const& what
      , Iterator err_pos, Iterator last) const
    {
        std::cout
            << "Error! Expecting "
            << what                         // what failed?
            << " here: \""
            << std::string(err_pos, last)   // iterators to error-pos, end
            << "\""
            << std::endl
        ;
    }
};

function<error_handler_> const error_handler = error_handler_();

///////////////////////////////////////////////////////////////////////////////
//  A functor that adds variables to our (variables) symbol-table
///////////////////////////////////////////////////////////////////////////////
struct var_adder
{
    template <typename>
    struct result { typedef void type; };

    var_adder(vars_t& vars, int& nvars)
      : vars(vars), nvars(nvars)
    {
    }

    void operator()(std::string const& var) const
    {
        vars.add(var.begin(), var.end(), nvars++);
    };

    vars_t& vars;
    int& nvars;
};

///////////////////////////////////////////////////////////////////////////////
//  A functor that adds functions to our (function) symbol-table
///////////////////////////////////////////////////////////////////////////////
struct function_adder
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    function_adder(functions_t& functions)
      : functions(functions)
    {
    }

    void operator()(std::string const& function_id, int arity, int address) const
    {
        function_info info = {arity, address};
        functions.add(function_id.begin(), function_id.end(), info);
    };

    functions_t& functions;
};

///////////////////////////////////////////////////////////////////////////////
//  A functor that resets the function-related state variables
///////////////////////////////////////////////////////////////////////////////
struct function_state_reset
{
    template <typename>
    struct result { typedef void type; };

    function_state_reset(
        code_t& code
      , vars_t& vars
      , int& nvars)
      : code(code)
      , vars(vars)
      , nvars(nvars)
    {
    }

    void operator()(int address) const
    {
        code[address+1] = nvars;
        nvars = 0; // reset
        vars.clear(); // reset
    };

    code_t& code;
    vars_t& vars;
    int& nvars;
};

///////////////////////////////////////////////////////////////////////////////
//  White-space and comments grammar definition
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct white_space : grammar<Iterator>
{
    white_space() : white_space::base_type(start)
    {
        using boost::spirit::ascii::char_;

        start =
                space                               // tab/space/cr/lf
            |   "/*" >> *(char_ - "*/") >> "*/"     // C-style comments
            ;
    }

    rule<Iterator> start;
};

///////////////////////////////////////////////////////////////////////////////
//  Our expression grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct expression : grammar<Iterator, white_space<Iterator> >
{
    expression(
        code_t& code
      , vars_t& vars
      , functions_t& functions);

    typedef white_space<Iterator> white_space_;

    rule<Iterator, white_space_>
        expr, equality_expr, relational_expr
      , logical_expr, additive_expr, multiplicative_expr
      , unary_expr, primary_expr, variable
    ;

    rule<Iterator, white_space_> sine_function, cosine_function, pow_function
		;

    rule<Iterator, locals<function_info, int>, white_space_> function_call;

    code_t& code;
    vars_t& vars;
    functions_t& functions;
    function<compile_op> op;
};

///////////////////////////////////////////////////////////////////////////////
//  Our statement grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct statement : grammar<Iterator, white_space<Iterator> >
{
    statement(code_t& code, functions_t& functions);

    typedef white_space<Iterator> white_space_;

    code_t& code;
    vars_t vars;
    functions_t& functions;
    int nvars;
    bool has_return;

    expression<Iterator> expr;
    rule<Iterator, white_space_>
        statement_, statement_list, var_decl, compound_statement
      , return_statement;

    rule<Iterator, locals<int>, white_space_> if_statement;
    rule<Iterator, locals<int, int>, white_space_> while_statement;
    rule<Iterator, std::string(), white_space_> identifier;
    rule<Iterator, int(), white_space_> var_ref;
    rule<Iterator, locals<int>, white_space_> assignment;
    rule<Iterator, void(int), white_space_> assignment_rhs;

    function<var_adder> add_var;
    function<compile_op> op;
};

///////////////////////////////////////////////////////////////////////////////
//  Our program grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct program : grammar<Iterator, white_space<Iterator> >
{
    program(code_t& code);

    typedef white_space<Iterator> white_space_;

    code_t& code;
    rule<Iterator, std::string(), white_space_> identifier;
    rule<Iterator, white_space_> start;

    typedef
        locals<
            std::string // function name
          , int         // address
        >
    function_locals;

    functions_t functions;
    statement<Iterator> statement_;

    rule<Iterator, function_locals, white_space_> function;
    boost::phoenix::function<function_adder> add_function;
    boost::phoenix::function<function_state_reset> state_reset;
    boost::phoenix::function<compile_op> op;
};

#endif


