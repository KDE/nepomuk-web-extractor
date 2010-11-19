// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_SKELETON_CONSTRAINED_HH
# define MLN_MORPHO_SKELETON_CONSTRAINED_HH

/// \file
///
/// Compute a skeleton under constraints.
///
/// \todo Add an extension handling policy for the user to set it.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>
# include <mln/extension/adjust_duplicate.hh>
# include <mln/data/fill.hh>


namespace mln
{

  namespace morpho
  {

    template <typename I,
	      typename N, typename F,
	      typename K, typename R>
    mln_ch_value(I, bool)
    skeleton_constrained(const Image<I>& input,
			 const Neighborhood<N>& nbh, const F& is_simple,
			 const Image<K>& constraint, const Image<R>& priority);



# ifndef MLN_INCLUDE_ONLY

    template <typename I,
	      typename N, typename F,
	      typename K, typename R>
    inline
    mln_ch_value(I, bool)
    skeleton_constrained(const Image<I>& input_,
			 const Neighborhood<N>& nbh_, const F& is_simple,
			 const Image<K>& constraint_, const Image<R>& priority_)
    {
      trace::entering("morpho::skeleton_constrained");

      const I& input      = exact(input_);
      const N& nbh        = exact(nbh_);
      const K& constraint = exact(constraint_);
      const R& priority   = exact(priority_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());
      mln_precondition(constraint.is_valid());
      mln_precondition(priority.is_valid());

      extension::adjust_duplicate(input, nbh);

      // FIXME: Tests!

      typedef mln_psite(I) P;
      typedef p_queue_fast<P> Q;
      p_priority<mln_value(R), Q> q;

      mln_ch_value(I, bool) output;

      // Initialization.
      {
	initialize(output, input);
	data::fill(output, input);
	extension::adjust_duplicate(output, nbh);

	mln_piter(I) p(input.domain());
	for_all(p)
	  if (input(p) == false &&
	      is_simple(input, nbh, p)) // p is a simple point of the background.
	    {
	      q.push(priority(p), p);
	    }
      }

      // Propagation.
      {
	P p;
	mln_niter(N) n(nbh, p);
	while (! q.is_empty())
	  {
	    p = q.pop_front();
	    for_all(n)
	      if (output.has(n) &&
		  output(n) == true &&
		  constraint(n) == false &&
		  is_simple(output, nbh, n))
		{
		  output(n) = false; // Remove n from object.
		  q.push(priority(n), n);
		}
	  }
      }

      trace::exiting("morpho::skeleton_constrained");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_SKELETON_CONSTRAINED_HH
