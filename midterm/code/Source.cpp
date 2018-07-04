#include <algorithm>
#include <cmath>
#include <forward_list>
#include <iostream>
#include <list>
#include <thread>
#include <vector>


namespace cpppc {
	template <typename Iterator, typename Value = typename Iterator::value_type>
	Value next_value_or_default(Iterator it, Iterator end, Value def) {
		if (it == end || ++it == end) { return def; }
		return *it;
	} // function: next_value_or_default

/*
Als aller erstes fällt auf, dass beim return der bracket operator benutzt wird. Der wird konzeptuell erst für RndAccIter spezifiziert (hier das richtige Wort? oder eher definiert?).
Damit gehen scho mal nur noch die Container, welche einen solchen liefern.
Die allgemein gültigste Lösung, da bereits für InputIter spezifiziert (streng genommen eigentlich sogar schon durch das Grundkonzept eines Iterators [derefenceable + incrementable]):
return *(++it);

Hat man nun das gelöst fällt einem als nächstes auf (wenn man von unten nach oben durchgeht), dass 'it + n' zu weiteren Einschränkungen führt.
Genau genommen wieder dazu, dass man nur RndAccIter verwenden kann.
ohne viel umzuschreiben wäre die erste Lösung die mir einfällt:
if (it == end || ++it == end) { return def; }
return *it;

Damit kann nun jeder Container der mindestens nen InputIterator liefert die Funktion benutzen.
*/


/***********************************************************************/
/***********************************************************************/
/**********************          CHUNKS          ***********************/
/***********************************************************************/
/***********************************************************************/

	template <std::size_t B, class T, class Container>
	class chunks
	{
	public:
		using value_type = T;
		using size_type  = std::size_t;

/***************************************************************/
/******************     CHUNKS__ITERATOR     *******************/
/***************************************************************/
		class iterator {
			using self_t    = iterator;
			using chunk_t   = chunks;
			using pos_t     = int;
			using index_t   = int;

// not sure if T is the correct type since then every chunk_iter dereferences to the first elem of its chunk?!?
			using difference_type    =  std::ptrdiff_t;
			using value_type         =  T;
			using pointer            =  T*;
			using reference          =  T&;
			using iterator_category  =  std::random_access_iterator_tag;

		public:
			iterator(chunk_t & chunk, const pos_t & pos)
			: _chunk(chunk)
			, _pos(pos)
			{ }


			// Pre-increment
			self_t & operator++() {
				increment(1);
				return *this;
			}

			// Post-increment
			self_t operator++(int) {
				self_t old(*this);
				increment(1);
				return old;
			}

			constexpr decltype(auto) operator[](index_t  i){
				return _chunk._container;
			}

			constexpr auto begin() noexcept {
			// TODO(new): 
			//       correct elem_iterator
				return elem_iterator();
			}

		private:
			void increment(const index_t & offset) {
				_pos += offset;
			}

		private:
			chunk_t & _chunk;
			pos_t     _pos;
		}; // iterator


/***************************************************************/
/******************      ELEM__ITERATOR      *******************/
/***************************************************************/

		class elem_iterator {
			using self_t = elem_iterator;
			using pos_t  = int;

			using difference_type	= std::ptrdiff_t;
			using value_type		= T;
			using pointer			= T * ;
			using reference			= T & ;
			using iterator_category = std::random_access_iterator_tag;

		private:
			pos_t _pos;
		}; // elem_iterator


	public:

		explicit chunks(Container && c)
		: _container(std::forward<Container>(c))
		{ }

		constexpr auto begin() noexcept {
			return iterator(*this, 0);
		}

		constexpr auto end() noexcept {
			// TODO(new): 
			//       correct iterator
			return iterator();
		}

	private:
		// hmm is const right here?!?
		const Container & _container;


	}; // class chunks


	
/***********************************************************************/
/***********************************************************************/
/**********************          log10           ***********************/
/***********************************************************************/
/***********************************************************************/

// Clang does not like this....
// Clang : declaring a parameter with a default argument is disallowed	code
// Clang : all parameters should be named in a function	code

	template <class T>
	void log10(T x, typename std::enable_if<
	                                         std::is_integral<T>::value
		                                    ,T
	                                        >::type = 0 ) {
		std::cout << std::log10(x);
	}

	template <class T>
	void log10(T x, typename std::enable_if<
		                                    std::is_floating_point<T>::value
		                                   ,T
	                                       >::type = 0 ) {
		std::cout << std::log10(std::sqrt(x));
	}

/*
 da so dann nen Rückgabewert benötigt wird, ist das dann nimma void log10.... also nicht das was gefordert


	template <class T>
	typename std::enable_if<std::is_integral<T>::value
		                   , T
	                       >::type log10(T x) {
		std::cout << std::log10(x);
	}

	template <class T>
	typename std::enable_if<std::is_floating_point<T>::value
		                   , T
	                       >::type log10(T x) {
		std::cout << std::log10(std::sqrt(x));
	}
*/


/***********************************************************************/
/***********************************************************************/
/********************         print_walk           *********************/
/***********************************************************************/
/***********************************************************************/
	template<class Iter>
	void print_walk(Iter begin, Iter end, std::random_access_iterator_tag /*unused*/) {
		// https://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=random&button=
		// -> random_shuffle sounds great. maybe the best way would be to have a view and than just shuffle the view?
		// faster to implement, at least for me at the moment is the following (just cpy into a container and shuffle this)
		// other idea would be to just rand the index and keep track of alrdy used indices
		// oh I could create a vector with the indices and shuffle this
		std::vector<std::size_t> indices;
		auto use = begin;
		for (std::size_t i = 0; use != end; ++i, ++use) {
			indices.push_back(i);
		}
		std::random_shuffle(std::begin(indices), std::end(indices));
		for (auto i : indices) {
			use = begin;
			std::cout << use[i] << " | ";
		}
		std::cout << '\n';
	}

	template<class Iter>
	void print_walk(Iter begin, Iter end, std::input_iterator_tag /*unused*/) {
		for (; begin != end; ++begin) {
			std::cout << *begin << " | ";
		}
		std::cout << '\n';
	}

	template<class Iter>
	void print_walk(Iter begin, Iter end, std::bidirectional_iterator_tag /*unused*/) {
		/*
		std::copy(
			std::make_reverse_iterator(end),
			std::make_reverse_iterator(begin),
// hier fällt mir auf die schnelle keine Lösung ein, wie ich an den value_type komm...irgendwie aus Iterator Member Types die jeder std Iterator hat ziehen
// zudem kommt nen error: error C2760: syntax error: unexpected token 'int', expected 'expression' ?!?
			std::ostream_iterator<int>(std::cout, "| "));
		std::cout << '\n';
		*/

		for (--end; begin != end; --end) {
			std::cout << *end << " | ";
		}
		std::cout << *end << " |\n";
	}

	template <class Iter>
	void print_walk(Iter begin, Iter end)
	{
		print_walk(begin, end, typename std::iterator_traits<Iter>::iterator_category());
	}

} // namespace cpppc



int main() {
// testing next_value_or_default
	std::cout << "*********** TEST ***********\n";
	std::cout << "** next_value_or_default **\n";
	std::vector<int> vi1 { 11, 22, 33, 44, 55, 66 };
	auto vzwei = cpppc::next_value_or_default(std::begin(vi1), std::end(vi1), 0);
	std::cout << "v_zwei ?= " << vzwei << '\n';

	std::forward_list<int> fli1{ 11, 22, 33, 44, 55, 66 };
	auto lzwei = cpppc::next_value_or_default(std::begin(fli1), std::end(fli1), 0);
	std::cout << "l_zwei ?= " << lzwei << '\n';
	
// testing log10 & print_walk
	std::cout << '\n';
	std::cout << "*********** TEST ***********\n";
	std::cout << "*********** log10 **********\n";
	std::vector<double>       vf1 { 99, 88, 77, 66, 55, 44, 33, 22, 11 };

	std::cout << "\nlog10(int): \n| ";
	for (auto begin = std::begin(vi1); begin != std::end(vi1); ++begin) {
		cpppc::log10(*begin);
		std::cout << " | ";
	}
	std::cout << '\n';
	std::cout << "\nlog10(double): \n| ";
	for (auto begin = std::begin(vf1); begin != std::end(vf1); ++begin) {
		cpppc::log10(*begin);
		std::cout << " | ";
	}
	std::cout << '\n';
	std::cout << '\n';
	std::cout << "*********** TEST ***********\n";
	std::cout << "******** print_walk ********\n";
//	std::forward_list<double> flf1{ 99, 88, 77, 66, 55, 44, 33, 22, 11 };
	std::list<int>            li1 { 11, 22, 33, 44, 55, 66 };
//	std::list<double>         ld1 { 99, 88, 77, 66, 55, 44, 33, 22, 11 };

	std::cout << "print_walk rndIter:\n";
	cpppc::print_walk(std::begin(vi1), std::end(vi1));
	std::cout << "\n\nprint_walk forwardIter:\n";
	cpppc::print_walk(std::begin(fli1), std::end(fli1));
	std::cout << "\n\nprint_walk biDiIter:\n";
	cpppc::print_walk(std::begin(li1), std::end(li1));



/**********************************************************************************/
/********************* for keeping the [VS | MS] console open *********************/
/**********************************************************************************/
	std::cout << "\n\n\n\nTESTS DONE: press <enter> to close console in [VS | MS]";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return 0;
}