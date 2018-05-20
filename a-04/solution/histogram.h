#ifndef CPPPC__S03__HISTOGRAM_H__INCLUDED
#define CPPPC__S03__HISTOGRAM_H__INCLUDED

// Some headers you might find useful:
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>


template <class Iter>
inline Iter histogram(Iter first, Iter last) {
    typedef typename std::iterator_traits<Iter>::value_type      value_t;
    typedef typename std::iterator_traits<Iter>::difference_type index_t;

    // YOUR IMPLEMENTATION HERE
    
    // check if there are regular 0 values
    Iter zero_pos = std::find(first, last, 0);
    value_t zero_count = 0;
    if(zero_pos != last) zero_count = std::count(zero_pos, last, 0); 

    Iter pos = first;
    std::for_each(first, last, [&](value_t &v){
		                           if(v){
		                              value_t temp = v; // remember to set all occureneces to 0
					      pos = std::find(pos, last, v); // pos is needed since old counts could be equal to v
		                              v = std::count(pos, last, v);
					      ++pos;
					      std::for_each(pos, last, [&](value_t &vv){ if(temp == vv) vv = 0; }); //set all occurences of v to 0
					   }
		                });
    if(zero_pos != last) *zero_pos = zero_count;

    // sort all 0 to the end
    pos = std::find(first, last, 0);
    Iter notZero = last;
    if(pos != last){
       std::for_each(pos, last, [&](value_t &v){ if(0==v){
		                                    notZero = std::find_if(pos, last, [](value_t &v){ return v!=0; });
						    std::swap(*pos, *notZero);
						    pos = std::find(pos, last, 0);
		                                 }
				 });
    }
    // der versuch führt zum selben ergebnis :(
   
    notZero = first;
    int count = 0;
    std::for_each(first, pos, [&](value_t &v){ if(v && count)++notZero; ++count;}); 
    return notZero;
   // return pos;
   /* std::vector<value_t> copy;
    std::copy(first, last, copy.begin());
    return std::copy(copy.begin(), copy.end(), first);*/ // -> führt zu nem segfault ... o.O T.T ?!?!?!?
}

#endif // CPPPC__S03__HISTOGRAM_H__INCLUDED
