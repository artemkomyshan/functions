// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ak
{

template <typename...> struct Or;

template <> struct Or<> : public std::false_type
{
};

template <typename _Cond1> struct Or<_Cond1> : public _Cond1
{
};

template <typename _Cond1, typename _Cond2>
struct Or<_Cond1, _Cond2> : public std::conditional<_Cond1::value, _Cond1, _Cond2>::type
{
};

template <typename _Cond1, typename _Cond2, typename _Cond3, typename... _Condn>
struct Or<_Cond1, _Cond2, _Cond3, _Condn...>
    : public std::conditional<_Cond1::value, _Cond1, Or<_Cond2, _Cond3, _Condn...> >::type
{
};

template <typename...> struct And;

template <> struct And<> : public std::true_type
{
};

template <typename _Cond1> struct And<_Cond1> : public _Cond1
{
};

template <typename _Cond1, typename _Cond2>
struct And<_Cond1, _Cond2> : public std::conditional<_Cond1::value, _Cond2, _Cond1>::type
{
};

template <typename _Cond1, typename _Cond2, typename _Cond3, typename... _Condn>
struct And<_Cond1, _Cond2, _Cond3, _Condn...>
    : public std::conditional<_Cond1::value, And<_Cond2, _Cond3, _Condn...>, _Cond1>::type
{
};

template <typename _Cond> struct Not : public std::integral_constant<bool, !_Cond::value>
{
};

template <typename _Cond, typename Ret = void> using Requires = typename std::enable_if<_Cond::value, Ret>::type;

} // namespace ak