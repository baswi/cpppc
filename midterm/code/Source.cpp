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
		using size_type = std::size_t;

		/***************************************************************/
		/******************     CHUNKS__ITERATOR     *******************/
		/***************************************************************/
		class iterator {
			using self_t = iterator;
			using chunk_t = chunks;
			using pos_t = int;
			using index_t = int;
		public:
			// not sure if T is the correct type since then every chunk_iter dereferences to the first elem of its chunk?!?
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T * ;
			using reference = T & ;
			using iterator_category = std::random_access_iterator_tag;

		public:
			iterator() = delete;
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

			constexpr bool operator==(const self_t & rhs) const noexcept {
				return _pos == rhs._pos;
			}

			constexpr bool operator!=(const self_t & rhs) const noexcept {
				return !(*this == rhs);
			}

			auto operator-(const self_t & rhs) {
				return (_pos - rhs._pos);
			}

			auto operator[](index_t  i) {
				return *(elem_iterator(_chunk, 0, (_pos + i)));
			}

			// ...:begin' cannot result in a constant expression ??? WHY ...?!?!?!?
			auto begin() noexcept {
				return elem_iterator(_chunk, 0, _pos);
			}

			// part of the hack to satisfy the example... see chunks operator[] and the example for more details
			constexpr decltype(auto) data() const {
				return &(_chunk.elem(_pos * (B / sizeof(T))));
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
			using chunk_t = chunks;
			using pos_t = int;
			using index_t = int;

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T * ;
			using reference = T & ;
			using iterator_category = std::random_access_iterator_tag;

		public:
			elem_iterator() = delete;
			elem_iterator(chunk_t & chunk, const pos_t & pos, const pos_t & chunkPos)
				: _chunk(chunk)
				, _pos(pos)
				, _chunkPos(chunkPos)
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

			constexpr bool operator==(const self_t & rhs) const noexcept {
				return ((_pos == rhs._pos) && (_chunkPos == rhs._chunkPos));
			}

			constexpr bool operator!=(const self_t & rhs) const noexcept {
				return !(*this == rhs);
			}

			auto operator-(const self_t & rhs) {
				return (_pos - rhs._pos);
			}

			constexpr decltype(auto) operator*() const {
				/* too dumb to figure this out atm...
				// for this to work the chunk operator[] need to return the correct part of the container -> a view would be great...wouldn't it?!?
				return (_chunk[_chunkPos])[_pos];
				*/
				return _chunk.elem((_chunkPos * (B / sizeof(T))) + _pos);
			}


		private:
			void increment(const index_t & offset) {
				_pos += offset;
			}

		private:
			chunk_t & _chunk;
			pos_t     _pos;
			pos_t     _chunkPos;
		}; // elem_iterator


	public:

		explicit chunks(Container & container)
			: _container(container)
		{ }

		constexpr auto begin() noexcept {
			return iterator(*this, 0);
		}

		constexpr auto end() noexcept {
			int numChunks = ((std::size(_container) * sizeof(T))
				/
				B);
			return iterator(*this, (++numChunks));
		}

		// some sort of view would be the best...but I'm out of time & not that sure how to implement it correctly....
		// so this is my hack to satisfy the example...
		constexpr auto operator[](int offset) const {
			return iterator(*this, offset);
		}

		// not nice (meaning not what I want) but can't get my head around how to quickly build a view for the elem_Iter to access the Elems...
		constexpr auto elem(int pos) const {
			return _container[pos];
		}

	private:
		// hmm is const right here?!?
		const Container & _container;

	}; //class chunks


	
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
		std::shuffle(std::begin(indices), std::end(indices));
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

//******************************************************************************
//********************************* CHUNKS *************************************
//******************************************************************************

	std::vector<uint16_t> v_us;

	//                                    ,-- wrapped container
	//                                    |
	cpppc::chunks<128, uint16_t, std::vector<uint16_t>> v_chunks(v_us);
	//             |      |
	//             |      '-- element type
	//             |
	//             '-- maximum size of a
	//                 single chunk in bytes

	// Iterate chunks:
	auto first_chunk = v_chunks.begin();
	auto num_chunks = std::distance(v_chunks.begin(), v_chunks.end());

	std::cout << num_chunks << '\n';
	// --> 128/(16/8) = 64

	// Iterators on elements in a chunk:
	uint16_t first_chunk_elem = *first_chunk.begin();
	uint16_t third_chunk_elem = first_chunk[2];

	/* hmmm return is void....why.... again no more time... :(
	// for this to work properly (as I think it is intended) the operator[] on chunks must return a view on the _container for the specific part
	// Pointer to data in second chunk:
	uint16_t * chunk_1_data = v_chunks[1].data();
	// Pointer to data in third chunk (= end pointer of data in second chunk):
	uint16_t * chunk_2_data = v_chunks[2].data();

	*/





/**********************************************************************************/
/********************* for keeping the [VS | MS] console open *********************/
/**********************************************************************************/
	std::cout << "\n\n\n\nTESTS DONE: press <enter> to close console in [VS | MS]";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return 0;
}