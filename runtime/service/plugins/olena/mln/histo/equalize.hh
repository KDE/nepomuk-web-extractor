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

#ifndef MLN_HISTO_EQUALIZATION_HH
# define MLN_HISTO_EQUALIZATION_HH

# include <mln/core/concept/image.hh>
# include <mln/histo/all.hh>

/// \file
///
/// Function histogram equalization.

namespace mln
{

  namespace histo
  {

    /**
     Equalizes the histogram of image \p input.
     \author J. Fabrizio, R. Levillain
     */
    template <typename I>
    mln_concrete(I)
    equalize(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    mln_concrete(I)
    equalize(const Image<I>& input_)
    {
      trace::entering("histo::equalize");

      const I& input = exact(input_);
      mln_concrete(I) out_image;

      array<mln_value(I)> histogram = compute(input);
      array<mln_value(I)> histogram_correction;

      unsigned cumulation = 0;
      int number_of_pixels = input.nsites();
      //int number_of_colors=histogram.nvalues();
      //int number_of_colors=mln_card(mln_value(I));
      int max_color = mln_max(mln_value(I));
      mln_piter(I) p(input.domain());

      mln_viter(mln::value::set<mln_value(I)>) v(histogram.vset());
      for_all(v)
	if (histogram(v) != 0)
	{
	  cumulation += histogram(v);
	  histogram_correction(v) = (/*number_of_colors-1*/max_color)
	    * cumulation / number_of_pixels;
	}

      initialize(out_image, input);
      for_all(p)
	out_image(p) = histogram_correction[input(p)];

      trace::exiting("histo::equalize");
      return out_image;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln



#endif // ! MLN_HISTO_EQUALIZATION_HH
