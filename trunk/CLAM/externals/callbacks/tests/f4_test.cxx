#include "CBL.hxx"
using CBL::Functor4;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor4<int,int,int,int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		notify( 3, 4, 5, 6 );
	}

private:

	Functor4<int,int,int,int> notify;

};

class CDPlayer
{
public:
	void play( int value, int value2, int value3, int value4)
	{
		cout << "Playing " << value << " " << value2 << " " << value3 << " " << value4 << endl;
	}
	
	void stop( int value, int value2, int value3, int value4) const
	{
		cout << "Stopping " << value << " " << value2 << " " << value3 << " " << value4 << endl;
	}
};

void wow( int value, int value2, int value3, int value4 )
{
	cout << " wow! " << value << " " << value2 << " " << value3 << " " << value4 << endl;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor4<int,int,int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor4<int,int,int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor4( int, int, int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor4( int, int, int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor4<int,int,int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor4( int, int, int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

