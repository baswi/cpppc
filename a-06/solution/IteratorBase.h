#ifndef CPPPC__S06__ITERATORBASE_H__INCLUDED
#define CPPPC__S06__ITERATORBASE_H__INCLUDED

#include <iterator>


namespace bases{

template<
  class Derived,
  class ValueType    = typename Derived::value_type,
  class IndexType    = int,
  class Pointer      = ValueType *,
  class Reference    = ValueType & >
class IteratorBase{

  typedef IteratorBase<
            Derived,
            ValueType,
            IndexType,
            Pointer,
            Reference >          self_t;
  typedef IndexType		 index_type;
  typedef Derived                derived_t;
private:
  index_type _pos;

private:
  derived_t & derived() {
    return static_cast<derived_t &>(*this);
  }
  constexpr const derived_t & derived() const {
    return static_cast<const derived_t &>(*this);
  }

public:
  typedef std::random_access_iterator_tag       iterator_category;

  typedef ValueType                                    value_type;
  typedef index_type                              difference_type;
  typedef Pointer                                         pointer;
  typedef const Pointer                             const_pointer;
  typedef Reference                                     reference;
  typedef const Reference                         const_reference;

public:
  constexpr IteratorBase()                = default;
  constexpr IteratorBase(self_t &&)       = default;
  constexpr IteratorBase(const self_t &)  = default;
  ~IteratorBase()                         = default;
  self_t & operator=(self_t &&)           = default;
  self_t & operator=(const self_t &)      = default;


  reference operator*() const{
    return derived().dereference();
  }

  derived_t operator+(int offset){
    derived().increment(offset);
    return derived();
  }

  difference_type operator-(const derived_t & rhs) const{
//TODO: find a way where you don't have to access the private member _pos directly	  
    return derived()._pos - rhs._pos;
  }

 bool operator==(const derived_t & rhs) const{
//TODO: find a way where you don't have to access the private member _pos directly	  
    return ( derived()._pos == rhs._pos ); 
  }

  bool operator!=(const derived_t & rhs) const{
    return !(derived() == rhs);
  }

  // Pre-increment
  derived_t & operator++(){
    derived().increment(1);
    return derived();
  }

  // Post-increment
  derived_t operator++(int){
    derived_t old(derived());
    derived().increment(1);
    return old;
  }

  derived_t & operator+=(difference_type rhs){
    derived().increment(rhs);
    return derived(); 
  }

  // Pre-decrement
  derived_t & operator--(){
    derived().decrement(1);
    return derived();
  }

  // Post-decrement
  derived_t operator--(int){
    derived_t old(derived());
    derived().decrement(1);
    return old;
  }


}; // class IteratorBase


} // namespace bases


#endif // CPPPC__S06__ITERATORBASE_H__INCLUDED 
