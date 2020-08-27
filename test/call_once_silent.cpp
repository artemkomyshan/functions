// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "ak/call_once_silent.hpp"

#include "test.hpp"

using namespace ak;

TEST (call_once_silent_simple_void_call)
{
  auto count = 0u;
  call_once_silent<> vf = [&count] () mutable { ++count; };

  assert (0u == count);

  vf ();
  assert (1u == count);

  vf ();
  assert (1u == count);
};

TEST (call_once_silent_simple_int_params_call)
{
  auto count = 0u;
  call_once_silent<int, int> vf = [&count] (int, int) mutable { ++count; };

  assert (0u == count);

  vf (1, 2);
  assert (1u == count);

  vf (2, 4);
  assert (1u == count);
};

TEST (call_once_silent_simple_move)
{
  auto sum = 0u;
  call_once_silent<int, int> vft = [&sum] (int a, int b) mutable { sum = a + b; };
  call_once_silent<int, int> vf = std::move (vft);
  vft = nullptr;

  vft (1, 1);
  assert (0u == sum);

  vf (1, 2);
  assert (3u == sum);

  vf (2, 4);
  assert (3u == sum);
};

TEST (call_once_silent_null_ptr)
{
  call_once_silent<int, int> vf1 = [] (int, int) mutable {};
  assert (vf1 != nullptr);

  vf1 = nullptr;
  assert (vf1 == nullptr);

  call_once_silent<int, int> vf2;
  assert (vf2 == nullptr);

  call_once_silent<int, int> vf3 = nullptr;
  assert (vf3 == nullptr);
};
