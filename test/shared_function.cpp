// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "ak/shared_function.hpp"

#include "test.hpp"

using namespace ak;

TEST(shared_function_simple_void_call)
{
  auto count = 0u;
  auto vf = shared_function<void()>([&count]() mutable { ++count; });

  assert(0u == count);

  vf();
  assert(1u == count);

  vf();
  assert(2u == count);
};

TEST(shared_function_simple_default_ctor)
{
  shared_function<void(int)> f;
  assert(f == nullptr);

  try
    {
      f(5);
      assert(!"Exception expected");
    }
  catch (std::bad_function_call const&)
    {
    }
  catch (...)
    {
      assert(!"Unexpected exception");
    }
};

TEST(shared_function_simple_move)
{
  auto sum = 0u;
  auto vft = shared_function<void(int, int)>(
      [&sum](int a, int b) mutable { sum = a + b; });
  auto vf = shared_function<void(int, int)>(std::move(vft));
  vft = nullptr;

  try
    {
      vft(1, 1);
      assert(!"Exception expected");
    }
  catch (std::bad_function_call const&)
    {
    }
  catch (...)
    {
      assert(!"Unexpected exception");
    }

  vf(1, 2);
  assert(3u == sum);
  auto vf1 = shared_function<void(int, int)>([](int, int) mutable {});
  assert(vf1 != nullptr);

  vf1 = nullptr;
  assert(vf1 == nullptr);

  shared_function<int(int)> vf2;
  assert(vf2 == nullptr);

  shared_function<int(int)> vf3 = nullptr;
  assert(vf3 == nullptr);
};

TEST(shared_function_shared)
{
  auto sum = 0;
  shared_function<int(int, int)> vf;
  {
    auto vft = shared_function<int(int, int)>(
        [&sum](int a, int b) mutable { return sum += a + b; });
    vf = vft;

    assert(6 == vft(2, 4));
  }
  
  assert(9 == vf(2, 1));
};