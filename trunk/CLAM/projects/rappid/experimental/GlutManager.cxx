
#include "GlutManager.h"
#include "GlutWindow.h"
#include <GL/glut.h>



GlutManager::GlutManager()
{

	static int argc = 1;

	static char *argv[4] = { "application" , 0, 0, 0};

//	glutInit(&argc,argv);
	
	mWindows.reserve(10);

}

int GlutManager::mNumWindows = 0;

GlutManager *GlutManager::mInstance = 0;

std::vector<GlutWindow*> GlutManager::mWindows;

GlutManager *GlutManager::Instance()
{
	if (!mInstance)
		mInstance = new GlutManager();
	return mInstance;
}

void GlutManager::Loop()
{
	glutMainLoop();
}

int GlutManager::Open(GlutWindow* win,const char *title = "CLAM Display")
{
	if (win->Id() != 0)
		return win->Id();

	int id = glutCreateWindow(title);
//	glutEnterGameMode();
//	int id = glutGetWindow();

	glutDisplayFunc  (&GlutManager::GlutDisplayCB);
	glutReshapeFunc  (&GlutManager::GlutReshapeCB);
	glutMotionFunc   (&GlutManager::GlutMotionCB);
	glutMouseFunc    (&GlutManager::GlutMouseCB);
	glutTimerFunc    (100,&GlutManager::GlutTimerCB,0);
	glutIdleFunc     (&GlutManager::GlutIdleCB);
	glutKeyboardFunc (&GlutManager::GlutKeyboardCB);

	if (id < 1)
		return id;

	int old_size = mWindows.size();
	if (id+1 > old_size) {
		mWindows.resize(id+1);
		for (int i = old_size; i < id; i++)
			mWindows[i]=0;
	}

	mWindows[id]=win;

//	std::cout << " Window " << id << " opened" << std::endl;
	
	return id;
}

void GlutManager::Close(int id)
{
	if (id < 1 || id > int(mWindows.size()))
		return;
	if (!mWindows[id])
		return;
	glutDestroyWindow(id);
	mWindows[id] = 0;
}

void GlutManager::GlutDisplayCB()
{
	int i = glutGetWindow();
	mWindows[i]->DisplayCb();
}

void GlutManager::GlutReshapeCB(int w, int h)
{
	int i = glutGetWindow();
	mWindows[i]->ReshapeCb(w,h);
}

void GlutManager::GlutTimerCB(int v)
{
	for (unsigned i=0; i<mWindows.size(); i++)
		if (mWindows[i])
			mWindows[i]->TimerCb(v);
}

void GlutManager::GlutIdleCB(void)
{
	for (unsigned i=0; i<mWindows.size(); i++)
		if (mWindows[i]) 								// BUG: Race condition.
			mWindows[i]->IdleCb();
}

void GlutManager::GlutMouseCB(int button, int state, int x, int y)
{
	int i = glutGetWindow();
	mWindows[i]->MouseCb(button,state,x,y);
}

void GlutManager::GlutMotionCB(int x, int y)
{
	int i = glutGetWindow();
	mWindows[i]->MotionCb(x,y);
}

void GlutManager::GlutKeyboardCB(unsigned char key, int x, int y)
{
	int i = glutGetWindow();
	mWindows[i]->KeyboardCb(key,x,y);
}


bool GlutManager::mRunning = false;

pthread_t GlutManager::mThread;


void GlutManager::Start(void)
{
	if (mRunning)
		return;
	mRunning = true;
	pthread_create(&mThread,0,&GlutManager::MainLoop,0);
}


void* GlutManager::MainLoop(void*)
{
	glutMainLoop();
	return 0;
}

