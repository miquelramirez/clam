#include "CBL.hxx"
using CBL::Functor0R;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor0R<int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		int value =	notify( );
		
		cout << "The Button received " << value <<  endl;
	}

private:

	Functor0R<int> notify;

};

class CDPlayer
{
public:
	int play( )
	{
		cout << "Playing "  << endl;
		
		return 33;
	}
	
	int stop( ) const
	{
		cout << "Stopping "  << endl;
		
		return 66;
	}
};

int wow( )
{
	cout << " wow! "  << endl;
	
	return 99;
}

int main()
{

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor0R<int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor0R<int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor0R( int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor0R( int, cd, CDPlayer, stop ) );


	//Button wowButton( makeFunctor((Functor0R<int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor0R( int, wow ) );


	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

