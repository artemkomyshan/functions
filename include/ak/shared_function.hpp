// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "ak/requires.hpp"

/**
 * shared_function is a polymorphic function wrapper
 * that retains shared ownership of a function through a shared pointer.
 * Several shared_function objects may own the same object.
 */

namespace ak
{

template <typename Signature>
class shared_function;

template <typename Ret, typename... Args>
class shared_function<Ret(Args...)>
{
public:
  explicit operator bool() const;

  Ret operator()(Args... args) const;

  std::function<Ret(Args...)> const& get() const;
  std::function<Ret(Args...)>& get();

  shared_function() = default;
  shared_function(std::nullptr_t);
  shared_function& operator=(std::nullptr_t);

  shared_function(shared_function&& other) = default;
  shared_function& operator=(shared_function&& other) = default;

  shared_function(const shared_function& other) = default;
  shared_function& operator=(const shared_function& other) = default;

  template <
      typename OFunc1,
      typename = Requires<
          Not<std::is_same<typename std::decay_t<OFunc1>, shared_function>>>,
      typename = Requires<Not<std::is_convertible<OFunc1, shared_function>>>>
  shared_function(OFunc1&& call);

  template <
      typename OFunc1,
      typename = Requires<
          Not<std::is_same<typename std::decay_t<OFunc1>, shared_function>>>,
      typename = Requires<Not<std::is_convertible<OFunc1, shared_function>>>>
  shared_function& operator=(OFunc1&& call);

  void swap(shared_function& other);

private:
  std::shared_ptr<std::function<Ret(Args...)>> mCall;
};

template <typename Ret, typename... Args>
shared_function<Ret(Args...)>::shared_function(std::nullptr_t)
    : shared_function()
{
}

template <typename Ret, typename... Args>
auto shared_function<Ret(Args...)>::operator=(std::nullptr_t)
    -> shared_function&
{
  mCall = nullptr;
  return *this;
}

template <typename Ret, typename... Args>
template <typename OFunc1, typename, typename>
shared_function<Ret(Args...)>::shared_function(OFunc1&& call)
    : mCall(std::make_shared<std::function<Ret(Args...)>>(
          std::forward<OFunc1>(call)))
{
}

template <typename Ret, typename... Args>
template <typename OFunc1, typename, typename>
shared_function<Ret(Args...)>&
shared_function<Ret(Args...)>::operator=(OFunc1&& call)
{
  mCall =
      std::make_shared<std::function<Ret(Args...)>>(std::forward<OFunc1>(call));
  return *this;
}

template <typename Ret, typename... Args>
shared_function<Ret(Args...)>::operator bool() const
{
  return mCall && mCall.get();
}

template <typename Ret, typename... Args>
Ret shared_function<Ret(Args...)>::operator()(Args... args) const
{
  if (!mCall)
    throw std::bad_function_call();

  return mCall->operator()(std::forward<Args>(args)...);
}

template <typename Ret, typename... Args>
void shared_function<Ret(Args...)>::swap(shared_function& other)
{
  std::swap(mCall, other.mCall);
}

template <typename Ret, typename... Args>
const std::function<Ret(Args...)>& shared_function<Ret(Args...)>::get() const
{
  return *mCall;
}

template <typename Ret, typename... Args>
std::function<Ret(Args...)>& shared_function<Ret(Args...)>::get()
{
  return *mCall;
}

template <typename Ret, typename... Args>
inline bool operator==(const shared_function<Ret(Args...)>& func,
                       std::nullptr_t) noexcept
{
  return !static_cast<bool>(func);
}

template <typename Ret, typename... Args>
inline bool operator==(std::nullptr_t,
                       const shared_function<Ret(Args...)>& func) noexcept
{
  return !static_cast<bool>(func);
}

template <typename Ret, typename... Args>
inline bool operator!=(const shared_function<Ret(Args...)>& func,
                       std::nullptr_t) noexcept
{
  return static_cast<bool>(func);
}

template <typename Ret, typename... Args>
inline bool operator!=(std::nullptr_t,
                       const shared_function<Ret(Args...)>& func) noexcept
{
  return static_cast<bool>(func);
}

namespace swap_ns
{

using std::swap;
template <typename Ret, typename... Args>
inline void swap(shared_function<Ret(Args...)>& l,
                 shared_function<Ret(Args...)>& r)
{
  l.swap(r);
}

} // namespace swap_ns
} // namespace ak