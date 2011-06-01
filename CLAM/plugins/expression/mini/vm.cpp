/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "definitions.hpp"

#include <cmath>
#include <algorithm>

#ifdef _WIN32
#pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'  (performance warning)
#endif

/*
Some notes on the added sin/cos

model expression:

    return (1+2+3+cos(0)+cos(0))*sin(0);
		
-------------------------
Parsing succeeded
-------------------------

good!

bytecode:

code=21 0 16 1 16 2 1 16 3 1 16 0 25 1 16 0 25 1 16 0 24 3 23 

stack:

stack=1.02295e-43 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 1 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 3 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 3 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 6 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
val=0 cos=1 code=25
stack=1.02295e-43 6 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 7 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 7 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
val=0 cos=1 code=25
stack=1.02295e-43 7 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 8 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
val=0 sin=0 code=24
stack=1.02295e-43 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
stack=1.02295e-43 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

-------------------------
Result: 0
-------------------------

correct!

The trick is to adjust the stack correctly: 
- after each op_float ('16') which  loads a float from the code to the stack we lower the stack_ptr
- then we do sin/cos in place, cos we want to alter the value
- then we raise the stack_ptr so the next op_add ('1') or op_mul ('3') can lower it and perform their ops

*/

operand_t vmachine::execute(
    code_t const& code
  , code_t::const_iterator pc
  , frame_t::iterator frame_ptr
)
{
#if 0
		std::cout << "code=";
		std::copy(code.begin(),code.end(),std::ostream_iterator<operand_t>(std::cout," "));
		std::cout << std::endl;
#endif
	
    frame_t::iterator stack_ptr = frame_ptr;
	
    while (true)
    {
        BOOST_ASSERT(pc != code.end());

#if 0
				std::cout << "stack=";
				std::copy(stack.begin()-1,stack.end(),std::ostream_iterator<operand_t>(std::cout," "));
				std::cout << std::endl;
#endif
			
        switch ((int)*pc++)
        {
            case op_neg:
                stack_ptr[-1] = -stack_ptr[-1];
                break;

            case op_not:
                stack_ptr[-1] = !bool(stack_ptr[-1]);
                break;

            case op_add:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] + stack_ptr[0];
                break;

            case op_sub:
                --stack_ptr;
                stack_ptr[-1] = stack_ptr[-1] - stack_ptr[0];
                break;

            case op_mul:
                --stack_ptr;				
#if 0					
                std::cout << "a=" << stack_ptr[-1] 
									<< " b=" << stack_ptr[0] 
									<< " a*b=" << stack_ptr[-1] * stack_ptr[0] 
									<< std::endl;
#endif								
                stack_ptr[-1] = stack_ptr[-1] * stack_ptr[0];
                break;

            case op_div:
                --stack_ptr;
                stack_ptr[-1] = operand_t(stack_ptr[-1] / stack_ptr[0]);
                break;

            case op_eq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] == stack_ptr[0]);
                break;

            case op_neq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] != stack_ptr[0]);
                break;

            case op_lt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] < stack_ptr[0]);
                break;

            case op_lte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] <= stack_ptr[0]);
                break;

            case op_gt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] > stack_ptr[0]);
                break;

            case op_gte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] >= stack_ptr[0]);
                break;

            case op_and:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) && bool(stack_ptr[0]);
                break;

            case op_or:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) || bool(stack_ptr[0]);
                break;
						
            case op_sin:
						{
                --stack_ptr;
#if 0						
                std::cout << "val=" << *stack_ptr
									<< " sin=" << sin(*stack_ptr) 
									<< " code=" <<  *(pc-1)
									<< std::endl;
#endif		
								operand_t val = sin(*stack_ptr);
								*stack_ptr = val;
						
								++stack_ptr;
								break;
						}
						
            case op_cos:
						{	
								--stack_ptr;
								operand_t val = cos(*stack_ptr);
								*stack_ptr = val;
				
								++stack_ptr;
								break;
						}

            case op_sqrt:
						{	
								--stack_ptr;
								operand_t val = sqrt(*stack_ptr);
								*stack_ptr = val;
				
								++stack_ptr;
								break;
						}

            case op_pow:
						{	
								operand_t exponent = *--stack_ptr;
								operand_t base = *--stack_ptr;
								*stack_ptr = pow(base, exponent);
#if 0
								std::cout << "val=" << *stack_ptr
									<< " exponent=" << exponent
									<< " base=" << base 
									<< " code=" <<  *(pc-1) 
									<< std::endl;
#endif		
								++stack_ptr;
								break;
						}
						
						case op_log:
						{	
								operand_t val = *--stack_ptr;
								*stack_ptr = log(val);

								++stack_ptr;
								break;
						}
						
						case op_floor:
						{	
								operand_t val = *--stack_ptr;
								*stack_ptr = floor(val);

								++stack_ptr;
								break;
						}
												
            case op_load:
                *stack_ptr++ = frame_ptr[*pc++];
                break;

            case op_store:
                --stack_ptr;
                frame_ptr[*pc++] = stack_ptr[0];
                break;

            //case op_int:
            case op_float:
                *stack_ptr++ = *pc++;
                break;

            case op_true:
                *stack_ptr++ = true;
                break;

            case op_false:
                *stack_ptr++ = false;
                break;

            case op_jump:
                pc = code.begin() + *pc;
                break;

            case op_jump_if:
                if (!bool(stack_ptr[-1]))
                    pc = code.begin() + *pc;
                else
                    ++pc;
                --stack_ptr;
                break;

            case op_stk_adj:
                stack_ptr += *pc++;
                break;

            case op_call:
                {
                    int nargs = *pc++;
                    int jump = *pc++;

                    // a function call is a recursive call to execute
                    operand_t r = execute(
                        code
                      , code.begin() + jump
                      , stack_ptr - nargs
                    );

                    // cleanup after return from function
                    stack_ptr[-nargs] = r;      //  get return value
                    stack_ptr -= (nargs - 1);   //  the stack will now contain
                                                //  the return value
                }
                break;

            case op_return:
                return stack_ptr[-1];
        }
    }
}

