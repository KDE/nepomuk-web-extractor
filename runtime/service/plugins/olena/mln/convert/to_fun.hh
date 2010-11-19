// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_TO_FUN_HH
# define MLN_CONVERT_TO_FUN_HH

/// \file
///
/// Conversions towards some mln::Function.

# include <mln/pw/value.hh>
# include <mln/fun/c.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a C unary function into an mln::fun::C.
    template <typename R, typename A>
    fun::C<R(*)(A)> to_fun(R (*f)(A));

    /// Convert an image into a function.
    template <typename I>
    pw::value_<I> to_fun(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename R, typename A>
    inline
    fun::C<R(*)(A)> to_fun(R (*f_)(A))
    {
      fun::C<R(*)(A)> f(f_);
      return f;
    }

    template <typename I>
    inline
    pw::value_<I> to_fun(const Image<I>& ima)
    {
      return pw::value(ima);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_FUN_HH
