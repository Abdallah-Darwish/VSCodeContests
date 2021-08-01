#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;
using ll	= long long;
const int N = 200'010;
int main()
{
	ios_base::sync_with_stdio(false), cout.tie(nullptr), cin.tie(nullptr);
#ifdef _DEBUG_
	ifstream in("in.txt");
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
	cout << boolalpha;
#endif
	// your code goes here
#ifdef _DEBUG_
	cout << endl;
	cin.rdbuf(cinbuf);
#endif
}