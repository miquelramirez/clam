#include "CBL.hxx"
using CBL::Functor3;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor3<int, int, int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		notify( 3, 4, 5);
	}

private:

	Functor3<int,int,int> notify;

};

class CDPlayer
{
public:
	void play( int value, int value2, int value3)
	{
		cout << "Playing " << value << " " << value2 << " " << value3 << endl;
	}
	
	void stop( int value, int value2, int value3) const
	{
		cout << "Stopping " << value << " " << value2 << " " << value3 << endl;
	}
};

void wow( int value, int value2, int value3 )
{
	cout  << " wow! " << value << " " << value2 << " " << value3 << endl;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor3<int,int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor3<int,int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor3( int, int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor3( int, int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor3<int,int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor3( int, int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}


