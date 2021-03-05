// Copyright (C) 2021 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <memory>

/**
 * @brief The callback_guardian
 * Inheriting from this class or composition of it allows safe capturing "this"
 * to any callable object via make_guarded_callback.
 */

namespace ak
{

class callback_guardian
{
public:
  callback_guardian() : shared(std::make_shared<int>()) {}

  callback_guardian(callback_guardian const&) : callback_guardian() {}

  callback_guardian& operator=(callback_guardian const&)
  {
    // nothing should be copied
    return *this;
  }

  /**
   * @brief make_guarded_callback wraps provided function into other safe
   * function which checks owner availability before calling the target
   * function.
   * @param target - callback which needs to check before calling.
   * @param error_cb - message which is logged when \c target is called but
   * owner is destroyed.
   * @return guarded callback that checks owner availability before call
   * \c target.
   */
  template <typename Func>
  auto make_guarded_callback(Func target,
                             std::function<void()> error_cb = nullptr)
  {
    auto weak = std::weak_ptr<void>(shared);
    return [weak, cb = std::move(target),
            error = std::move(error_cb)](auto&&... args) {
      if (!weak.expired())
        cb(std::forward<decltype(args)>(args)...);
      else if (error)
        error();
    };
  }

private:
  std::shared_ptr<int> shared;
};

} // namespace ak
