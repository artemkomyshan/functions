// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <memory>

/**
 * @brief The call_on_expire struct wraps function into shared_ptr and invokes
 * this function before deletion. In other words, the bound function is invoked
 * when the last remaining call_on_expire referred to the function is destroyed.
 */

namespace ak
{

class call_on_expire
{
  using tCall = std::function<void (void)>;

public:
  template <typename Func>
  call_on_expire (Func call)
      : mCall (new tCall (std::move (call)), [] (tCall *call) {
          (*call) ();
          delete call;
        })
  {
  }

private:
  std::shared_ptr<tCall> mCall;
};

} // namespace ak