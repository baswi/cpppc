#ifndef CPPPC__S04__LAZY_SEQUENCE_H__INCLUDED
#define CPPPC__S04__LAZY_SEQUENCE_H__INCLUDED

#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>

namespace cpppc {

template<typename ValueType>
class lazy_sequence;

template<typename ValueType>
class lazy_sequence_iterator
{

    typedef lazy_sequence_iterator<ValueType>		self_t;
    typedef ValueType					value_type;
   // see comment at Member   typedef int						position_type;
public:
    typedef std::function<value_type(value_type)>	functor;


    lazy_sequence_iterator() = delete;

    lazy_sequence_iterator(functor func, value_type value) 
    : _func(func) 
    , _value(value)
    { }

    bool operator == (const self_t& rhs) {
	return ((this == &rhs)
		|| (_pos == rhs._pos )  // maybe it would be better to have &lazy_sequence as a Member
		                        // since _func == rhs_func does not check for equal function
	       );
    }
    bool operator != (const self_t& rhs) {
        return !(*this==rhs);
    }
    const value_type& operator * () const {
        return _value;
    }
    lazy_sequence_iterator operator ++ () {
        //TODO
        return *this;
    }

private:
    functor		_func;
    value_type 		_pos;
   // position_type	_pos;    // -> here _pos == _value

}; // class lazy_sequence_iterator

template <typename ValueType>
class lazy_sequence {

   typedef lazy_sequence<ValueType>			self_t;
   typedef ValueType					value_t;
   typedef size_t					size_type;
   
public:
   typedef lazy_sequence_iterator<ValueType>		iterator;
   typedef const iterator				const_iterator;
   typedef std::function<value_type(value_type)>	functor;

   lazy_sequence(size_type size, functor func)
   : _size(size)
   , _func(func) 
   { }

   iterator begin() {
       return iterator(_func, 0);
   }
   iterator end() {
       return iterator(_func, _size);  // hmm maybe let the iterator know the size to determine the true end?
   }

   size_type size(){
      return end() - begin();
   }

private:
   functor 	_func;
   size_type	_size;
   

}; // class lazy_sequence

} // namespace cpppc

#endif // CPPPC__S04__LAZY_SEQUENCE_H__INCLUDED
