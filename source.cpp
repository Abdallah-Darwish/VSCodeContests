#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;
using ll	= long long;
const int N = 200'010;
const int A = 'z' - 'a' + 2;
string a[N];
int msk[N];
int frq[A], an, sn;
void print_msk()
{
	cout << "msk: ";
	for (int i = 0; i < an; i++)
	{
		cout << msk[i] << ' ';
	}
	cout << endl;
}
pair<string, int> solve(int idx)
{
	print_msk();
	if (idx >= sn)
	{
		return {"", count_if(msk, msk + an,
							 [idx](int x) -> bool { return x == idx; })};
	}
	fill_n(frq, A, 0);

	char c;
	for (int ai = 0; ai < an; ai++)
	{
		if (msk[ai] < idx) { continue; }
		c = a[ai][idx];
		if (c == '*') { msk[ai]++; }
		else { frq[c - 'a']++; }
	}
	map<int, vector<int>> fm;
	for (int i = 0; i < A; i++)
	{
		if (frq[i] != 0) { fm[frq[i]].push_back(i); }
	}

	pair<string, int> result{"", -1};
	if (fm.empty())
	{
		auto nxt = solve(idx + 1);
		nxt.first.insert(nxt.first.begin(), 'a');
		result = nxt;
	}
	else
	{
		char mc;
		for (auto p = fm.crbegin(); p != fm.crend(); p++)
		{
			auto &v = p->second;
			for (int i : v)
			{
				mc = i + 'a';
				for (int ai = 0; ai < an; ai++)
				{
					if (msk[ai] >= idx && a[ai][idx] == mc) { msk[ai]++; }
				}
				auto nxt = solve(idx + 1);
				if (nxt.second > result.second)
				{
					result = nxt;
					result.first.insert(result.first.begin(), mc);
				}
				for (int ai = 0; ai < an; ai++)
				{
					if (msk[ai] >= idx && a[ai][idx] == mc) { msk[ai]--; }
				}
			}
		}
	}
	for (int ai = 0; ai < an; ai++)
	{
		if (msk[ai] > idx) { msk[ai]--; }
	}
	return result;
}
void read_input()
{
	cin >> an;
	for (int i = 0; i < an; i++)
	{
		cin >> a[i];
	}
	sn = a[0].length();
}
int main()
{
	ios_base::sync_with_stdio(false), cout.tie(nullptr), cin.tie(nullptr);
#ifdef _DEBUG_
	ifstream in("in.txt");
	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
	cout << boolalpha;
#endif
	read_input();
	auto res = solve(0);
	cout << res.first << '\t' << res.second;
#ifdef _DEBUG_
	cout << endl;
	cin.rdbuf(cinbuf);
#endif
}