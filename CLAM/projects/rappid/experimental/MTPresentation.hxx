#ifndef _MTPRESENTATION_H
#define _MTPRESENTATION_H


extern "C" {
#include <pthread.h>
};

#include "VertexFormats.hxx"

namespace CLAMGUI {

	class MTPresentation {


	public:

		typedef enum {
			eRealTime,
			eBlocking,
			eStepByStep,
		} EPolicy;

		// GUI TRHEAD: Configuration requests.

		virtual void Draw(void) = 0;

		virtual void SetVisibleXRange(GLfloat min, GLfloat width) = 0;

		virtual void SetVisibleYRange(GLfloat min, GLfloat width) = 0;

		virtual void SetVisibleRanges(GLfloat x_min, GLfloat x_width,
									  GLfloat y_min, GLfloat y_width) = 0;

		virtual void Togle(int element, int representation) = 0;

		virtual void SetColor(int element, int representation, c3f color) = 0;

		virtual void SetWidth(int element, int representation, GLfloat width) = 0;

		virtual void SetStiple(int element, int representation,
							   bool active, GLint factor, GLushort pattern) = 0;

		void Step(); // GUI THREAD. Step request.

		void TogleFreeze(); // GUI THREAD. Freeze request.

		virtual ~MTPresentation();

	protected:

		MTPresentation(EPolicy policy = eBlocking);

		void Lock();           // Lock request for the GUI.

		bool WaitStepAndTryLock(); // Lock request for MODEL THREAD

		void Unlock();

		bool Frozen() const {return mFrozen;}
		
	private:

		EPolicy mPolicy;

		pthread_mutex_t mMutex;

		bool            mFrozen;

		bool            mFirstStep;
		pthread_cond_t  mStepRequest;
		pthread_mutex_t mStepMutex;

	};



}



#endif
