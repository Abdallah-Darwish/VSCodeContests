#include <iostream>
#ifdef _DEBUG_
#include "util.hpp"
using namespace util;
using namespace util::leetcode;
#else
static void enter()
{
	ios_base::sync_with_stdio(false), cout.tie(nullptr), cin.tie(nullptr);
}
static void exit() {}
#endif
using namespace std;
const int N = 505;
void solve()
{
	// your code goes here
}
int main() { enter(), solve(), exit(); }