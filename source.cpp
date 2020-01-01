#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
using ll	= signed long long int;
const int N = 1e6;
int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(nullptr);
	cin.tie(nullptr);
#ifdef _DEBUG_
	ifstream in("in.txt");
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
	cout << boolalpha;
#endif

#ifdef _DEBUG_
	cout << endl;
	cin.rdbuf(cinbuf);
#endif
}