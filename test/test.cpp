// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <iostream>

#include "call_on_expire.cpp"
#include "call_once_silent.cpp"
#include "callback_guardian.cpp"
#include "not_empty_function.cpp"
#include "shared_function.cpp"

int main()
{
  std::cout << "TEST number: " << tests.size() << std::endl;
  auto count = 0u;
  for (auto& test : tests)
    {
      std::cout << count << " " << test.first << std::endl;
      ++count;
      test.second();
    }

  return 0;
}