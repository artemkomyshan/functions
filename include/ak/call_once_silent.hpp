// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <type_traits>
#include <utility>

#include "ak/callable_type_traits.hpp"
#include "ak/requires.hpp"

/**
 * @brief The call_once_silent class is std::function wrapper which allows this
 * function to be invoked just once. in the case when the function is called
 * more than once, all calls starting from the second will be ignored
 */

namespace ak
{

template <typename... Args>
class call_once_silent
{
  template <typename Func,
            typename Ret2 = typename std::result_of<Func&(Args...)>::type>
  struct Callable : check_func_return_type<Ret2, void>
  {
  };

public:
  template <
      typename Func,
      typename = Requires<
          Not<std::is_same<typename std::decay<Func>::type, call_once_silent>>>,
      typename = Requires<Callable<Func>>>
  call_once_silent(Func&& func) : mFunc(std::forward<Func>(func))
  {
  }

  call_once_silent() noexcept : mFunc(nullptr) {}

  call_once_silent(std::nullptr_t) noexcept : mFunc(nullptr) {}

  call_once_silent(call_once_silent const& o) = delete;
  call_once_silent& operator=(call_once_silent const& o) = delete;

  call_once_silent(call_once_silent&& o) = default;
  call_once_silent& operator=(call_once_silent&& o) = default;

  template <
      typename Func,
      typename = Requires<
          Not<std::is_same<typename std::decay<Func>::type, call_once_silent>>>,
      typename = Requires<Callable<Func>>>
  call_once_silent& operator=(Func&& func)
  {
    mFunc = std::forward<Func>(func);
    return *this;
  }

  call_once_silent& operator=(std::nullptr_t) noexcept
  {
    mFunc = nullptr;
    return *this;
  }

  void operator()(Args... args)
  {
    if (!mFunc)
      return;

    auto func = std::move(mFunc);
    mFunc = nullptr;

    func(std::forward<Args>(args)...);
  }

  explicit operator bool() const
  {
    return mFunc != nullptr;
  }

private:
  std::function<void(Args...)> mFunc;
};

// null pointer comparisons
template <typename... Args>
bool operator==(call_once_silent<Args...> const& call, std::nullptr_t) noexcept
{
  return !static_cast<bool>(call);
}

template <typename... Args>
bool operator==(std::nullptr_t, call_once_silent<Args...> const& call) noexcept
{
  return !static_cast<bool>(call);
}

template <typename... Args>
bool operator!=(call_once_silent<Args...> const& call, std::nullptr_t) noexcept
{
  return static_cast<bool>(call);
}
template <typename... Args>
bool operator!=(std::nullptr_t, call_once_silent<Args...> const& call) noexcept
{
  return static_cast<bool>(call);
}

} // namespace ak