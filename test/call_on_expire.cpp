// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "ak/call_on_expire.hpp"

#include "test.hpp"

using namespace ak;

TEST(call_on_expire_local)
{
  auto called = false;

  {
    call_on_expire coe{[&called]() mutable {
      called = true;
    }};
    assert(!called);

    call_on_expire ocoe = coe;
    assert(!called);
  }

  assert(called);
};

TEST(call_on_expire_release)
{
  auto called = false;

  call_on_expire coe;

  {
    call_on_expire lcoe{[&called]() mutable {
      called = true;
    }};
    assert(!called);

    coe = lcoe;
    assert(!called);
  }

  assert(!called);

  coe.release();

  assert(called);
};