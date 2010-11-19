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

#ifndef MLN_MORPHO_THICK_MISS_HH
# define MLN_MORPHO_THICK_MISS_HH

/*! \file
 *
 * \brief Morphological thick-miss.
 */

# include <mln/morpho/hit_or_miss.hh>


namespace mln
{

  namespace morpho
  {


    /*! Morphological thick-miss.
     *
     * This operator is THICK_B = Id + HMTopeBG_B, where B = (Bfg, Bbg).
     */
    template <typename I, typename Wfg, typename Wbg>
    mln_concrete(I)
    thick_miss(const Image<I>& input,
	       const Window<Wfg>& win_fg, const Window<Wbg>& win_bg);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename Wfg, typename Wbg>
    inline
    mln_concrete(I)
    thick_miss(const Image<I>& input,
	       const Window<Wfg>& win_fg, const Window<Wbg>& win_bg)
    {
      trace::entering("morpho::thick_miss");
      // FIXME: Fix the following line (win_miss ??)
//       mln_precondition(exact(win_miss).is_centered());
      mln_precondition((exact(win_fg) && exact(win_bg)).is_empty());

      mln_concrete(I)
	output = morpho::plus( input,
			       hit_or_miss_background_opening(input,
							      win_fg, win_bg) );

      trace::exiting("morpho::thick_miss");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_THICK_MISS_HH
