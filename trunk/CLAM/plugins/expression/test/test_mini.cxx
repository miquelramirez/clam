//
//
//

#include "MiniCppUnit.hxx"

#include "definitions.hpp"
#include "main.hpp"

#include <vector>
#include <boost/assign/std/vector.hpp>
using namespace boost::assign;

typedef std::vector<operand_t> input_t;

#include <cmath>
#include <limits>

namespace {
	
bool equal(operand_t a, operand_t b) {
    return fabs(a - b) < std::numeric_limits<operand_t>::epsilon();
}

} // anon.

class mini_test : public TestFixture<mini_test>
{
public:
	TEST_FIXTURE( mini_test )
	{
		TEST_CASE( test_three_params );
		TEST_CASE( test_assignment );
		TEST_CASE( test_if );
		TEST_CASE( test_if_and );
		TEST_CASE( test_operator_precedence );
		TEST_CASE( test_while );
		TEST_CASE( test_sine );
		TEST_CASE( test_cosine );
		TEST_CASE( test_sin_cos );
		TEST_CASE( test_sin_cos_float );
		TEST_CASE( test_pow );
		TEST_CASE( test_log );
		TEST_CASE( test_floor );
		//TEST_CASE( test_simple );
		//TEST_CASE( test_ternary );
	}
	
	float get_computed_value(std::string const& source_code, input_t const& in)
	{
		typedef std::string::const_iterator iterator_type;
		typedef program<iterator_type> program;

		vmachine mach;              //  Our virtual machine
		code_t code;      //  Our VM code
		program prog(code);         //  Our grammar definition

		ASSERT(compile(prog, source_code));
			
		mach.stack = in;

		float r = mach.execute(code, code.begin(), mach.stack.begin());
		std::cout << "r=" << r << std::endl;
		
		return r;
	}

	void test_three_params()
  {
		std::string source_code =
		"float my_function(a, b, c)"
		"{"
		""
		"	return (a + b) - c;"
		""
		"}"
		;
		
		input_t input;
		input += 1,2,3; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(0, r));
  }
	
  void test_assignment()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	float c = 2;"
		""
		"	return a + b * c;"
		"}"
		;

		input_t input;
		input += 2, 0.5; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(3, r));
  }
	
	void test_if()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	if (a == 2)"
		"		a = a * b;"
		"	else"
		"		a = a + 10;"
		""
		"	return a + b;"
		"}"
		;
		
		input_t input;
		input += 2, 0.5; 

		float r = get_computed_value(source_code, input);

		ASSERT(equal(1.5, r));
  }
  void test_if_and()
  {
		std::string source_code =
		"float my_function(a,b)"
		"{"
		""
		"	if (a && b)"
		"		return 1;"
		"	else"
		"		return 0;"
		""
		"}"
		;
		
		input_t input;
		input += 0, 0;
 		float r = get_computed_value(source_code, input);
		ASSERT(equal(0, r));

		input.clear();
		input += 0, 1;
		r=get_computed_value(source_code, input);
		ASSERT(equal(0, r));

		input.clear();
		input += 1, 0;
		r=get_computed_value(source_code, input);
		ASSERT(equal(0, r));

		input.clear();
		input += 1, 1;
		r=get_computed_value(source_code, input);
		ASSERT(equal(1, r));

  }
  void test_operator_precedence()
  {
		std::string source_code =
		"float my_function(a)"
		"{"
		""
		"	if (a>0 && a<1)"
		"		return 1;"
		"	else"
		"		return 0;"
		""
		"}"
		;
		
		input_t input;
		input += 0.5;
 		float r = get_computed_value(source_code, input);
		ASSERT(equal(1, r));
  }
	void test_while()
		
  {
		std::string source_code =
		"float my_function()"
		"{"
		""
		"	float n = 0;"
		"	while (n < 10)"
		"	{"
		"		n = n + 1;"
		"	}"
		"	return n;"
		"}"
		;
		
		input_t input;
		float r = get_computed_value(source_code, input);

		ASSERT(equal(10, r));
  }
	
  void test_sine()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	return a + b * sin(0);"
		""
		"}"
		;
		
		input_t input;
		input += 2, 0.5; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(2, r));
  }

	void test_cosine()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	return a + b / cos(0);"
		""
		"}"
		;
		
		input_t input;
		input += 2, 0.5; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(2.5, r));
		
  }
	
	void test_sin_cos()
  {
		std::string source_code =
		"float my_function(a, b, c)"
		"{"
		""
		"	return (1+2+3+cos(b)+cos(c))*sin(a);"
		""
		"}"
		;
		
		input_t input;
		input += 0,0,0; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(0, r));
  }

	void test_sin_cos_float()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	return cos(a)*sin(0.5)+b;"
		""
		"}"
		;
		
		input_t input;
		input += 0,2; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(2.47942554, r));
  }
	
	void test_pow()
  {
		std::string source_code =
		"float my_function(a, b)"
		"{"
		""
		"	return pow(a,b);"
		""
		"}"
		;
		
		input_t input;
		input += 2,2; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(4, r));
  }
	
	void test_log()
  {
		std::string source_code =
		"float my_function(a)"
		"{"
		""
		"	return log(a);"
		""
		"}"
		;
		
		input_t input;
		input += 2.3; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(0.832909123, r));
  }
	
	void test_floor()
  {
		std::string source_code =
		"float my_function(a)"
		"{"
		""
		"	return floor(a);"
		""
		"}"
		;
		
		input_t input;
		input += 2.832909123; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(equal(2, r));
  }
	
	void test_simple()
  {		
		typedef std::string::const_iterator iterator_type;
    symbols<char, function_info> functions;
    code_t code;
    statement<iterator_type> prog(code, functions);

		std::string source_code =
		"{1 + 1;}"
		;
		
		ASSERT(compile(prog, source_code));
			
		input_t input;
		input += 1,0,0; 
		
		vmachine mach;  
		mach.stack = input;

		float r = mach.execute(code, code.begin(), mach.stack.begin());
		
		ASSERT(equal(2, r));
  }

#if 0
	void test_ternary()
  {
		std::string source_code =
		"float my_function(a, b, c)"
		"{"
		""
		"	return a < b ? 1 : c;"
		""
		"}"
		;
		
		input_t input;
		input += 1,0,0; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(0. == r);
  }
#endif
	
};

REGISTER_FIXTURE(mini_test)

