#ifndef __GLSAMPLEDETAILARRAY__
#define __GLSAMPLEDETAILARRAY__

#include "GLStraightLineArray.hxx"

namespace CLAMVM
{

class GLSampleDetailArray
		: public GLStraightLineArray
{
public:

		GLSampleDetailArray();

		~GLSampleDetailArray();

		void ExecuteGLCommands();
};

}

#endif // GLSampleDetailArray.hxx
