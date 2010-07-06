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

class mini_test : public TestFixture<mini_test>
{
public:
	TEST_FIXTURE( mini_test )
	{
		TEST_CASE( test_three_params );
		TEST_CASE( test_assignment );
		TEST_CASE( test_if );
		TEST_CASE( test_while );
		//TEST_CASE( test_sine );
		//TEST_CASE( test_cosine );
		//TEST_CASE( test_sin_cos );
		//TEST_CASE( test_simple );
	}
	
	float get_computed_value(std::string const& source_code, input_t const& in)
	{
		typedef std::string::const_iterator iterator_type;
		typedef program<iterator_type> program;

		vmachine mach;              //  Our virtual machine
		std::vector<int> code;      //  Our VM code
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
		
		ASSERT(0 == r);
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
		
		ASSERT(3.0 == r);
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

		ASSERT(1.5 == r);
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

		ASSERT(10.0 == r);
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
		
		ASSERT(2. == r);
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
		
		ASSERT(2.5 == r);
		
  }
	
	void test_sin_cos()
  {
		std::string source_code =
		"float my_function(a, b, c)"
		"{"
		""
		"	return cos(0) + 1;"
		""
		"}"
		;
		
		input_t input;
		input += 1,0,0; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(1.0 == r);
  }
	
	void test_simple()
  {
		std::string source_code =
		"	return cos(0) + 1;"
		;
		
		input_t input;
		input += 1,0,0; 

		float r = get_computed_value(source_code, input);
		
		ASSERT(2.0 == r);
  }
	
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

		float r = get_computed_value(source_code_three_params, input);
		
		ASSERT(0. == r);
  }
};

REGISTER_FIXTURE(mini_test)

