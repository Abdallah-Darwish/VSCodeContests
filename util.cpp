#include "util.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
using namespace std;
using namespace util::concepts;
namespace util
{
	streambuf *_cinbuf = nullptr;
	ifstream *_in	   = nullptr;
	void enter()
	{
		ios_base::sync_with_stdio(false), cout.tie(nullptr), cin.tie(nullptr);
		assert(_in == nullptr);
		_cinbuf = cin.rdbuf();
		string in_path;
#ifdef _VS_
		in_path = R"*(C:\Users\abdal\source\repos\VSCodeContests\in.txt)*";
#else
		in_path = "in.txt";
#endif
		_in = new ifstream(in_path);
		cin.rdbuf(_in->rdbuf());
		cout << boolalpha;
	}
	void exit()
	{
		assert(_in != nullptr);
		cout << endl;
		cin.rdbuf(_cinbuf);
		delete _in;
		_in		= nullptr;
		_cinbuf = nullptr;
	}
} // namespace util
