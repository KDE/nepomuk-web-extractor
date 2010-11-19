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

#ifndef SCRIBO_TABLE_CONNECT_VERTICAL_LINES_HH
# define SCRIBO_TABLE_CONNECT_VERTICAL_LINES_HH

/// \file
///
/// Connect vertical lines with aligned rows.
///
/// \todo do not modify arguments but return a new value.

# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/table/internal/connect_lines.hh>


namespace scribo
{

  namespace table
  {


    /// Connect vertical lines with the new aligned rows.
    ///
    /// \param[in]	aligned_rows  a list of new aligned rows.
    /// \param[in,out]	tablebboxes   the vertical and horizontal lines
    ///				      bounding boxes.
    /// \param[in]	input	      The image from where the lines are
    ///				      extracted.
    /// \param[in]	max_distance  max distance allowed between a vertical
    ///				      and horizontal lines.
    template <typename I>
    void
    connect_vertical_lines(const mln::util::array<int>& aligned_rows,
			   mln::util::couple<mln::util::array<box<mln_site(I)> >,
					mln::util::array<box<mln_site(I)> > >& tablebboxes,
			   const Image<I>& input,
			   unsigned max_distance);


# ifndef MLN_INCLUDE_ONLY


    /// Connect vertical lines with the new aligned rows.
    template <typename I>
    void
    connect_vertical_lines(const mln::util::array<int>& aligned_rows,
			   mln::util::couple<mln::util::array<box<mln_site(I)> >,
					mln::util::array<box<mln_site(I)> > >& tablebboxes,
			   const Image<I>& input,
			   unsigned max_distance)
    {
      trace::entering("scribo::table::connect_vertical_lines");
      mln_precondition(exact(input).is_valid());

      internal::connect_lines(aligned_rows, tablebboxes.first(),
			      0, exact(input).nrows(), max_distance);

      trace::exiting("scribo::table::connect_vertical_lines");
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_CONNECT_VERTICAL_LINES_HH
