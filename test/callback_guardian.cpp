// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "ak/callback_guardian.hpp"

#include "test.hpp"

using namespace ak;

class Test
{
  callback_guardian guard;
  std::function<void()> cb;
  std::function<void()> error_cb;

public:
  void set_cb(std::function<void()> call)
  {
    cb = call;
  }

  void set_error_cb(std::function<void()> call)
  {
    error_cb = call;
  }

  auto get_cb()
  {
    return guard.make_guarded_callback([this] { cb(); },
                                       [e = error_cb] { e(); });
  }
};

TEST(callback_guardian_simple)
{
  Test guarded;
  int cout_cb = 0;
  int cout_error_cb = 0;

  guarded.set_cb([&cout_cb]() mutable { ++cout_cb; });
  guarded.set_error_cb([&cout_error_cb]() mutable { ++cout_error_cb; });
  guarded.get_cb()();

  assert(cout_cb == 1);
  assert(cout_error_cb == 0);
};

TEST(callback_guardian_expired)
{
  int cout_cb = 0;
  int cout_error_cb = 0;

  std::function<void()> call;
  {
    Test guarded;
    guarded.set_cb([&cout_cb]() mutable { ++cout_cb; });
    guarded.set_error_cb([&cout_error_cb]() mutable { ++cout_error_cb; });
    call = guarded.get_cb();
  }
  call();
  assert(cout_cb == 0);
  assert(cout_error_cb == 1);
};

TEST(callback_guardian_copy)
{
  int cout_cb = 0;
  int cout_error_cb = 0;

  Test log_life;

  std::function<void()> call;
  {
    Test guarded;
    guarded.set_cb([&cout_cb]() mutable { ++cout_cb; });
    guarded.set_error_cb([&cout_error_cb]() mutable { ++cout_error_cb; });
    call = guarded.get_cb();

    log_life = guarded;
  }
  call();
  assert(cout_cb == 0);
  assert(cout_error_cb == 1);
};