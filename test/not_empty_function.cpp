// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <memory>

#include "ak/not_empty_function.hpp"

#include "test.hpp"

TEST(not_empty_function__)
{

  not_empty_function<int()> f1([] { return 1; });

  assert(f1);
  assert(f1() == 1);

  not_empty_function<int()> f2 = f1;

  f1 = [] {
    return 5;
  };

  assert(f1);
  assert(f1() == 5);
  assert(f2);
  assert(f2() == 1);

  std::swap(f1, f2);

  assert(f1);

  assert(f1() == 1);

  assert(f2);

  assert(f2() == 5);

  assert(!(f1 == nullptr));
  assert(!(nullptr == f1));
  assert(f1 != nullptr);
  assert(nullptr != f1);

  not_empty_function<int(int, int)> sum = [](int a, int b) {
    return a + b;
  };

  assert(sum);
  assert(sum(1, 3) == 4);
};

class FunctionStorage
{
public:
  FunctionStorage(not_empty_function<int(int)> const& func) : mFunc(func) {}

  int invoke(int i)
  {
    return mFunc(i);
  }

private:
  not_empty_function<int(int)> mFunc;
};

TEST(not_empty_function_example)
{

  FunctionStorage fs([](int i) { return i * i; });
  assert(fs.invoke(3) == 9);

  FunctionStorage fs3([](int const& i) { return i - 2; });
  assert(fs3.invoke(5) == 3);
};