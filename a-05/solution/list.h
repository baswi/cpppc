
//#include <iterator>
#include <iostream>

namespace cpppc
{
  // forward declaration
  template <typename ValueT, ValueT   default_value = ValueT()> class list;


  template <typename ValueT, ValueT   default_value> class list {
    typedef list<ValueT, default_value> self_t;
  public:

    //NODE
    struct list_node {
      list_node * next;
      ValueT      value;
    };
    //

// list<T>::iterator
    class iterator {
      typedef typename cpppc::list<ValueT, default_value> list_t;
      typedef typename list_t::list_node list_node;
      typedef typename list_t::iterator self_t;
    public:
       //iterator() = delete;

      iterator(list_node * node) //initialwert
      : _list_node(node)
      {//set_member(&_tail);
       //
      }

      iterator & operator++() { //prefix
        _list_node = _list_node->next;
        return *this;
      }

      iterator operator++(int) { //postfix
        iterator old = *this;
        _list_node = _list_node->next;
        return old;
      }

      ValueT & operator*() {
        return _list_node->value;
      }

      const ValueT & operator*() const {
        return _list_node->value;
      }

      bool operator==(const self_t & rhs) const {
        return  (_list_node == &rhs); // equal member
      }

      bool operator!=(const self_t & rhs) {
        return not (*this==rhs);
      }

      list_node *  operator&(){ //getter von list_node von außerhalb liegender iteratoren
        return _list_node;
      }

      const list_node * operator&() const{ //getter von list_node von außerhalb liegenden iteratoren
        return _list_node;
      }

      void set_member(list_node * node){
        _list_node=node;
      }


    private:
      list_node * _list_node;
    };
////////Ende Iterator




public:
  list()    //Konstruktor
  : _tail(),_end(&_tail), _begin(&_tail) //wichtig um iterator zu bilden..?
 //
  {}

  ~list(){    //Destruktor
    while (!empty()){
      pop_front();
    }
  }

  list(const self_t & other)  //copy
  : _tail(),_end(&_tail), _begin(&_tail)
  {
    *this=other;
  }

  self_t & operator=(const self_t & rhs) { //assign
    while (!empty()){pop_front();}
    int size=rhs.size();
    for (int i=size;i>0;i--){
      push_front(rhs[i-1]);
    }
    return *this;
  }

// move constructor

  list(self_t && other)
  : _tail()
  , _end(&_tail)
  , _begin(std::move(other._begin))
  { other._begin = _end; }

// move assign operator

  self_t & operator=(self_t && rhs){
    while (!empty()){pop_front();}
    _begin = std::move(rhs._begin);
    rhs._begin = other._end;
    return *this;
  }
    

    iterator begin() const{
      return _begin;
    }

    const iterator & end() const{ //siehe oben
      return _end;
    }  // Iterator mit nullptr

  bool operator==(const self_t & other) const{
    if (empty() && other.empty()) return true; //leere listen sind gleich
    if (this==&other) return true; //Gleichheit der Adressen
    iterator iter_this=begin();
    iterator iter_other=other.begin();

    while (iter_this!=_end && iter_other!=other.end()){
      if (*iter_this!=*iter_other) return false;  //elementweiser Vergleich der Werte von list_node
      iter_other++;
      iter_this++;
    }
    if (iter_this==_end && iter_other==other.end()) return true;
    else return false;
  }

  bool operator!=(const self_t & other) const{
    return not (*this==other);
  }

  ValueT & operator[](const int offset){ //const int offset..? vermutlich besser um unerwartete Seiteneffekte zu vermeiden zum beispiel mit funktion(i++) wobei das i dann verändert würde
    int i=offset;
    iterator iter=begin();
    while (iter!=_end && 0<i){iter++;i--;}
    return *iter; //bekommen default_value falls wert von offset zu groß
  }

  const ValueT & operator[](const int offset) const {
    int i=offset;
    iterator iter=begin();
    while (iter!=_end && 0<i){iter++;i--;}
    return *iter; //bekommen default_value falls wert von offset zu groß
    }

  int size() const{
    int i=0;
    iterator iter=begin();
    while (iter!=_end) { ++iter;++i;}
    return i;
  }


bool empty() const{
  return (_begin==_end);
}

void push_front(ValueT value){
  list_node * new_node=new list_node;
  new_node->next=&_begin; //alter Kopf wird hinter den neuen Kopf angefügt
  _begin.set_member(new_node);  //neuer Kopf führt die Liste an
  new_node->value=value;
}

ValueT pop_front(){
  //wenn leer potentieller fehler
  ValueT res= *_begin; //wie im iterator
  list_node * node= &_begin;
  ++_begin;
  delete node;
  return res;
}


  private:
    //static constexpr
    list_node _tail = { nullptr, default_value }; //nullterminierung //dummyobjekt
    const iterator _end; // wird mit _tail  init; letztes node.next der kette verweist auf _end
    iterator _begin; //iterator der auf erstes Element zeigt; liste zu anfang leer per Standardcopykonstruktor

  };

}

