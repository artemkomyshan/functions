// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ak
{

template <typename...>
struct Or;

template <>
struct Or<> : public std::false_type
{
};

template <typename Cond1>
struct Or<Cond1> : public Cond1
{
};

template <typename Cond1, typename Cond2>
struct Or<Cond1, Cond2>
    : public std::conditional<Cond1::value, Cond1, Cond2>::type
{
};

template <typename Cond1, typename Cond2, typename Cond3, typename... CondN>
struct Or<Cond1, Cond2, Cond3, CondN...>
    : public std::conditional<Cond1::value, Cond1,
                              Or<Cond2, Cond3, CondN...>>::type
{
};

template <typename...>
struct And;

template <>
struct And<> : public std::true_type
{
};

template <typename Cond1>
struct And<Cond1> : public Cond1
{
};

template <typename Cond1, typename Cond2>
struct And<Cond1, Cond2>
    : public std::conditional<Cond1::value, Cond2, Cond1>::type
{
};

template <typename Cond1, typename Cond2, typename Cond3, typename... CondN>
struct And<Cond1, Cond2, Cond3, CondN...>
    : public std::conditional<Cond1::value, And<Cond2, Cond3, CondN...>,
                              Cond1>::type
{
};

template <typename Cond>
struct Not : public std::integral_constant<bool, !Cond::value>
{
};

template <typename Cond, typename Ret = void>
using Requires = typename std::enable_if<Cond::value, Ret>::type;

} // namespace ak