#include "CBL.hxx"
using CBL::Functor2;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor2<int,int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		notify( 3, 10 );
	}

private:

	Functor2<int,int> notify;

};

class CDPlayer
{
public:
	void play( int value1, int value2 )
	{
		cout << "Playing " << value1 << " " << value2 << endl;
	}
	
	void stop( int value, int value2 ) const
	{
		cout << "Stopping " << value << " " << value2 << endl;
	}
};

void wow( int value, int value2 )
{
	cout << value << " wow! " << value2 << endl;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor2<int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor2<int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor2( int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor2( int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor2<int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor2( int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

