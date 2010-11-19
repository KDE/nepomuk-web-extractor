// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DRAW_BOX_PLAIN_HH
# define MLN_DRAW_BOX_PLAIN_HH

/*! \file
 *
 * \brief Draw a plain box in an image.
 *
 * \fixme Rename as box.hh
 */

# include <mln/core/concept/image.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/data/fill.hh>
# include <mln/draw/line.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace draw
  {

    /*! Draw a plain box at value \p v in image \p ima
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] b the box to draw.
     * \param[in] v The value to assign to all drawn pixels.
     *
     * \pre \p ima has to be initialized.
     * \pre \p ima has \p beg.
     * \pre \p ima has \p end.
     *
     */
    template <typename I, typename B>
    void box_plain(Image<I>& ima,
		   const Box<B>& b,
		   const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename B>
    inline
    void box_plain(Image<I>& ima,
		   const Box<B>& b,
		   const mln_value(I)& v)
    {
      mln_precondition(exact(ima).is_valid());

      mln_psite(I) pmin = exact(b).pmin();
      mln_psite(I) pmax = exact(b).pmax();

      mln_precondition(exact(ima).has(pmin) && exact(ima).has(pmax));

      data::fill((ima | b).rw(), v);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_BOX_PLAIN_HH
