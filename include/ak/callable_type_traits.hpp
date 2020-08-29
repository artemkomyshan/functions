// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ak
{

template <typename From, typename To>
using check_func_return_type =
    Or<std::is_void<To>, std::is_same<From, To>, std::is_convertible<From, To>>;

}