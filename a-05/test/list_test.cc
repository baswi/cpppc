/* ------------------------------------------------------------------------ *
 *                       DO NOT MODIFIY THIS FILE                           *
 * -------------------------------------------------------------------------*/

#include "list_test.h"

#include <solution/list.h>
#include <gtest/gtest.h>
using cpppc::list;
TEST_F(ListTest, StandardConcept)
{
  LOG_MESSAGE("listTest.StandardConcept: list__new");
  list<int> v1;
  list<int> v2;

  LOG_MESSAGE("listTest.StandardConcept: list__equals");
  ASSERT_EQ(v1,v2);

  v1.push_front(12);
  v1.push_front(34);

  ASSERT_EQ(1,v1!=v2);

  LOG_MESSAGE("listTest.StandardConcept: list__assign");
  v2=v1;

  ASSERT_EQ(v1,v2);

  LOG_MESSAGE("listTest.StandardConcept: list__copy");
  list<int> v3 (v1);

  ASSERT_EQ(v1,v3);
}

TEST_F(ListTest, ListConcept)
{
  LOG_MESSAGE("listTest.ListConcept: list__new");
  list<int> v;

  ASSERT_EQ(0,  v.size());
  ASSERT_EQ(1,  v.empty());

  LOG_MESSAGE("listTest.ListConcept: list__push_front(56)");
  v.push_front(56);
  LOG_MESSAGE("listTest.ListConcept: list__push_front(78)");
  v.push_front(78);
  LOG_MESSAGE("listTest.ListConcept: list__push_front(90)");
  v.push_front(90);

 // ASSERT_EQ(v.end(), v.begin() + v.size());

  LOG_MESSAGE("listTest.ListConcept: list__at");
  ASSERT_EQ(3,  v.size());
  ASSERT_EQ(0,  v.empty());
  ASSERT_EQ(56, v[2]);
  ASSERT_EQ(78, v[1]);
  ASSERT_EQ(90, v[0]);

  for (int i = 0; i < v.size(); i++) {
    LOG_MESSAGE("listTest.ListConcept: v[%d] : %d",
                i, v[i]);
  }

  LOG_MESSAGE("listTest.ListConcept: list__push_front(34)");
  v.push_front(34);
  LOG_MESSAGE("listTest.ListConcept: list__push_front(12)");
  v.push_front(12);

  for (int i = 0; i < v.size(); i++) {
    LOG_MESSAGE("listTest.ListConcept: v[%d] : %d",
                i, v[i]);
  }

  LOG_MESSAGE("listTest.ListConcept: list__at");
  ASSERT_EQ(5,  v.size());
  ASSERT_EQ(0,  v.empty());
  ASSERT_EQ(12, v[0]);
  ASSERT_EQ(34, v[1]);
  ASSERT_EQ(56, v[4]);

  LOG_MESSAGE("listTest.ListConcept: list__copy");
  list<int> vcopy (v);

  LOG_MESSAGE("listTest.ListConcept: list__begin, list__end");
  list<int>::iterator viter = v.begin();
  list<int>::iterator vend  = v.end();

  for (; viter != vend; ++viter) {
    *viter += 100;
  }

  LOG_MESSAGE("listTest.VectorConcept: vector__at");
  for (int i = 0; i < vcopy.size(); i++) {
    ASSERT_EQ(v[i], vcopy[i] + 100);
  }

  LOG_MESSAGE("listTest.ListConcept: list__assign(v_empty)");
  list<int> v_empty;
  v=v_empty;

  ASSERT_EQ(v,v_empty);
  ASSERT_EQ(0, v.size());
  ASSERT_EQ(1, v.empty());
} 


TEST_F(ListTest, ListLIFO)
{
  LOG_MESSAGE("listTest.ListLIFO: list__new");
  list<int> v;

  LOG_MESSAGE("listTest.ListLIFO: list__push_front(...)");
  v.push_front(444);
  v.push_front(333);
  v.push_front(222);
  v.push_front(111);
  ASSERT_EQ(4,   v.size());

  LOG_MESSAGE("listTest.ListLIFO: list__pop_front(...)");
  ASSERT_EQ(111, v.pop_front());
  ASSERT_EQ(222, v.pop_front());
  ASSERT_EQ(333, v.pop_front());
  ASSERT_EQ(444, v.pop_front());
  ASSERT_EQ(0,   v.size());

  LOG_MESSAGE("listTest.ListLIFO: list__push_front(...)");
  v.push_front(111);
  v.push_front(222);
  v.push_front(333);
  v.push_front(444);
  ASSERT_EQ(4,   v.size());

  LOG_MESSAGE("listTest.ListLIFO: list__pop_front(...)");
  ASSERT_EQ(444, v.pop_front());
  ASSERT_EQ(333, v.pop_front());
  ASSERT_EQ(222, v.pop_front());
  ASSERT_EQ(111, v.pop_front());
  ASSERT_EQ(0,   v.size());

  LOG_MESSAGE("listTest.ListLIFO: list__delete");
  v.~list();
}
TEST_F(ListTest, ListMOVES)
{
  LOG_MESSAGE("listTest.ListMOVES: list__new");
  list<int> v1, v2, v3, v4;

  LOG_MESSAGE("listTest.ListMOVES: list__move-assign from rvalue temporary");
  v1 = f(list<int>());
  
  LOG_MESSAGE("listTest.ListLIFO: list__push_front(...)");
  v3.push_front(444);
  v3.push_front(333);
  v3.push_front(222);
  v3.push_front(111);

  // Copy assignment
  v1 = v3;

  // sa2 is a copy of sa1
  ASSERT_EQ(444, v1[3]);
  ASSERT_EQ(333, v1[2]);
  ASSERT_EQ(222, v1[1]);
  ASSERT_EQ(111, v1[0]);

  // Move assignment
  v4 = std::move(v1);

  // sa1 is empty, the original values are in sa3
  ASSERT_EQ(true, v1.empty());
  ASSERT_EQ(0, v1.size());
  ASSERT_EQ(444, v4[3]);
  ASSERT_EQ(111, v4[0]);
}
