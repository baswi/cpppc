#ifndef CPPPC__S03__SPARSE_ARRAY_H__INCLUDED
#define CPPPC__S03__SPARSE_ARRAY_H__INCLUDED

// Might come in handy:
#include <unordered_map>


namespace cpppc {

namespace detail {


template <class SparseArrayT>
class sparse_array_iterator
{
  typedef sparse_array_iterator<SparseArrayT>		self_t;

public:
  using iterator_category	= std::random_access_iterator_tag;
  using value_type		= typename SparseArrayT::value_type;
  using difference_type		= std::ptrdiff_t;
  using reference		= value_type &;
  using pointer			= value_type *;
  using proxy			= typename SparseArrayT::proxy;
  	
public:

  sparse_array_iterator(SparseArrayT * sp_a, int position)
  : _sparse_array(sp_a)
  , _position(position)
  { }

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

  self_t & operator+=(int offset){
    increment(offset);
    return *this;
  }

  self_t operator+(int offset) const{
    self_t res(*this);
    res += offset;
    return res;
  }

  bool operator==(const self_t & rhs) const noexcept {
    return ( this == &rhs
	   || (_sparse_array == rhs._sparse_array && _position == rhs._position));
  }

  bool operator!=(const self_t & rhs) const noexcept {
    return !(*this == rhs);
  }

  proxy operator*() {
    return _sparse_array->at(_position);
  }

  const proxy operator*() const{ 		//hier ein prob ?!?
    return _sparse_array->at(_position);
  }

  proxy operator->() {
    return _sparse_array->at(_position);
  }

  const proxy & operator->() const{
    return _sparse_array->at(_position);
  }

  // Pre_decrement
  self_t & operator--(){
    decrement(1);
    return *this;
  }

  // Post_decrement
  self_t operator--(int){
    self_t old(*this);
    decrement(1);
    return old;
  }

  self_t & operator-=(int offset){
    decrement(offset);
    return *this;
  }

  self_t operator-(int offset) const{
    self_t res(*this);
    res -= offset;
    return res;
  }

  difference_type operator-(self_t rhs){
    return (_position - rhs._position);
  }

  proxy operator[](difference_type n){	  
    increment(n);
    return _sparse_array->at(_position);
  }

  bool operator<(self_t rhs){
    return ((rhs._position - _position) > 0); 
  }

  bool operator>(self_t rhs){
    return ((rhs._position - _position) < 0);
  }

  bool operator>=(self_t rhs){
    return !(*this < rhs);
  }

  bool operator<=(self_t rhs){
    return !(*this > rhs);
  }

private:

  void increment(int offset) {
    _position += offset;
  }

  void decrement(int offset) {
    _position -= offset;
  }

private:
  SparseArrayT *	_sparse_array;
  int			_position;

};

template <class SparseArrayT>
class sparse_array_proxy_ref
{
  typedef sparse_array_proxy_ref<SparseArrayT>		self_t;

  using value_type =	typename SparseArrayT::value_type;
  using index_t    =	typename SparseArrayT::index_t;

public:

  sparse_array_proxy_ref() = delete;

  sparse_array_proxy_ref(SparseArrayT & sp_array, index_t index)
  : _sp_array(sp_array)
  , _index(index)
  { }
 
  sparse_array_proxy_ref & operator=(value_type rhs){
    if(!(rhs == value_type())) this->_sp_array._data.emplace(_index, rhs);
    return *this;
  }

  operator const value_type() const {
    if(this->_sp_array._data.emplace(_index, value_type()).second){
       this->_sp_array._data.erase(_index);
       return value_type();       
    }
    else{
       return this->_sp_array._data[_index];
    }
  }

  // Pre_decrement
  self_t operator--(){
    if((this->_sp_array._data[_index] -1) == value_type()){
       this->_sp_array._data.erase(_index);
    }
    else{
       this->_sp_array._data[_index] -= 1;
    }
    return *this;
  }

 /* // Post_decrement
  self_t operator--(int){
    self_t old(*this);
    decrement(1);
    return old;
  }
*/
private:
  SparseArrayT & 	_sp_array;
  index_t		_index;
};

} // namespace detail


template <class T, std::size_t N>
class sparse_array
{
  // Just a suggestion:

  typedef size_t				  size_type;
  typedef sparse_array<T, N>                      self_t;
  typedef detail::sparse_array_proxy_ref<self_t>  proxy;
  typedef std::unordered_map<size_type, T>	  data;

public:

  typedef size_type                               index_t;
  typedef detail::sparse_array_iterator<self_t>   iterator;
  typedef const iterator			  const_iterator;
  typedef index_t                                 difference_type;
  typedef T					  value_type;
  typedef T&					  reference;
  typedef const T&				  const_reference;
  typedef value_type*				  pointer;
  typedef const pointer				  const_pointer;
  typedef std::reverse_iterator<iterator>	  reverse_iterator;
  typedef std::reverse_iterator<const_iterator>	  const_reverse_iterator;

  friend iterator;
  friend proxy;

public:

  // YOUR IMPLEMENTATION HERE
// ** constructor **
  sparse_array()
  : _size(N)
  , _begin(iterator(this, 0))
  , _end(iterator(this, N))
  {}
// ** destructor **
  ~sparse_array() = default;

// ** copy-const **
  sparse_array(const self_t & other)
  : _size(other._size)
  , _data(other._data)
  , _begin(iterator(this, 0))
  , _end(iterator(this, other._size))
  {}
 
// ** assign **
  self_t & operator=(const self_t & rhs){
    _data = rhs._data;
    return *this;
  }


  bool operator==(const self_t & rhs) const noexcept {
    return _data == rhs._data;
  }

  bool operator!=(const self_t & rhs) const noexcept {
    return !(*this == rhs);
  }

// Element access 
  proxy				operator[](index_t i){ 
    return proxy(*this, i);
  }

  const proxy			operator[](index_t i) const{  // hier kein proxy zurück geben -> T operator[](int offset) const
    return proxy(*this, i);
  }

  /*
   *
   T operator[](int offset) const{
      auto it_match = _data.find(offset);
      if (it_match == entries.end(){
         return _default;
	 }
	 return *it_match;

   }
   *
   */

  const proxy			at(index_t i) const {
    return proxy(*this, i);
  }

  proxy				at(index_t i){
    return proxy(*this, i);
  }

  proxy				front(){
    return proxy(*this, 0);
  }

  const proxy			front() const{
    return proxy(*this, 0);
  }

  proxy				back(){
    return proxy(*this, _size-1);
  }

  const proxy		 	back() const{
    return proxy(*this, _size-1);
  }


// Iterators

  iterator begin(){
    return _begin;
  }

  reverse_iterator rbegin(){
    return reverse_iterator(_end);
  }

  iterator end(){
    return _end;
  }

  reverse_iterator rend(){
    return reverse_iterator(_begin);
  }

// Capacity
  constexpr bool empty() const noexcept{
    return _data.empty();
  }

  constexpr size_type size() const noexcept{
    return _size;
  }

  constexpr size_type max_size() const noexcept{
    return _size;
  }


// Operations  

  void fill(const value_type & v){
    for(size_type i = 0; i < max_size(); ++i){
       _data[i] = v;
    } 
  }

  void swap(self_t & other){
    _data.swap(other._data);
  }

  std::pair<std::__detail::_Node_iterator<std::pair<const unsigned long, int>, false, false>, bool>
  emplace(index_t i, value_type v){
    return _data.emplace(i, v);
  }

  size_type erase(const index_t & i){
    return erase(i);
  }


private:
  const size_type	_size;
  data			_data;
  iterator		_begin;
  iterator      	_end;
};

} // namespace cpppc

#endif // CPPPC__S03__SPARSE_ARRAY_H__INCLUDED
