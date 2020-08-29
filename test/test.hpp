// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::function<void()>> tests;

struct test_t
{
  test_t(std::string _name) : name(_name) {}

  std::string name;

  template <typename Func>
  test_t& operator<<(Func&& func)
  {
    tests.emplace(name, std::forward<Func>(func));
    return *this;
  }
};

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define TEST(NAME) auto NAME = test_t{STRINGIFY(NAME)} << []