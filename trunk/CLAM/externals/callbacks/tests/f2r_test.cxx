#include "CBL.hxx"
using CBL::Functor2R;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor2R<int,int,int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		int value = 0;
		value = notify( 3, 2 );
		
		cout << " The Button Received " << value << endl;
	}

private:

	Functor2R<int,int,int> notify;

};

class CDPlayer
{
public:
	int play( int value, int value2 )
	{
		cout << "Playing " << value << " " << value2 << endl;
		return value + value2;
	}
	
	int stop( int value, int value2 ) const
	{
		cout << "Stopping " << value << " " << value2 << endl;
		
		return value + value2;
	}
};

int wow( int value, int value2 )
{
	cout << " wow! " << value << " " << value2 << endl;
	
	return value + value2;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor2R<int,int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor2R<int,int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor2R( int, int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor2R( int, int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor2R<int,int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor2R( int, int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

