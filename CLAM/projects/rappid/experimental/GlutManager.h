#ifndef _GLUT_MANAGER_H
#define _GLUT_MANAGER_H

#include<pthread.h>
#include<vector>
#include "CBL.hxx"

class GlutWindow;


class GlutManager {

public:

	typedef CBL::Functor0 IdleCb;
	typedef CBL::Functor0 DisplayCb;
	typedef CBL::Functor1<int> TimerCb;
	typedef CBL::Functor2<int,int> MotionCb;
	typedef CBL::Functor2<int,int> ReshapeCb;
	typedef CBL::Functor4<int,int,int,int> MouseCb;
	typedef CBL::Functor3<unsigned char,int,int> KeyboardCb;

	struct Callbacks {
		IdleCb     mIdle;
		DisplayCb  mDisplay;
		TimerCb    mTimer;
		MotionCb   mMotion;
		ReshapeCb  mReshape;
		MouseCb    mMouse;
		KeyboardCb mKeyboard;
	};

	static GlutManager* Instance();

	static void Loop();

	int  Open(GlutWindow*, const char*);

	void Close(int window_id);

	static void Start();

private:

	GlutManager();

	static GlutManager *mInstance;

	static int mNumWindows;

	static std::vector<GlutWindow*> mWindows;

	static bool mRunning;

	static pthread_t mThread;

	static void GlutIdleCB(void);
	static void GlutTimerCB(int);
	static void GlutDisplayCB(void);
	static void GlutMotionCB(int x, int y);
	static void GlutReshapeCB(int w, int h);
	static void GlutMouseCB(int button, int state, int x, int y);
	static void GlutKeyboardCB(unsigned char key, int x, int y);

	static void* MainLoop(void*);

};



#endif
