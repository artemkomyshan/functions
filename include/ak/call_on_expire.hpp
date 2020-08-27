// Copyright (C) 2020 Artem Komyshan
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <memory>

#include "requires.hpp"

/**
 * @brief The call_on_expire struct wraps function into shared_ptr and invokes
 * this function before deletion. In other words, the bound function is invoked
 * when the last remaining call_on_expire referred to the function is destroyed.
 */

namespace ak
{

class call_on_expire
{
  using call_t = std::function<void (void)>;

public:
  template <typename Func, typename = Requires<Not<std::is_same<typename std::decay<Func>::type, call_on_expire> > > >
  call_on_expire (Func &&action)
      : action_ (new call_t (std::forward<Func> (action)), [] (call_t *action) {
          (*action) ();
          delete action;
        })
  {
  }

  call_on_expire () = default;

  void
  release ()
  {
    action_.reset ();
  }

private:
  std::shared_ptr<call_t> action_;
};

} // namespace ak
