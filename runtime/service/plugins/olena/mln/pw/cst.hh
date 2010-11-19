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

#ifndef MLN_PW_CST_HH
# define MLN_PW_CST_HH

/// \file
///
/// \brief Definition of a constant function.

# include <mln/fun/internal/selector.hh>
# include <mln/value/equiv.hh>
# include <mln/value/concept/scalar.hh>


namespace mln
{

  // Forward declarations
  namespace pw
  {

    template <typename T>
    struct cst_;

    template <typename T>
    cst_<T> cst(const T& t);

  } // end of namespace mln::pw


  namespace trait
  {

    template <typename F, typename S>
    struct set_binary_< op::eq,  mln::Function_v2v, F,  mln::value::Scalar, S >
    {
      typedef mln_trait_op_eq(F, pw::cst_<mln_value_equiv(S)>) ret;
    };

  } // end of namespace mln::trait


  template <typename F, typename S>
  mln_trait_op_eq(F,S)
    operator == (const Function_v2v<F>& fun, const value::Scalar<S>& s)
  {
    return exact(fun) == pw::cst( value::equiv(s) );
  }



  namespace pw
  {

    // FIXME: Doc!

    template <typename T>
    struct cst_
      : fun::internal::selector_from_result_<T, cst_<T> >::ret
    {
      typedef T result;

      cst_();

      cst_(const T& t);

      template <typename P>
      T operator()(const P&) const;

    private:
      T t_;
    };


    // FIXME: Doc!

    template <typename T>
    cst_<T> cst(const T& t);


# ifndef MLN_INCLUDE_ONLY

    // pw::cst_<T>

    template <typename T>
    inline
    cst_<T>::cst_()
    {
    }

    template <typename T>
    inline
    cst_<T>::cst_(const T& t)
      : t_(t)
    {
    }

    template <typename T>
    template <typename P>
    inline
    T
    cst_<T>::operator()(const P&) const
    {
      return t_;
    }

    // pw::cst(t)

    template <typename T>
    inline
    cst_<T> cst(const T& t)
    {
      cst_<T> tmp(t);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::pw

} // end of namespace mln


#endif // ! MLN_PW_CST_HH
