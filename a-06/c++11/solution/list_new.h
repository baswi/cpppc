#ifndef CPPPC__S06__LIST_H__INCLUDED
#define CPPPC__S06__LIST_H__INCLUDED

#include "IteratorBase.h"

namespace cpppc
{

// list<T>::iterator
    template< class T >
    class list_iterator
      : public bases::IteratorBase<
  	  list_iterator<T>                      // Derived
         ,T                                     // ValueType
         ,int                                   // IndexType -> difference_type
         ,list_node<T>*                         // Pointer
         ,T&                                    // Reference
        >
    {
      typedef list_iterator<T>     self_t;
      typedef list_node<T>         node_t;
      using int                 =  index_t;
      using base_t              =  bases::IteratorBase_forward< self_t, T, index_t, list_node<T>*, T& >;
    public:

      iterator(list_node * node)
      : _list_node(node)
      { }


      ValueT & dereference() const{
        return _list_node->value;
      }


    private:
      friend base_t;

      void increment(index_t offset) {
//TODO
      }

      pointer pos() {
        return _list_node;
      }

      list_node * _list_node;
    }; // list_iterator


//NODE
template<class T>
struct list_node {
  list_node() = default;

  list_node * next = nullptr;
  T                  value;
};
//


template <class T> 
class list {
  typedef list<T>                 self_t;
  typedef liste_node<T>           node_t;
  typedef size_t                  size_type;
  public:

  typedef T                       value_type;
  typedef T *                     pointer;
  typedef T &                     reference;
  typedef const T &               const_reference;
  typedef list_iterator<T>        iterator;
  typedef list_iterator<const T>  const_iterator;

public:
  list()
  : _list_head()
  {}

  ~list(){
    while (not empty()){
      pop_front();
    }
  }

  list(const self_t & other)
  : _list_head(other._list_head)
  {
    *this=other;
  }

  self_t & operator=(const self_t & rhs) { //assign
    while (not empty()){pop_front();}
    int size=rhs.size();
    for (int i=size;i>0;i--){
      push_front(rhs[i-1]);
    }
    return *this;
  }

// move constructor

  list(self_t && other)
  , _list_head(std::move(other._list_head))
  { }

// move assign operator

  self_t & operator=(self_t && rhs){
	  
    while (!empty()){pop_front();}
    _list_head = std::move(rhs._list_head);
    return *this;
  }
    
// Iterators
  iterator begin() const{
      return iter(_list_head);
    }

  iterator end() const{
      return iter(node_t());
    }

  bool operator==(const self_t & other) const{
    return _list_head == other._list_head;
  }

  bool operator!=(const self_t & other) const{
    return not (*this==other);
  }

  reference operator[](int i) const{
    return *(begin().increment(i));
  }

  size_type size() const{
    size_type s=0;
    for(iterator i=begin(); i!=end(); ++i, ++s){}
    return s;
  }


  bool empty() const{
    return (begin()==end());
  }

  void push_front(value_type value){
    node * new_node = new node;
    new_node->next = &_list_head;
    new_node->value = value;
    _list_head = new_node;
  }

  value_type pop_front(){
    if(empty())
	  return value_type();

    value_type res = _list_head.value;
    list_node * node= &_list_head;
    _list_head = _list_head->next;
    delete node;
    return res;
  }


  private:
    node       _list_head;
  };

}

#endif // CPPPC__S06__LIST_H__INCLUDED
