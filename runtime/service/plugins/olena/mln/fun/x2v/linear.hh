// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_X2V_LINEAR_HH
# define MLN_FUN_X2V_LINEAR_HH

/*! \file
 *
 * \brief Define a linear interpolation of values from an underlying image
 */

# include <mln/core/image/image1d.hh>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/selector.hh>
# include <mln/convert/to.hh>
# include <mln/algebra/vec.hh>

namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      /*! \brief Represent a linear interolation of values from an underlying image
       *
       */
      template < typename I >
      struct linear
        : public fun::internal::selector_<const algebra::vec<1,float>,
                                          // float is a dummy parameter (real is C)
                                          mln_value(I), linear<I> >::ret
      {
        typedef mln_value(I) result;

        /// Constructor with the underlying image
        linear(const I& ima);

        /// Return the interpolated value in the underlying image
        /// at the given 'point' v.
        template <typename C>
        mln_value(I)
        operator()(const algebra::vec<1,C>& v) const;

        /// Underlying image
        const I& ima;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      linear<I>::linear(const I& ima) : ima(ima)
      {
        mlc_bool(I::psite::dim == 1)::check();
      }

      template <typename I>
      template <typename C>
      mln_value(I)
      linear<I>::operator()(const algebra::vec<1,C>& v) const
      {
        typedef mln_sum(mln_value(I)) vsum;

        // looking for img(x);
        double x = v[0];

        // p1
        double xa = mln_point(I)::coord(v[0]);
        vsum ya = ima(point1d(xa));

        // x makes sens in img
        if (x == xa)
          return ima(xa);

        // p2
        double xb = mln_point(I)::coord(v[0] + 1);
        vsum yb = ima(point1d(xb));

        // Taylor-young
        return convert::to<mln_value(I)>
          (ya + (x - xa) * (yb - ya) / (xb - xa));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_LINEAR_HH
