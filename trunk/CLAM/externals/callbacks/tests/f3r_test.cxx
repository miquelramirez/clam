#include "CBL.hxx"
using CBL::Functor3R;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor3R<int,int,int,int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		int value = 0;
		value = notify( 3, 2, 3 );
		
		cout << " The Button Received " << value << endl;
	}

private:

	Functor3R<int,int,int,int> notify;

};

class CDPlayer
{
public:
	int play( int value, int value2, int value3 )
	{
		cout << "Playing " << value << " " << value2 << " " << value3 << endl;
		return value + value2 + value3;
	}
	
	int stop( int value, int value2, int value3 ) const
	{
		cout << "Stopping " << value << " " << value2 << " " << value3 << endl;
		
		return value + value2 + value3;
	}
};

int wow( int value, int value2, int value3 )
{
	cout << " wow! " << value << " " << value2 << " " << value3 << endl;
	
	return value + value2 + value3;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor3R<int,int,int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor3R<int,int,int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor3R( int, int, int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor3R( int, int, int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor3R<int,int,int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor3R( int, int, int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

