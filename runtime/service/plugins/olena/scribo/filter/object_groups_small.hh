// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_FILTER_OBJECT_GROUPS_SMALL_HH
# define SCRIBO_FILTER_OBJECT_GROUPS_SMALL_HH

/// \file
///
/// Remove small objects groups.


# include <mln/util/array.hh>

# include <scribo/core/object_groups.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /*!  \brief Remove objects within a group with less than \p n
      links.

      \param[in] groups Information about object groups.
      \param[in] n_links The minimum number of links per group.

      \return A copy of object group in which small groups have been
      removed.
    */
    template <typename L>
    object_groups<L>
    object_groups_small(const object_groups<L>& groups,
			unsigned n_links);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    inline
    object_groups<L>
    object_groups_small(const object_groups<L>& groups,
			unsigned n_links)
    {
      trace::entering("scribo::filter::object_groups_small");

      mln_precondition(groups.is_valid());

      // Counting the number of objects per group.
      mln::util::array<unsigned> group_size(groups.nelements(), 0);
      for_all_groups(i, group_size)
	++group_size[groups(i)];

      object_groups<L> output = groups.duplicate();
      output(0) = 0;
      for_all_groups(i, output)
	if (group_size[groups(i)] < n_links
	    || !groups.components()(i).is_valid())
	  output(i) = 0;

      trace::exiting("scribo::filter::object_groups_small");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_OBJECT_GROUPS_SMALL_HH

