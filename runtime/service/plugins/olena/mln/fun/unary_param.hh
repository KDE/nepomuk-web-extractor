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

#include "unary.hh"

#ifndef MLN_FUN_UNARY_PARAM_HH
# define MLN_FUN_UNARY_PARAM_HH

# include <mln/fun/unary.hh>
# include <mln/fun/param.hh>

namespace mln
{

  namespace fun
  {

    template <typename F, typename Param, typename Storage = void, typename E = F>
    struct unary_param: unary< unary_param<F,Param,Storage,E>, E>
    {
      typedef F flag;
      typedef Param param;
      typedef Storage storage;

      unary_param()
      {
      }

      template <typename U>
      unary_param(const U& param)
      {
	this->init(param);
      }

    };

    template <typename F, typename Param, typename E>
    struct parameter< unary_param<F,Param,void,E> >
    {
      typedef Param param;
      typedef void storage;
    };

    template <typename F, typename Param, typename Storage, typename E>
    struct parameter< unary_param<F,Param,Storage,E> >
    {
      typedef Param param;
      typedef Storage storage;

      template <typename U>
      storage compute(const U& u)
      {
	return F::compute_param(u);
      }
    };

  }

  namespace trait
  {

    namespace next
    {

      template <typename F, typename Param, typename Storage, typename E, typename T>
      struct set_precise_unary_<mln::fun::unary_param<F,Param,Storage,E>, T>
      {
	typedef mln_trait_nunary(F, T) ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait


} // end of namespace mln

#endif // ! MLN_FUN_UNARY_PARAM_HH
