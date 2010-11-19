// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_BINARY_HH
# define MLN_FUN_BINARY_HH

# include <mln/core/concept/meta_function.hh>
# include <mln/fun/spe/binary.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/functions.hh>

namespace mln
{

  namespace fun
  {

    template <typename F, typename E = F>
    struct binary : mln::Meta_Function_vv2v< E >
    {
      typedef F flag;
      typedef mln_trait_fun_storage(flag) storage;

      template <typename T1, typename T2>
      struct with
      {
	typedef spe::binary<F, T1, T2> ret;
      };

      template <typename T1, typename T2>
      typename with<T1, T2>::ret::result operator()(const T1& a, const T2& b) const
      {
	return typename with<T1, T2>::ret(state())(a, b);
      }

      template <typename U>
      void init(const U& value)
      {
	state_ = mln::trait::function::internal::introspect::has_storage_t<flag, void>::compute(value);
      };

      binary()
      {
      }

      template <typename U>
      binary(const U& param)
      {
	this->init(param);
      }

      stored<storage>& state()
      {
	return state_;
      }

      const stored<storage>& state() const
      {
	return state_;
      }

    protected:
      stored<storage> state_;
    };

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_BINARY_HH
