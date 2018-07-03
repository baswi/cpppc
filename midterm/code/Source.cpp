#include <forward_list>
#include <iostream>
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


/***************************************************************/
/******************          CHUNKS          *******************/
/***************************************************************/

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

			constexpr decltype(auto) operator[](index_t i){
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
} // namespace cpppc



int main() {
	std::vector<int> v1 { 1, 2, 3 };
	auto vzwei = cpppc::next_value_or_default(std::begin(v1), std::end(v1), 0);
	std::cout << "vzwei ?= " << vzwei << '\n';

	std::forward_list<int> l1{ 1, 2, 3 };
	auto lzwei = cpppc::next_value_or_default(std::begin(l1), std::end(l1), 0);
	std::cout << "lzwei ?= " << lzwei << '\n';
	


	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return 0;
}