#include "CBL.hxx"
using CBL::Functor4R;
using CBL::makeFunctor;
#include <iostream>
using std::cout;
using std::endl;

class Button
{
public:

	Button( const Functor4R<int,int,int,int,int> &uponclickdothis )
		: notify( uponclickdothis )
	{
	}

	void click()
	{
		int value = 0;
		value = notify( 3, 2, 3, 2 );
		
		cout << " The Button Received " << value << endl;
	}

private:

	Functor4R<int,int,int,int,int> notify;

};

class CDPlayer
{
public:
	int play( int value, int value2, int value3, int value4 )
	{
		cout << "Playing " << value << " " << value2 << " " << value3 << " " << value4 << endl;
		return value + value2 + value3 + value4;
	}
	
	int stop( int value, int value2, int value3, int value4 ) const
	{
		cout << "Stopping " << value << " " << value2 << " " << value3 << " " << value4 << endl;
		
		return value + value2 + value3 + value4;
	}
};

int wow( int value, int value2, int value3, int value4 )
{
	cout << " wow! " << value << " " << value2 << " " << value3 << " " << value4 << endl;
	
	return value + value2 + value3 + value4;
}

int main()
{

	typedef int* my_ptr;

	my_ptr* p = new my_ptr;

	CDPlayer cd;
	
	//Button playButton( makeFunctor((Functor4R<int,int,int,int,int>*)0, cd, &CDPlayer::play ) );
	//Button stopButton( makeFunctor((Functor4R<int,int,int,int,int>*)0, cd, &CDPlayer::stop ) );
	Button playButton( makeMemberFunctor4R( int, int, int, int, int, cd, CDPlayer, play ) );
	Button stopButton( makeMemberFunctor4R( int, int, int, int, int, cd, CDPlayer, stop ) );

	//Button wowButton( makeFunctor((Functor4R<int,int,int,int,int>*)0, &wow ) );
	Button wowButton( makeCFunctionFunctor4R( int, int, int, int, int, wow ) );

	playButton.click();
	stopButton.click();
	wowButton.click();
	

	return 0;
}

