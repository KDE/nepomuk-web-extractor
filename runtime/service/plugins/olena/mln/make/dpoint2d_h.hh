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

#ifndef MLN_MAKE_DPOINT2D_H_HH
# define MLN_MAKE_DPOINT2D_H_HH

/// \file
///
/// Routine to construct an mln::dpoint2d_h.

# include <mln/core/alias/dpoint2d_h.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an mln::dpoint2d_h.
     *
     * \param[in] row Row coordinate.
     * \param[in] col Column coordinate.
     *
     * \return A 2D dpoint.
     */
    mln::dpoint2d_h dpoint2d_h(def::coord row, def::coord col);


# ifndef MLN_INCLUDE_ONLY

    inline
    mln::dpoint2d_h dpoint2d_h(def::coord row, def::coord col)
    {
      mln::dpoint2d_h tmp;
      tmp[0] = row;
      tmp[1] = col;
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_DPOINT2D_H_HH
