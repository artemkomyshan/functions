// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

#include "requires.hpp"

/**
 * not_empty_function makes it obvious to a reader (human or machine) that a
 * test for nullptr is not necessary before calling. A function with a
 * not_empty_function parameter makes it clear that the caller of the function
 * is responsible for any nullptr checks that may be necessary. Similarly, a
 * function with a return value of not_empty_function makes it clear that the
 * caller of the function does not need to check for nullptr.
 *
 * // It is the caller's job to make sure cb is valid function ( cb != nullptr)
 * void request(not_empty_function<void(int)> cb);
 *
 */

namespace ak
{

template <typename Signature> class not_empty_function;

template <typename Ret, typename... Args> class not_empty_function<Ret (Args...)>
{
  template <typename From, typename To>
  using check_func_return_type = Or<std::is_void<To>, std::is_same<From, To>, std::is_convertible<From, To> >;

  template <typename Func, typename Ret2 = typename std::result_of<Func &(Args...)>::type>
  struct Callable : check_func_return_type<Ret2, Ret>
  {
  };

public:
  explicit operator bool () const;

  Ret operator() (Args... args) const;

  /// Constructors create an empty function call are deleted.
  not_empty_function () = delete;
  not_empty_function (std::nullptr_t) = delete;
  not_empty_function &operator= (std::nullptr_t) = delete;

  /// Copy on move, since default implemetation clears source object.
  not_empty_function (not_empty_function &&other);
  not_empty_function &operator= (not_empty_function &&other);

  not_empty_function (const not_empty_function &other) = default;
  not_empty_function &operator= (const not_empty_function &other) = default;

  template <typename Func,
            typename = Requires<Not<std::is_same<typename std::decay<Func>::type, not_empty_function> > >,
            typename = Requires<Callable<Func> > >
  not_empty_function (Func &&call);

  template <typename Func,
            typename = Requires<Not<std::is_same<typename std::decay<Func>::type, not_empty_function> > >,
            typename = Requires<Callable<Func> > >
  not_empty_function &operator= (Func &&call);

  void swap (not_empty_function &other);

private:
  std::function<Ret (Args...)> mCall;
};

template <typename Ret, typename... Args>
not_empty_function<Ret (Args...)>::not_empty_function (not_empty_function &&other) : mCall (other.mCall)
{
  assert (mCall);
}

template <typename Ret, typename... Args>
not_empty_function<Ret (Args...)> &
not_empty_function<Ret (Args...)>::operator= (not_empty_function &&other)
{
  mCall = other.mCall;
  assert (mCall);
  return *this;
}

template <typename Ret, typename... Args>
template <typename Func, typename, typename>
not_empty_function<Ret (Args...)>::not_empty_function (Func &&call) : mCall (std::forward<Func> (call))
{
  assert (mCall);
}

template <typename Ret, typename... Args>
template <typename Func, typename, typename>
not_empty_function<Ret (Args...)> &
not_empty_function<Ret (Args...)>::operator= (Func &&call)
{
  mCall = std::forward<Func> (call);
  assert (mCall);
  return *this;
}

template <typename Ret, typename... Args> not_empty_function<Ret (Args...)>::operator bool () const
{
  assert (mCall);
  return true;
}

template <typename Ret, typename... Args>
Ret
not_empty_function<Ret (Args...)>::operator() (Args... args) const
{
  assert (mCall);
  return mCall (std::forward<Args> (args)...);
}

template <typename Ret, typename... Args>
void
not_empty_function<Ret (Args...)>::swap (not_empty_function &other)
{
  std::swap (mCall, other.mCall);
}

template <typename Ret, typename... Args>
inline bool
operator== (const not_empty_function<Ret (Args...)> &func, std::nullptr_t) noexcept
{
  return !static_cast<bool> (func);
}

template <typename Ret, typename... Args>
inline bool
operator== (std::nullptr_t, const not_empty_function<Ret (Args...)> &func) noexcept
{
  return !static_cast<bool> (func);
}

template <typename Ret, typename... Args>
inline bool
operator!= (const not_empty_function<Ret (Args...)> &func, std::nullptr_t) noexcept
{
  return static_cast<bool> (func);
}

template <typename Ret, typename... Args>
inline bool
operator!= (std::nullptr_t, const not_empty_function<Ret (Args...)> &func) noexcept
{
  return static_cast<bool> (func);
}

namespace swap_ns
{

using std::swap;
template <typename Ret, typename... Args>
inline void
swap (not_empty_function<Ret (Args...)> &l, not_empty_function<Ret (Args...)> &r)
{
  l.swap (r);
}

} // namespace swap_ns
} // namespace ak
