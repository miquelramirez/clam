#include "CBL.hxx"
using CBL::Functor1R;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor1R<int, int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		int value = 0;
		value =	notify( 3 );
		cout << "The Button receives " << value << endl;
	}

private:

	Functor1R<int,int> notify;

};

class CDPlayer
{
public:
	int play( int value )
	{
		cout << "Playing " << value << endl;
		
		return value*3;
	}
	
	virtual int stop( int value ) const
	{
		cout << "Stopping " << value <<  endl;
		
		return value*2;
	}
};

int wow( int value )
{
	cout << " wow! " << value <<  endl;
	
	return value * 5;
}

int main()
{

	CDPlayer cd;
	
//	Button playButton( makeFunctor((Functor1R<int,int>*)0, cd, &CDPlayer::play ) );
//	Button stopButton( makeFunctor((Functor1R<int,int>*)0, cd, &CDPlayer::stop ) );

	Button playButton( makeMemberFunctor1R(int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor1R(int, int, cd, CDPlayer, stop ) );

//	Button wowButton( makeFunctor((Functor1R<int,int>*)0, &wow ) );

	Button wowButton( makeCFunctionFunctor1R( int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

