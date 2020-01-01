#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
using ll	= signed long long int;
using vi	= vector<int>;
const int N = 1e6;
bool a[N];
int n, t, res[N], x = 1;
vi v1, v2;
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
	v1.reserve(N);
	v2.reserve(N);
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cin >> res[i];
		if (res[i] == 0) { v1.push_back(i); }
		else
		{
			a[res[i]] = true;
		}
	}
	for (int i = 1; i <= n; i++)
	{
		if (a[i] == false) { v2.push_back(i); }
	}
	if (v1[0] == v2[0]) { swap(v2[0], v2[1]); }
	res[v1[0]] = v2[0];
	for (int i = 1; i < v1.size(); i++, x = i)
	{
		if (v1[i] == v2[i]) { swap(res[v1[0]], v2[i]); }
		res[v1[i]] = v2[i];
	}

	for (int i = 1; i <= n; i++)
	{
		cout << res[i] << ' ';
	}
#ifdef _DEBUG_
	cout << endl;
	cin.rdbuf(cinbuf);
#endif
}