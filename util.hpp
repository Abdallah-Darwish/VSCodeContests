#pragma once
#include <limits>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <bitset>
#include <iostream>
#include <string>
using namespace std;
using ll  = long long;
using vi  = vector<int>;
using vvi = vector<vi>;
namespace util
{

	namespace concepts
	{
		template <typename T>
		concept is_iterable = requires(T &t) {
			begin(t) != end(t);				   // begin/end and operator !=
			++declval<decltype(begin(t)) &>(); // operator ++
			*begin(t);						   // operator*
			size(t);						   // size
		};
		template <typename T>
		concept has_push_back = requires(T &t) { t.push_back(*begin(t)); };
		template <typename T>
		concept has_insert = requires(T &t) { t.insert(*begin(t)); };
		template <typename T>
		concept is_insertable = has_insert<T> || has_push_back<T>;
		template <typename P>
		concept is_pair = requires(P &p) {
			typename P::first_type;
			typename P::second_type;
			p.first;
			p.second;
			requires std::same_as<decltype(p.first), typename P::first_type>;
			requires std::same_as<decltype(p.second), typename P::second_type>;
		};
		template <typename B>
		concept is_bitset = requires(B &b) {
			size(b);
			b.set(0);
			b.test(0);
			requires std::same_as<decltype(b.to_string()), string>;
		};
		template <typename Q>
		concept is_queue = requires(Q &q) {
			typename Q::value_type;
			q.push(declval<decltype(q.front())>());
			q.pop();
			q.front();
			q.empty();
			q.size();
		};
		template <typename S>
		concept is_stack = requires(S &s) {
			typename S::value_type;
			s.push(declval<decltype(s.top())>());
			s.pop();
			s.top();
			s.empty();
			s.size();
		};
		template <typename T>
		concept is_non_str_iterable = is_iterable<T> && !is_same_v<T, string> &&
									  !is_same_v<remove_all_extents_t<T>, char>;
		template <typename T>
		concept is_printable =
			is_non_str_iterable<T> || is_pair<T> || is_queue<T> || is_stack<T>;
		template <typename T>
		concept is_readable =
			is_non_str_iterable<T> || is_pair<T> || is_queue<T> || is_stack<T>;
	} // namespace concepts

	void enter();
	void exit();
	template <typename T> ostream &print(ostream &os, const T &v, int lpad = 0)
	{
		if constexpr (concepts::is_queue<T> || concepts::is_stack<T>)
		{
			T q = v;
			vector<typename T::value_type> vec{};
			vec.reserve(q.size());
			while (!q.empty())
			{
				if constexpr (concepts::is_queue<T>)
				{
					vec.push_back(q.front());
				}
				else { vec.push_back(q.top()); }
				q.pop();
			}
			print(os, vec);
		}
		else if constexpr (concepts::is_non_str_iterable<T>)
		{
			os << string(lpad, ' ') << "{ ";
			if constexpr (concepts::is_non_str_iterable<decltype(*begin(v))>)
			{
				os << endl;
			}
			decltype(size(v)) sz = size(v);
			for (const auto &i : v)
			{
				sz--;
				print(os, i, lpad + 2);
				if (sz) { os << ", "; }
				if constexpr (concepts::is_non_str_iterable<decltype(i)>)
				{
					os << endl;
				}
			}
			if constexpr (concepts::is_non_str_iterable<decltype(*begin(v))>)
			{
				os << string(lpad, ' ');
			}
			else { os << ' '; }
			os << "}";
		}
		else if constexpr (concepts::is_pair<T>)
		{
			os << '[';
			print(os, v.first) << ", ";
			print(os, v.second) << ']';
		}
		else { os << v; }
		return os;
	}
	template <concepts::is_printable T>
	ostream &operator<<(ostream &os, const T &v)
	{
		return print(os, v);
	}

	namespace leetcode
	{
		template <typename T> istream &read(istream &in, T &item)
		{
			if constexpr (concepts::is_queue<T> || concepts::is_stack<T>)
			{
				vector<typename T::value_type> vec{};
				read(in, vec);
				if constexpr (concepts::is_stack<T>)
				{
					reverse(vec.begin(), vec.end());
				}
				for (const auto &i : vec)
				{
					item.push(i);
				}
			}
			else if constexpr (is_same_v<T, char>)
			{
				in.ignore(numeric_limits<streamsize>::max(), '"');
				item = in.get();
				in.ignore(numeric_limits<streamsize>::max(), '"');
			}
			else if constexpr (is_same_v<T, string>)
			{
				char quote = ' ';

				if (in.peek() == '"')
				{
					quote = '"';
					in.ignore(1);
				}
				getline(in, item, quote);
			}
			else if constexpr (concepts::is_non_str_iterable<T>)
			{
				in.ignore(numeric_limits<streamsize>::max(), '[');
				while (in && (in.peek() != ']'))
				{
					remove_const_t<remove_reference_t<decltype(*begin(item))>>
						i{};
					read(in, i);
					if (in.peek() == ',')
					{
						in.ignore(numeric_limits<streamsize>::max(), ',');
					}
					if constexpr (concepts::has_push_back<T>)
					{
						item.push_back(i);
					}
					else { item.insert(i); }
				}
				in.ignore(numeric_limits<streamsize>::max(), ']');
			}
			else if constexpr (concepts::is_pair<T>)
			{
				in.ignore(numeric_limits<streamsize>::max(), '[');
				read(in, item.first)
					.ignore(numeric_limits<streamsize>::max(), ',');
				read(in, item.second)
					.ignore(numeric_limits<streamsize>::max(), ']');
			}
			else
			{
				in >> item;
				if constexpr (is_arithmetic_v<T>) { in.clear(); }
			}
			return in;
		}

		template <typename T> struct read_wrapper
		{
			T &val;
		};
		template <typename T> read_wrapper<T> read(T &v) { return {v}; }
		template <typename T>
		istream &operator>>(istream &in, read_wrapper<T> &&w)
		{
			return read<T>(in, w.val);
		}

		template <concepts::is_readable T>
		istream &operator>>(istream &in, T &v)
		{
			return read<T>(in, v);
		}
	} // namespace leetcode
} // namespace util
