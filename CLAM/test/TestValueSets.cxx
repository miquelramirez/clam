#include "ValueSet.hxx"
#include <iostream>
#include <algorithm>

template < typename ValueSetIterator >
void simpleDataMapper( ValueSetIterator first, ValueSetIterator last )
{
	while ( first!=last)
		{
			std::cout << "{ " << first.var_value() << "," << *first << "}" << std::endl;
			first++;
		}
}

int main( int argc, char** argv )
{
	
	unsigned char buff[16];

	int i = 16;

	unsigned char* ptr_buff = &(buff[0]);
	unsigned char remainder = 0;

	do
		{
			remainder = i%3;
			switch (remainder)
				{
				case 0:
					*ptr_buff++ = 127;
					break;
				case 1:
					*ptr_buff++ = 0;
					break;
				case 2:
					*ptr_buff++= 255;
					break;
				}
			
		} 
	while ( --i );

	CLAMGUI::ValueSet testValueSet( 0, 0.01, 16, "", "", "seconds", "s" );

	std::copy( buff, buff+17, testValueSet.begin() ); 

	simpleDataMapper( testValueSet.begin(), testValueSet.end() );

	return 0;
}
