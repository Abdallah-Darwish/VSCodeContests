#pragma once
#include <limits>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <bitset>
#include <iostream>
#include <string>
#include <variant>
using namespace std;
using ll  = long long;
using vi  = vector<int>;
using vvi = vector<vi>;
namespace util
{
	const int PAD = 2;
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
			requires same_as<decltype(p.first), typename P::first_type>;
			requires same_as<decltype(p.second), typename P::second_type>;
		};
		template <typename B>
		concept is_bitset = requires(B &b) {
			size(b);
			b.set(0);
			b.test(0);
			requires same_as<decltype(b.to_string()), string>;
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
		concept is_nullable_variant =
			(is_null_pointer_v<variant_alternative_t<0, T>> ||
			 is_null_pointer_v<variant_alternative_t<1, T>>) &&
			variant_size_v<T> == 2;

		template <is_nullable_variant T> constexpr size_t not_null_index()
		{
			if constexpr (is_null_pointer_v<variant_alternative_t<0, T>>)
			{
				return 1;
			}
			else { return 0; }
		}
		template <is_nullable_variant T>
		using not_null_alternative_t =
			variant_alternative_t<not_null_index<T>(), T>;
		template <typename T>
		concept is_non_str_iterable = is_iterable<T> && !is_same_v<T, string> &&
									  !is_same_v<remove_all_extents_t<T>, char>;
		template <typename T>
		concept is_binary_node = requires(T &t) {
			typename T::value_type;
			T();
			T(declval<typename T::value_type>());
			T(declval<typename T::value_type>(), declval<T *>(),
			  declval<T *>());
			requires same_as<decltype(t.left), T *>;
			requires same_as<decltype(t.right), T *>;
		};
		template <typename P>
		concept is_binary_node_p =
			is_pointer_v<P> &&
			is_binary_node<remove_const_t<remove_pointer_t<P>>>;
		template <typename T>
		concept is_printable =
			is_non_str_iterable<T> || is_pair<T> || is_queue<T> ||
			is_stack<T> || is_nullable_variant<T>;
		template <typename T>
		concept is_readable =
			is_non_str_iterable<T> || is_pair<T> || is_queue<T> ||
			is_stack<T> || is_nullable_variant<T> || is_binary_node_p<T>;

	} // namespace concepts

	template <typename T> struct binary_node
	{
		using value_type = T;
		T val;
		binary_node *left, *right;
		binary_node(T v, binary_node *l, binary_node *r)
			: val(v), left(l), right(r)
		{
		}
		binary_node(T v) : binary_node(v, nullptr, nullptr) {}
		binary_node() : binary_node({}, nullptr, nullptr) {}
	};

	void enter();
	void exit();
	template <typename T>
	ostream &print(ostream &os, const T &v, size_t lpad = 0)
	{
		if (lpad) { cout << string(lpad, ' '); }
		if constexpr (concepts::is_nullable_variant<T>)
		{
			if (holds_alternative<nullptr_t>(v)) { cout << "null"; }
			else { print(os, get<concepts::not_null_index<T>()>(v)); }
		}
		else if constexpr (concepts::is_queue<T> || concepts::is_stack<T>)
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
			os << "{";
			size_t inner_lpad = 0;
			if constexpr (concepts::is_non_str_iterable<decltype(*begin(v))>)
			{
				os << endl;
				inner_lpad = lpad + PAD;
			}
			else { cout << ' '; }
			auto sz = size(v);
			for (const auto &i : v)
			{
				sz--;
				print(os, i, inner_lpad);
				if (sz) { os << ","; }
				if constexpr (concepts::is_non_str_iterable<decltype(i)>)
				{
					os << endl;
				}
				else { os << ' '; }
			}
			if constexpr (concepts::is_non_str_iterable<decltype(*begin(v))>)
			{
				if (lpad > 0) { os << string(lpad, ' '); }
			}
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
#define dbg(_V) cout << #_V << ": " << _V << endl;

	namespace leetcode
	{
		template <concepts::is_binary_node_p T>
		using node_vector = vector<
			variant<nullptr_t, typename remove_pointer_t<T>::value_type>>;
		template <concepts::is_binary_node_p T>
		T _read_node(const node_vector<T> &vec, int i)
		{
			if (i >= vec.size() || holds_alternative<nullptr_t>(vec[i]))
			{
				return static_cast<T>(nullptr);
			}

			return new (remove_pointer_t<T>)(get<1>(vec[i]),
											 _read_node<T>(vec, 2 * i + 1),
											 _read_node<T>(vec, 2 * i + 2));
		}
		template <typename T> istream &read(istream &in, T &item)
		{
			if constexpr (concepts::is_nullable_variant<T>)
			{
				if (in.peek() == 'n')
				{
					in.ignore(4); // null
					item = nullptr;
				}
				else
				{
					typename concepts::not_null_alternative_t<T> i{};
					read(in, i);
					item = i;
				}
			}
			else if constexpr (concepts::is_binary_node_p<T>)
			{
				node_vector<T> vec{};
				read(in, vec);
				item = _read_node<T>(vec, 0);
			}
			else if constexpr (concepts::is_queue<T> || concepts::is_stack<T>)
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
				while (in && in.peek() == '\n')
				{
					in.ignore(1);
				}
				char quote = '\n';
				int nxt	   = in.peek();
				if (nxt == ' ') { quote = ' '; }
				else if (nxt == '"') { quote = '"'; }
				if (nxt == quote) { cin.ignore(1); }

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
