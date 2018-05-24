#ifndef CPPPC__S03__SPARSE_ARRAY_H__INCLUDED
#define CPPPC__S03__SPARSE_ARRAY_H__INCLUDED

// Might come in handy:
#include <unordered_map>


namespace cpppc {

namespace detail {

template <bool flag, class IsTrue, class IsFalse>
	struct choose;

template <class IsTrue, class IsFalse>
	struct choose<true, IsTrue, IsFalse>{
		typedef IsTrue type;
	};

template <class IsTrue, class IsFalse>
	struct choose<false, IsTrue, IsFalse>{
		typedef IsFalse type;
	};

template <class SparseArrayT, bool isconst = false>
class sparse_array_iterator
{  
  using self_t	   =    sparse_array_iterator;
  using index_t    =	int;
  using proxy	   =	typename SparseArrayT::proxy;

public:
  using iterator_category	= std::random_access_iterator_tag;
  using value_type		= typename SparseArrayT::value_type;
  using difference_type		= std::ptrdiff_t;
  using reference		= typename choose<isconst, const SparseArrayT&, SparseArrayT&>::type;
  using pointer			= typename choose<isconst, const SparseArrayT*, SparseArrayT*>::type;
 
  	
public:
// constructor
  sparse_array_iterator(pointer sparse_array, index_t pos)
  : _sparse_array(sparse_array)
  , _pos(pos)
  {  }

  sparse_array_iterator(const sparse_array_iterator<SparseArrayT, false>& i)
  : _sparse_array(i._sparse_array)
  , _pos(i._pos)
  {  }


// operators

  proxy operator*(){
    //return proxy(*_sparse_array, _pos);
    return (*_sparse_array)[_pos];
  }

// der verzweifelte Versuch nen overloading für operator* zu erreichen, das dann vom const_iterator aufgerufen wird...  
  const value_type operator*(choose<true, const SparseArrayT*, SparseArrayT*>) const{
    return _sparse_array._data[_pos];
  }

  self_t operator+(int offset){
    _pos += offset;
    return *this;
  }

  difference_type operator-(const self_t & rhs) const{
    return _pos - rhs._pos;
  }

  friend bool operator==(const self_t & lhs,
		         const self_t & rhs){
    return ( lhs._sparse_array == rhs._sparse_array && lhs._pos == rhs._pos ); 
  }

  friend bool operator!=(const self_t & lhs,
		         const self_t & rhs){
    return !(lhs == rhs);
  }

  // Pre-increment
  self_t & operator++(){
    increment(1);
    return *this;
  }

  // Post-increment
  self_t operator++(int){
    self_t old(*this);
    increment(1);
    return old;
  }

  self_t & operator+=(difference_type rhs){
    increment(rhs);
    return *this; 
  }

  // Pre-decrement
  self_t & operator--(){
    decrement(1);
    return *this;
  }

  // Post-decrement
  self_t operator--(int){
    self_t old(*this);
    decrement(1);
    return old;
  }



private:
  void increment(index_t add){
    _pos += add;
  }

  void decrement(index_t sub){
    _pos -= sub;
  }

private:
  pointer		_sparse_array;
  index_t		_pos;

}; // class sparse_array_iterator

template <class SparseArrayT>
class sparse_array_proxy_ref
{
  typedef sparse_array_proxy_ref	self_t;
  using value_type =	typename SparseArrayT::value_type;
  using index_t    =	typename SparseArrayT::index_t;

public:

  sparse_array_proxy_ref(SparseArrayT & sparse_array, index_t index)
  : _sparse_array(sparse_array)
  , _index(index)
  { }

  value_type operator=(const value_type & rhs){
    if(value_type() == rhs){
      this->_sparse_array._data.erase(_index);
      return value_type();
    }
    this->_sparse_array._data[_index] = rhs;
    return rhs;      
  }

// conversion operator
  operator const value_type() const {
    if(this->_sparse_array._data.emplace(_index, value_type()).second){
       this->_sparse_array._data.erase(_index);
       return value_type();       
    }
    else{
       return this->_sparse_array._data[_index];
    }
  }


private:
  SparseArrayT &	_sparse_array;
  index_t		_index; 

}; // class sparse_array_iterator

} // namespace detail


template <class T, std::size_t N>
class sparse_array
{
  // Just a suggestion:

  typedef sparse_array<T, N>           		        	self_t;
  typedef detail::sparse_array_proxy_ref<self_t>		proxy;
  typedef std::size_t						size_type;
  typedef size_type						index_t;
  typedef T					  		value_type;
  typedef std::unordered_map<size_type, value_type>		data;

public:

  typedef detail::sparse_array_iterator<self_t>  		iterator;
  typedef detail::sparse_array_iterator<const self_t, true>	const_iterator;
  typedef std::reverse_iterator<iterator>			reverse_iterator;
  typedef index_t                                 		difference_type;

  friend iterator;
  friend const_iterator;
  friend proxy;

public:

// constructor
  sparse_array()
  :_size(N)
  { }

// copy constructor
  sparse_array(const self_t & other)
  : _data(other._data)
  , _size(other._size)
  { }

// assign operator
  self_t & operator=(const self_t & rhs) = default;
    
// destructor
  ~sparse_array() = default;

// operators
  bool operator==(const self_t & rhs) const{
    return _data == rhs._data;
  }

  bool operator!=(const self_t & rhs) const{
    return !(*this == rhs);
  }

  proxy operator[](index_t i){
    return proxy(*this, i); 
  }

  const value_type & operator[](const index_t i) const{
    return *_data.find(i);
  }

// iterator
  iterator begin(){
    return iterator(this, 0);
  }

  iterator end(){
    return iterator(this, N);
  }

  const_iterator cbegin() const{
    return const_iterator(this, 0);
  }

  const_iterator cend() const{
    return const_iterator(this, N);
  }

  reverse_iterator rbegin(){
    return reverse_iterator(end());
  }

  reverse_iterator rend(){
    return reverse_iterator(begin());
  }


// capacity
  size_type size() const{
    return _size;
  }

  size_type max_size() const{
    return N;
  }

  bool empty() const{
    return 0 < N ? false : true;
  }

  bool operator<(const self_t & rhs) const{
    return std::lexicographical_compare(cbegin(), cend()
		                       ,rhs.cbegin(), rhs.cend()
				       );
  }

  bool operator>(const self_t & rhs) const{
    return !(*this < rhs);
  }

// element access
  proxy front(){
    return proxy(*this, 0);
  }

  proxy back(){
    return proxy(*this, N-1);
  }


  void swap(self_t & other){
	  std::swap(_data, other._data);
  }

  void fill(const value_type & value){
    if(value == value_type()){
      _data.clear();
    }
    else{
      std::fill(begin(), end(), value);
    }
  }

private:
  data		_data;
  size_type	_size;

};

} // namespace cpppc

#endif // CPPPC__S03__SPARSE_ARRAY_H__INCLUDED
