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

#ifndef MLN_MAKE_DETACHMENT_HH
# define MLN_MAKE_DETACHMENT_HH

/// \file
/// \brief Compute the detachment of a cell w.r.t. from a binary
/// complex-based image.

# include <mln/core/image/complex_image.hh>
# include <mln/make/cell.hh>
# include <mln/topo/is_facet.hh>

namespace mln
{

  namespace make
  {

    /** \brief Compute the detachment of the cell corresponding to the
	facet \a f to the image \a ima.

	\pre \a f is a facet (it does not belong to any face of higher
	     dimension).
	\pre \a ima is an image of Boolean values.

	\return a set of faces containing the detachment.

	We do not use the fomal definition of the detachment here (see
	couprie.08.pami).  We use the following (equivalent) definition:
	an N-face F in CELL is not in the detachment of CELL from IMA if
	it is adjacent to at least an (N-1)-face or an (N+1)-face that
	does not belong to CELL.  */
    template <unsigned D, typename G, typename V>
    p_set< complex_psite<D, G> >
    detachment(const complex_psite<D, G>& f,
	       const complex_image<D, G, V>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned D, typename G, typename V>
    inline
    p_set< complex_psite<D, G> >
    detachment(const complex_psite<D, G>& f,
	       const complex_image<D, G, V>& ima)
    {
      mln_precondition(topo::is_facet(f));
      mlc_equal(V, bool)::check();

      typedef complex_psite<D, G> psite;
      typedef p_set<psite> faces_t;

      faces_t f_hat = make::cell(f);
      // Initialize DETACH_F to F_HAT.
      faces_t detach_f = f_hat;

      typedef complex_lower_higher_neighborhood<D, G> adj_nbh_t;
      adj_nbh_t adj_nbh;
      mln_piter(faces_t) g(f_hat);
      mln_niter(adj_nbh_t) n(adj_nbh, g);
      for_all(g)
	for_all(n)
	if (ima(n) && !f_hat.has(n))
	  {
	    detach_f.remove(g);
	    break;
	  }
      return detach_f;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_DETACHMENT_HH
