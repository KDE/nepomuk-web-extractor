// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_MIXED_NEIGHBORHOOD_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_MIXED_NEIGHBORHOOD_HH

/// \file
///
/// Definition of the elementary ``neighborhood'' on a graph.

# include <mln/core/neighb.hh>
# include <mln/core/image/graph_elt_mixed_window.hh>


namespace mln
{

  /// Elementary neighborhood on graph class.
  ///
  /// \tparam G is a graph type.
  /// \tparam S is a site set type.
  /// \tparam S2 is the site set type of the neighbors.
  //
  template <typename G, typename S, typename S2>
  struct graph_elt_mixed_neighborhood
    : public neighb< graph_elt_mixed_window<G,S,S2> >
  {
      typedef neighb< graph_elt_mixed_window<G,S,S2> > super_;

      graph_elt_mixed_neighborhood();
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename G, typename S, typename S2>
  inline
  graph_elt_mixed_neighborhood<G,S,S2>::graph_elt_mixed_neighborhood()
  {
  }


# endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_MIXED_NEIGHBORHOOD_HH
