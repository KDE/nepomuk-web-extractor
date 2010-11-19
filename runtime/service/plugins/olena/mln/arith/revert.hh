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

#ifndef MLN_ARITH_REVERT_HH
# define MLN_ARITH_REVERT_HH

/// \file
///
/// Point-wise revert (min -> max and max -> min) of images.
///
/// \todo Add static assertion and save one iterator in in-place version.

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>

// Specializations are in:
# include <mln/arith/revert.spe.hh>


// FIXME: Rely instead on mln/fun/v2v/revert.hh.
// FIXME: Revert on int value 0 does not give 0 (since min != - max; idem for float etc.)


namespace mln
{

  namespace arith
  {

    /// Point-wise reversion of image \p input.
    /*!
     * \param[in] input the input image.
     * \result The result image.
     *
     * \pre \p input.is_valid
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     output(p) = min + (max - input(p))
     */
    template <typename I>
    mln_concrete(I) revert(const Image<I>& input);


    /// Point-wise in-place reversion of image \p input.
    /*!
     * \param[in,out] input The target image.
     *
     * \pre \p input.is_valid
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     input(p) = min + (max - input(p))
     */
    template <typename I>
    void revert_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename O>
	inline
	void revert_(const I& input, O& output)
	{
	  trace::entering("arith::impl::generic::revert_");

	  typedef mln_value(I) V;
	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = mln_min(V) + (mln_max(V) - input(p));

	  trace::exiting("arith::impl::generic::revert_");
	}

      } // end of namespace mln::arith::impl::generic

    } // end of namespace mln::arith::impl


    // Facades.

    template <typename I>
    inline
    mln_concrete(I) revert(const Image<I>& input)
    {
      trace::entering("arith::revert");

      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output;
      initialize(output, input);
      impl::revert_(mln_trait_image_speed(I)(), exact(input), output);

      trace::exiting("arith::revert");
      return output;
    }

    template <typename I>
    inline
    void revert_inplace(Image<I>& input)
    {
      trace::entering("arith::revert_inplace");

      mln_precondition(exact(input).is_valid());

      impl::revert_(mln_trait_image_speed(I)(), exact(input), exact(input));

      trace::exiting("arith::revert_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_REVERT_HH
