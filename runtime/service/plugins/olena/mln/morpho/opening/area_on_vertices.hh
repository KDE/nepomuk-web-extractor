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

#ifndef MLN_MORPHO_OPENING_AREA_ON_VERTICES_HH
# define MLN_MORPHO_OPENING_AREA_ON_VERTICES_HH

/// \file
///
/// Morphological area opening on a line graph image computing
/// the area in terms of adjacent vertices.

# include <mln/core/image/edge_image.hh>
# include <mln/morpho/opening/algebraic.hh>
# include <mln/morpho/attribute/count_adjacent_vertices.hh>


namespace mln
{

  namespace morpho
  {

    namespace opening
    {

      /// Morphological area opening on a mln::line_graph_image computing
      /// the area in terms of adjacent vertices.
      ///
      /// \param[in] input An edge image.
      /// \param[in] nbh A graph neighborhood.
      /// \param[in] lambda Closing parameter.
      ///
      /// \return An edge image.
      //
      template <typename P, typename V, typename G, typename N>
      edge_image<P,V,G>
      area_on_vertices(const edge_image<P,V,G>& input,
		       const Neighborhood<N>& nbh,
		       unsigned lambda);


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename V, typename G, typename N>
    inline
    edge_image<P,V,G>
    area_on_vertices(const edge_image<P,V,G>& input,
		     const Neighborhood<N>& nbh,
		     unsigned lambda)
    {
      trace::entering("morpho::opening::area_on_vertices");
      mln_precondition(exact(input).is_valid());

      typedef attribute::count_adjacent_vertices< edge_image<P,V,G> > attribute_t;

      edge_image<P,V,G>
	output = opening::algebraic(input, nbh, attribute_t(), lambda);

      trace::exiting("morpho::opening::area_on_vertices");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY
    } // end of namespace mln::morpho::opening

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_AREA_ON_VERTICES_HH
