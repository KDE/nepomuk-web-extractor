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

#ifndef MLN_MAKE_PIX_HH
# define MLN_MAKE_PIX_HH

/*! \file
 *
 * \brief Routine to construct an mln::util::pix.
 */

# include <mln/util/pix.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::util::pix from an image \p ima and a psite \p p.
     *
     * \param[in] ima The input image.
     * \param[in] p   The point site.
     *
     * \return An mln::util::pix.
     */
    template <typename I>
    mln::util::pix<I> pix(const Image<I>& ima, const mln_psite(I)& p);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln::util::pix<I> pix(const Image<I>& ima, const mln_psite(I)& p)
    {
      mln::util::pix<I> tmp(ima, p);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_PIX_HH
