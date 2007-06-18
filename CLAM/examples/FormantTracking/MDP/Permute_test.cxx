#include <vector>
#include <iostream>

void permute( unsigned ntrack, unsigned ncandidates, std::vector<unsigned> s )
{
	for ( unsigned p = ntrack-1; p < ncandidates-1; p++ )
		{
			std::swap( s[p] , s[p+1]);
			for( int i=0; i<s.size(); i++ )
				std::cout << s[i];
			std::cout<<std::endl;
			permute(ntrack-1,p+1,s);
		}
}


int main()
{
	unsigned ncandidates=5;
	unsigned ntrack=3;

	std::vector< unsigned > s(5);

	s[0]=1;s[1]=1;s[2]=1;s[2]=1;

	for( int i=0; i<s.size(); i++ )
		std::cout << s[i];
	std::cout<<std::endl;

	permute(ntrack, ncandidates, s);

	return 0;
}

