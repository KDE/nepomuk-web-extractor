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

#ifndef MLN_FUN_V2V_LINEAR_HH
# define MLN_FUN_V2V_LINEAR_HH

/// \file
///
/// FIXME.

# include <mln/core/concept/function.hh>
# include <mln/convert/to.hh>


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /*!
       *\brief Linear function.
       * f(v) = a * v + b.
       * \c V is the type of input values; \c T is the type used to
       * compute the result; \c R is the result type.
       *
       * By defaut, \c T is \c V and \c R is \c T.
       */
      template <typename V, typename T = V, typename R = T>
      struct linear : public Function_v2v< linear<V,T,R> >
      {
	typedef R result;

	R operator()(const V& v) const;

	template <typename U> 
	R operator()(const U& u) const;

	linear(T a, T b);
	T a, b;
      };


      template <typename V, typename T = V, typename R = T>
      struct linear_sat : public Function_v2v< linear_sat<V,T,R> >
      {
	typedef R result;

	R operator()(const V& v) const;

	template <typename U> 
	R operator()(const U& u) const;

	linear_sat(T a, T b);
	T a, b;
      };


# ifndef MLN_INCLUDE_ONLY


      // linear.

      template <typename V, typename T, typename R>
      inline
      linear<V,T,R>::linear(T a, T b)
	: a(a),
	  b(b)
      {
      }

      template <typename V, typename T, typename R>
      inline
      R
      linear<V,T,R>::operator()(const V& v) const
      {
	return mln::convert::to<R>(a * static_cast<T>(v) + b);
      }

      template <typename V, typename T, typename R>
      template <typename U>
      inline
      R
      linear<V,T,R>::operator()(const U& u) const
      {
	return this->operator()(static_cast<V>(u));
      }


      // linear_sat.

      template <typename V, typename T, typename R>
      inline
      linear_sat<V,T,R>::linear_sat(T a, T b)
	: a(a),
	  b(b)
      {
      }

      template <typename V, typename T, typename R>
      inline
      R
      linear_sat<V,T,R>::operator()(const V& v) const
      {
	T res = a * static_cast<T>(v) + b;
	if (res > mln_max(R))
	  res = mln_max(R);
	else if (res < mln_min(R))
	  res = mln_min(R);
	return mln::convert::to<R>(res);
      }

      template <typename V, typename T, typename R>
      template <typename U>
      inline
      R
      linear_sat<V,T,R>::operator()(const U& u) const
      {
	mlc_converts_to(U, V)::check();
	return this->operator()(static_cast<V>(u));
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_LINEAR_HH
