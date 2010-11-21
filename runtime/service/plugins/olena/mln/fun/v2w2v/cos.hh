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

#ifndef MLN_FUN_V2W2V_COS_HH
# define MLN_FUN_V2W2V_COS_HH

/*! \file
 *
 *  \brief Cos functor.
 *
 *  \see mln/math/.
 */

# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>

# include <mln/math/cos.hh>
# include <mln/math/acos.hh>


namespace mln
{

  namespace fun
  {

    namespace v2w2v
    {

      /*! \brief Cosinus bijective functor.
       *
       *  \c V is the type of input values and the result type.
       *
       *  \see mln::math::cos.
       */
      template <typename V>
      struct cos : public Function_v2v< cos<V> >
      {
	typedef V result;
	V operator()(const V& v) const;
	V f_1       (const V& v) const;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      V
      cos<V>::operator()(const V& v) const
      {
	return mln::math::cos (v);
      }

      template <typename V>
      inline
      V
      cos<V>::f_1(const V& v) const
      {
	return mln::math::acos (v);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2w2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2W2V_COS_HH