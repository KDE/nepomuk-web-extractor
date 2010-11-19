// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_REGROUP_FROM_SINGLE_LEFT_LINK_WRT_H_RATIO_HH
# define SCRIBO_PRIMITIVE_REGROUP_FROM_SINGLE_LEFT_LINK_WRT_H_RATIO_HH

/// \file
///
/// \brief Regroup groups with their left neighbor w.r.t. their height
/// ratio.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/accu/center.hh>

# include <mln/labeling/compute.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>

# include <scribo/filter/object_links_bbox_h_ratio.hh>
# include <scribo/primitive/link/with_single_left_link.hh>

# include <scribo/primitive/link/compute.hh>
# include <scribo/primitive/group/apply.hh>
# include <scribo/primitive/group/from_single_link.hh>


namespace scribo
{

  namespace primitive
  {

    namespace regroup
    {

      /// \brief Regroup groups with their left neighbor w.r.t. their
      /// height ratio.
      ///
      /// \param[in] groups Groups data.
      /// \param[in] The maximum distance allowed to seach a neighbor object.
      ///
      /// \return Object groups data.
      //
      template <typename L>
      inline
      object_groups<L>
      from_single_left_link_wrt_h_ratio(const object_groups<L>& groups,
					unsigned neighb_max_distance,
					double hratio);


      /// \overload
      /// Max distance is set to mln_max(unsigned).
      /// hratio is set to 1.60f.
      template <typename L>
      inline
      object_groups<L>
      from_single_left_link_wrt_h_ratio(const object_groups<L>& groups);


# ifndef MLN_INCLUDE_ONLY


      // Facades

      template <typename L>
      inline
      object_groups<L>
      from_single_left_link_wrt_h_ratio(const object_groups<L>& groups,
					unsigned neighb_max_distance,
					double hratio)
      {
	trace::entering("scribo::primitive::regroup::from_single_left_link_wrt_h_ratio");

	mln_precondition(groups.is_valid());


	object_groups<L> output = groups.duplicate();

	fun::i2v::array<mln_value(L)> relabel_fun;
	component_set<L>
	  components = primitive::group::apply(groups, relabel_fun);

	object_links<L>
	  links = primitive::link::with_single_left_link(components,
							 neighb_max_distance);

	links = filter::object_links_bbox_h_ratio(links, hratio);


	object_groups<L>
	  new_groups = primitive::group::from_single_link(links);

	for_all_groups(g, groups)
	  output(g) = new_groups(relabel_fun(g));


	trace::exiting("scribo::primitive::regroup::from_single_left_link_wrt_h_ratio");
	return output;
      }


      template <typename L>
      inline
      object_groups<L>
      from_single_left_link_wrt_h_ratio(const object_groups<L>& groups)
      {
	return from_single_left_link_wrt_h_ratio(groups,
						 mln_max(unsigned), 1.60f);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::regroup

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_REGROUP_FROM_SINGLE_LEFT_LINK_WRT_H_RATIO_HH
