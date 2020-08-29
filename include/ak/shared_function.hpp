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

template <typename Func>
class shared_function
{
public:
  explicit operator bool() const;

  template <typename... Args>
  std::result_of_t<Func(Args...)> operator()(Args... args) const;

  Func const& get() const;
  Func& get();

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
  std::shared_ptr<Func> mCall;
};

template <typename Func>
shared_function<Func> make_shared_function(Func f)
{
  return shared_function<Func>(std::move(f));
}

template <typename Func>
shared_function<Func>::shared_function(std::nullptr_t) : shared_function()
{
}

template <typename Func>
auto shared_function<Func>::operator=(std::nullptr_t) -> shared_function&
{
  mCall = nullptr;
  return *this;
}

template <typename Func>
template <typename OFunc1, typename, typename>
shared_function<Func>::shared_function(OFunc1&& call)
    : mCall(std::make_shared<Func>(std::forward<OFunc1>(call)))
{
}

template <typename Func>
template <typename OFunc1, typename, typename>
shared_function<Func>& shared_function<Func>::operator=(OFunc1&& call)
{
  mCall = std::make_shared<Func>(std::forward<OFunc1>(call));
  return *this;
}

template <typename Func>
shared_function<Func>::operator bool() const
{
  return mCall && mCall.get();
}

template <typename Func>
template <typename... Args>
std::result_of_t<Func(Args...)>
shared_function<Func>::operator()(Args... args) const
{
  if (!mCall)
    throw std::bad_function_call();

  return mCall->operator()(std::forward<Args>(args)...);
}

template <typename Func>
void shared_function<Func>::swap(shared_function& other)
{
  std::swap(mCall, other.mCall);
}

template <typename Func>
const Func& shared_function<Func>::get() const
{
  return *mCall;
}

template <typename Func>
Func& shared_function<Func>::get()
{
  return *mCall;
}

template <typename Func>
inline bool operator==(const shared_function<Func>& func,
                       std::nullptr_t) noexcept
{
  return !static_cast<bool>(func);
}

template <typename Func>
inline bool operator==(std::nullptr_t,
                       const shared_function<Func>& func) noexcept
{
  return !static_cast<bool>(func);
}

template <typename Func>
inline bool operator!=(const shared_function<Func>& func,
                       std::nullptr_t) noexcept
{
  return static_cast<bool>(func);
}

template <typename Func>
inline bool operator!=(std::nullptr_t,
                       const shared_function<Func>& func) noexcept
{
  return static_cast<bool>(func);
}

namespace swap_ns
{

using std::swap;
template <typename Func>
inline void swap(shared_function<Func>& l, shared_function<Func>& r)
{
  l.swap(r);
}

} // namespace swap_ns
} // namespace ak