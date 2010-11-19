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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_LEFT_LINKS_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_LEFT_LINKS_HH

/// \file
///
/// Link text bounding boxes with their left neighbor.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/util/array.hh>

# include <scribo/primitive/link/with_single_left_link.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      /// Map each character bounding box to its left bounding box neighbor
      /// if possible.
      /// Iterate to the right but link boxes to the left.
      ///
      /// \return object links data.
      //
      template <typename L>
      inline
      object_links<L>
      with_several_left_links(const component_set<L>& objects,
			      unsigned neighb_max_distance);


      /// \overload
      template <typename L>
      inline
      object_links<L>
      with_several_left_links(const component_set<L>& comps);


# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      object_links<L>
      with_several_left_links(const component_set<L>& comps,
			      unsigned neighb_max_distance)
      {
	trace::entering("scribo::primitive::link::with_several_left_links");

	  //	  -------
	  //  <------X  |
	  //	  |     |
	  //	  |	|
	  //  <------X	|
	  //	  |     |
	  //	  |	|
	  //  <------X  |
	  //	  -------

	// FIXME: make it faster.
	internal::single_left_functor<L>
	  functor(comps, neighb_max_distance);
	object_links<L> link_center = compute(functor, anchor::Center);
	object_links<L> link_top    = compute(functor, anchor::Top);
	object_links<L> link_bot    = compute(functor, anchor::Bottom);


	object_links<L> final_link(comps);

	for_all_comps(i, comps)
	{
	  if (link_center(i) != i)
	    final_link(i) = link_center(i);
	  else if (link_top(i) == link_bot(i))
	    final_link(i) = link_top(i);
	  else if (link_top(i) != i && link_bot(i) == i)
	    final_link(i) = link_top(i);
	  else if (link_bot(i) != i && link_top(i) == i)
	    final_link(i) = link_bot(i);
	  else
	    final_link(i) = i;
	}



	trace::exiting("scribo::primitive::link::with_several_left_links");
	return final_link;
      }


      template <typename L>
      inline
      object_links<L>
      with_several_left_links(const component_set<L>& comps)
      {
	return with_several_left_links(comps);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_SEVERAL_LEFT_LINKS_HH
