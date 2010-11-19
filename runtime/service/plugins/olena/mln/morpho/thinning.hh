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

#ifndef MLN_MORPHO_THINNING_HH
# define MLN_MORPHO_THINNING_HH

/// \file
///
/// Morphological thinning.

# include <mln/morpho/hit_or_miss.hh>
# include <mln/morpho/thickening.hh>

#include <mln/io/pgm/save.hh>


namespace mln
{

  namespace morpho
  {


    /// Morphological thinning.
    ///
    /// This operator is THIN_B = Id - HMT_B, where B = (Bfg, Bbg).
    //
    template <typename I, typename Wfg, typename Wbg>
    mln_concrete(I)
      thinning(const Image<I>& input,
	       const Window<Wfg>& win_fg, const Window<Wbg>& win_bg);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename Wfg, typename Wbg>
      inline
      void
      thinning_tests(const Image<I>&    input_,
		     const Window<Wfg>& win_fg_,
		     const Window<Wbg>& win_bg_)
      {
	const I&   input  = exact(input_);
	const Wfg& win_fg = exact(win_fg_);
	const Wbg& win_bg = exact(win_bg_);

	// Tests.
	mln_precondition(input.is_valid());
	mln_precondition(win_fg.is_centered());
	mln_precondition(! win_bg.is_empty());
	mln_precondition((win_fg && win_bg).is_empty());

	// Avoid warnings.
	(void) input_;
	(void) win_fg_;
	(void) win_bg_;
      }

    } // end of namespace mln::morpho::internal


    template <typename I, typename Wfg, typename Wbg>
    inline
    mln_concrete(I)
      thinning(const Image<I>& input,
	       const Window<Wfg>& win_fg, const Window<Wbg>& win_bg)
    {
      trace::entering("morpho::thinning");


      internal::thinning_tests(input, win_fg, win_bg);

      mln_concrete(I) output = morpho::minus( input,
					      hit_or_miss(input, win_fg, win_bg) );

      mln_postcondition( complementation( thickening( complementation(input),
						      win_bg, win_fg ) ) == output);
      trace::exiting("morpho::thinning");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_THINNING_HH
