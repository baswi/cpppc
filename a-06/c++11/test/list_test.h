#ifndef CPPPC__05__LIST_TEST_H__INCLUDED
#define CPPPC__05__LIST_TEST_H__INCLUDED

#include <gtest/gtest.h>
#include "TestBase.h"

class ListTest : public ::testing::Test {

public:

  ListTest() {
    LOG_MESSAGE("Opening test suite: ListTest");
  }

  virtual ~ListTest() {
    LOG_MESSAGE("Closing test suite: ListTest");
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

template<class A>
A f(A a) { return a; };

#endif // CPPPC__05__LIST_TEST_H__INCLUDED
