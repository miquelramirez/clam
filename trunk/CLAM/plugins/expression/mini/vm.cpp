/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "definitions.hpp"

#include <cmath>
#include <algorithm>

# pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
                                // (performance warning)

operand_t vmachine::execute(
    std::vector<int> const& code
  , std::vector<int>::const_iterator pc
  , std::vector<operand_t>::iterator frame_ptr
)
{
	
		//std::copy(v.begin(),v.end(),std::ostream_iterator<T>(std::cout,"\n"))
	
    std::vector<operand_t>::iterator stack_ptr = frame_ptr;

    while (true)
    {
        BOOST_ASSERT(pc != code.end());

        switch (*pc++)
        {
            case op_neg:
                stack_ptr[-1] = -stack_ptr[-1];
                break;

            case op_not:
                stack_ptr[-1] = !bool(stack_ptr[-1]);
                break;

            case op_add:
                --stack_ptr;
                stack_ptr[-1] = operand_t(stack_ptr[-1] + stack_ptr[0]);
                break;

            case op_sub:
                --stack_ptr;
                stack_ptr[-1] = operand_t(stack_ptr[-1] - stack_ptr[0]);
                break;

            case op_mul:
                --stack_ptr;				
#ifdef 0					
                std::cout << "a=" << stack_ptr[-1] << " b=" << stack_ptr[0] << " a*b=" << operand_t(stack_ptr[-1] * stack_ptr[0]) << std::endl;
#endif								
                stack_ptr[-1] = operand_t(stack_ptr[-1] * stack_ptr[0]);
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
                --stack_ptr;
#if 0						
                std::cout << "val=" << stack_ptr[0] << " sin=" << sin(stack_ptr[0]) << " code=" <<  *(pc)<< " frame=" << frame_ptr[*(pc)] << std::endl;
#endif		
                stack_ptr[-1] = sin(stack_ptr[0]);
                break;

            case op_cos:
                --stack_ptr;
#if 0
                std::cout << "val=" << stack_ptr[0] << " cos=" << cos(stack_ptr[0]) << " code=" <<  *(pc)<< " frame=" << frame_ptr[*(pc)] << std::endl;
#endif		
                stack_ptr[-1] = cos(stack_ptr[0]);
                break;

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
                    int r = execute(
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

