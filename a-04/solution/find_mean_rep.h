#ifndef CPPPC__S03__FIND_MEAN_REP_H__INCLUDED
#define CPPPC__S03__FIND_MEAN_REP_H__INCLUDED

// Some headers you might find useful:
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

namespace cpppc {

template <class Iter>
Iter find_mean_rep(Iter first, Iter last) {
  typedef typename std::iterator_traits<Iter>::value_type value_t;

  // YOUR IMPLEMENTATION HERE


  int n = std::distance(first, last); // <- this makes it 'automatically' more efficient for random access iterator ranges 
                                      //    (according to cppreference - distance -> Type requirements)
  if(!n) return last; // if empty return end()

  value_t mean = std::accumulate(first, last, value_t()) / n;
  std::for_each(first, last, [=](value_t &v){ v = (mean - v > 0) ? mean - v : v - mean; });
  value_t min = *first;
  std::for_each(first, last, [&](value_t &v){ min = (min > v) ? v : min; });

  return std::find(first, last, min);
}

} // namespace cpppc

#endif // CPPPC__S03__FIND_MEAN_REP_H__INCLUDED
