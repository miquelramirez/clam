#include "CBL.hxx"
using CBL::Functor1;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor1<int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		notify( 3 );
	}

private:

	Functor1<int> notify;

};

class CDPlayer
{
public:
	void play( int value)
	{
		cout << "Playing " << value << endl;
	}
	
	void stop( int value) const
	{
		cout << "Stopping " << value << endl;
	}
};

void wow( int value )
{
	cout << value << " wow! " << value << endl;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor1<int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor1<int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor1( int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor1( int, cd, CDPlayer, stop ) );


	//Button wowButton( makeFunctor((Functor1<int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor1( int, wow ) );
	
	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

