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

#ifndef MLN_CANVAS_BROWSING_DIAGONAL2D_HH
# define MLN_CANVAS_BROWSING_DIAGONAL2D_HH

/// \file
///
/// Diagonal2d browsing of an image.

# include <mln/core/concept/browsing.hh>
# include <mln/core/concept/image.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Browsing in a certain direction.
      /*!
       * This canvas browse all the point of an image 'input' of type
       * 'I' and of dimension 'dim' in the direction 'dir'.
       *
       * The functor should provide (In addition to 'input', 'I',
       * 'dim' and 'dir') three methods :
       *
       *   - init() : Will be called at the beginning.
       *   - next() : Will be called at each point 'p' (also provided by
       *   the fonctor).
       *   - final(): Will be called at the end.
       *
       * F shall features : \n
       * { \n
       * --- as types: \n
       *   I; \n
       * --- as attributes: \n
       *   dim; \n
       *   dir; // and test dir < dim \n
       *   input; \n
       *   p; \n
       * --- as methods: \n
       *   void init(); \n
       *   void next(); \n
       *   void final(); \n
       * } \n
       *
       * Example : \n
       *
       * |  1 3 6
       * |  2 5 8
       * |  4 7 9
       * L------>
       *
       */
      struct diagonal2d_t : public Browsing< diagonal2d_t >
      {
	template <typename F>
	void operator()(F& f) const;
      };

      extern const diagonal2d_t diagonal2d;

# ifndef MLN_INCLUDE_ONLY

      const diagonal2d_t diagonal2d;

      template <typename F>
      inline
      void
      diagonal2d_t::operator()(F& f) const
      {
	trace::entering("canvas::browsing::diagonal2d");

	typedef typename F::I I;
	typedef mln_deduce(I, psite, delta) dpsite;
	typedef mln_psite(I) psite;

	// Directions
	dpsite dp_first(1, 0);
	dpsite dp_second(0, 1);
	dpsite dp_diag(-1, 1);

	mln_deduce(I, psite, delta) diag;

	psite
	  pmin = f.input.domain().pmin(),
	  pmax = f.input.domain().pmax();

	f.p = pmin;

	f.init();

	while (f.input.domain().has(f.p))
	{
	  psite start_diag = f.p;
	  // Browse one diag.
	  f.init_diag();
	  while (f.input.domain().has(f.p))
	  {
	    f.next();
	    f.p = f.p + dp_diag;
	  }

	  // Goto next diag start.
	  f.p = start_diag + dp_first;
	}
	f.p = f.p - dp_first;

	f.p = f.p + dp_second;
	while (f.input.domain().has(f.p))
	{
	  psite start_diag = f.p;

	  // Browse one diag.
	  f.init_diag();
	  while (f.input.domain().has(f.p))
	  {
	    f.next();
	    f.p = f.p + dp_diag;
	  }
	  f.p = f.p - dp_diag;

	  // Goto next diag start.
	  f.p = start_diag + dp_second;
	}

	f.final();
	trace::exiting("canvas::browsing::diagonal2d");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln

#endif // ! MLN_CANVAS_BROWSING_DIAGONAL2D_HH
