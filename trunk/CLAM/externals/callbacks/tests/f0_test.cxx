#include "CBL.hxx"
using CBL::Functor0;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor0 &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		notify(  );
	}

private:

	Functor0 notify;

};

class CDPlayer
{
public:
	void play( )
	{
		cout << "Playing " <<  endl;
	}
	
	void stop( ) const
	{
		cout << "Stopping " <<  endl;
	}
};

void wow( )
{
	cout << " wow! " << endl;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor0*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor0*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor0( cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor0( cd, CDPlayer, stop ) ); 

	//Button wowButton( makeFunctor((Functor0*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor0( wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}




