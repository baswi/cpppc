#ifndef CPPPC__S06__SPARSE_ARRAY_H__INCLUDED
#define CPPPC__S06__SPARSE_ARRAY_H__INCLUDED

// Might come in handy:
#include <unordered_map>

#include "IteratorBase.h"

namespace cpppc {

namespace detail {

template <class SparseArrayT>
class sparse_array_iterator
  :   public bases::IteratorBase_rnd_acc<
	sparse_array_iterator<SparseArrayT>   // Derived
       ,typename SparseArrayT::proxy          // ValueType
       ,int                                   // IndexType -> difference_type
       ,SparseArrayT*                         // Pointer
       ,typename SparseArrayT::proxy          // Reference
      >
{  
  using self_t	   =    sparse_array_iterator;
  using index_t    =	int;
  using proxy	   =	typename SparseArrayT::proxy;
  using base_t     =    bases::IteratorBase_rnd_acc< self_t, proxy, index_t, SparseArrayT*, proxy >;

public:
  using pointer			= SparseArrayT*;
 
  	
public:
// constructor
  sparse_array_iterator(pointer sparse_array, index_t pos)
  : _sparse_array(sparse_array)
  , _pos(pos)
  {  }

  sparse_array_iterator(const sparse_array_iterator<SparseArrayT>& i)
  : _sparse_array(i._sparse_array)
  , _pos(i._pos)
  {  }


// operators

// do I rly need to check _sparse_array == rhs._sparse_array ?!?
 bool operator==(const self_t & rhs) const{
    return ( _sparse_array == rhs._sparse_array && _pos == rhs._pos ); 
  }



private:
  friend base_t;

  proxy dereference() const{
  //  return proxy(static_cast<reference>(*_sparse_array), _pos);
    return (*_sparse_array)[_pos];
  }

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

  sparse_array_proxy_ref() = delete;

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
  typedef std::size_t						size_type;
  typedef size_type						index_t;

public:

  typedef T					  		value_type;
  typedef std::unordered_map<size_type, value_type>		data;
  typedef detail::sparse_array_proxy_ref<self_t>		proxy;
  typedef detail::sparse_array_iterator<self_t>  		iterator;
  typedef detail::sparse_array_iterator<const self_t> 	        const_iterator;
  typedef std::reverse_iterator<iterator>			reverse_iterator;
  typedef index_t                                 		difference_type;

  friend iterator;
  friend const_iterator;
  friend reverse_iterator;
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

// move constructor

  sparse_array(self_t && other)
  : _data(std::move(other._data))
  , _size(other._size)
  { }

// move assign operator

  self_t & operator=(self_t && rhs) = default; // since memberwise move should be exactly what we want
    
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
    if(_data.find(i) == _data.end())
      return std::move(value_type());
    return _data.find(i)->second;
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

#endif // CPPPC__S06__SPARSE_ARRAY_H__INCLUDED
