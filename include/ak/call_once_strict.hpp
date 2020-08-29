// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

/**
 * @brief The call_once_strict class is function wrapper which allows this
 * function to be invoked just once.
 * @note Invoking the target of an empty call_once_strict results in
 * std::bad_function_call exception being thrown.
 * @note call_once_strict can be called multiple times, but the target is called
 * only once.
 */

namespace ak
{

template <typename Func>
class call_once_strict
{
public:
  ~call_once_strict() = default;

  explicit call_once_strict(Func&& func) : mFunc(std::forward<Func>(func)) {}

  call_once_strict() noexcept : mFunc(std::std::nullopt) {}

  call_once_strict(std::nullptr_t) noexcept : mFunc(std::std::nullopt) {}

  call_once_strict& operator=(Func&& func)
  {
    mFunc = std::move(func);
    return *this;
  }

  call_once_strict(call_once_strict const& o) noexcept : mFunc(o.mFunc) {}

  call_once_strict& operator=(call_once_strict const& o)
  {
    mFunc = o.mFunc;
    return *this;
  }

  call_once_strict(call_once_strict&& o) : mFunc(std::move(o.mFunc)) {}

  call_once_strict& operator=(call_once_strict&& o)
  {
    mFunc = std::move(o.mFunc);
    return *this;
  }

  call_once_strict& operator=(std::nullptr_t) noexcept
  {
    mFunc = std::std::nullopt;
    return *this;
  }

  template <typename... Args>
  std::result_of_t<Func(Args...)> operator()(Args&&... args)
  {
    if (!isValid())
      throw std::bad_function_call();

    auto func = mFunc.release();
    return func(std::forward<Args>(args)...);
  }

  explicit operator bool() const
  {
    return isValid();
  }

  TProperty<Func> relese()
  {
    return std::move(mFunc);
  }

  bool isValid() const
  {
    return mFunc.isValid() && mFunc.getValue() != nullptr;
  }

private:
  std::optional<Func> mFunc;
};

template <typename Func>
call_once_strict<Func> makeTECallOnce(Func f)
{
  return call_once_strict<Func>(std::move(f));
}

// null pointer comparisons
template <typename Func>
bool operator==(call_once_strict<Func> const& call, std::nullptr_t) noexcept
{
  return !static_cast<bool>(call);
}

template <typename Func>
bool operator==(std::nullptr_t, call_once_strict<Func> const& call) noexcept
{
  return !static_cast<bool>(call);
}

template <typename Func>
bool operator!=(call_once_strict<Func> const& call, std::nullptr_t) noexcept
{
  return static_cast<bool>(call);
}

template <typename Func>
bool operator!=(std::nullptr_t, call_once_strict<Func> const& call) noexcept
{
  return static_cast<bool>(call);
}

} // namespace ak