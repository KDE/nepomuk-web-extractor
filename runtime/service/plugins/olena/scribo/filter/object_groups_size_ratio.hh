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

#ifndef SCRIBO_FILTER_OBJECT_GROUPS_SIZE_RATIO_HH
# define SCRIBO_FILTER_OBJECT_GROUPS_SIZE_RATIO_HH

/// \file
///


# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_groups.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    template <typename L>
    object_groups<L>
    object_groups_size_ratio(const object_groups<L>& groups,
			     float max_size_ratio,
			     float max_invalid_ratio_per_group);

# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_groups<L>
    object_groups_size_ratio(const object_groups<L>& groups,
			     float max_size_ratio,
			     float max_invalid_ratio_per_group)
    {
      trace::entering("scribo::filter::object_groups_size_ratio");

      mln_precondition(groups.is_valid());

      const component_set<L>& comps = groups.components();

      // FIXME: estimating the group size should be removed once
      // available in the object_group structure.
      // Counting the number of objects per group with a size ratio >
      // max_ratio.
      mln::util::array<unsigned>
	group_size(groups.size(), 0),
	invalid_object_in_group(groups.size(), 0);

      for_all_comps(i, comps)
      {
	if ((comps(i).bbox().nrows() / comps(i).bbox().ncols())
	    >= max_size_ratio)
	  ++invalid_object_in_group[groups[i]];

	++group_size[groups[i]];
      }

      object_groups<L> output(groups);
      output(0) = 0;
      for (unsigned i = 1; i < output.size(); ++i)
	if ((invalid_object_in_group[groups[i]] / static_cast<float>(group_size[groups[i]])) >= max_invalid_ratio_per_group
	    || !comps(i).is_valid())
	  output(i) = 0;

      trace::exiting("scribo::filter::object_groups_size_ratio");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_GROUPS_SIZE_RATIO_HH
