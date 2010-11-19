// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_GEOM_NROWS_HH
# define MLN_GEOM_NROWS_HH

/// \file
///
/// Give the number of rows of an image.

# include <mln/geom/min_row.hh>
# include <mln/geom/max_row.hh>


namespace mln
{

  namespace geom
  {

    /// Give the number of rows of an image.
    template <typename I>
    unsigned nrows(const Image<I>& ima);

    /// Give the number of rows of a box 2d or 3d.
    template <typename B>
    unsigned nrows(const Box<B>& b);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    unsigned nrows(const Image<I>& ima)
    {
      mln_precondition(exact(ima).is_valid());
      unsigned nrows = geom::max_row(ima) - geom::min_row(ima) + 1;

      return nrows;
    }

    template <typename B>
    unsigned nrows(const Box<B>& b)
    {
      metal::not_<metal::equal<metal::int_<B::dim>, metal::int_<1> > >::check();
      unsigned nrows = geom::max_row(b) - geom::min_row(b) + 1;

      return nrows;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_NROWS_HH
