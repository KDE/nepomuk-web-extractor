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

#ifndef MLN_LINEAR_CONVOLVE_DIRECTIONAL_HH
# define MLN_LINEAR_CONVOLVE_DIRECTIONAL_HH

/// mln/linear/convolve_directional.hh
///
/// Convolution by a line-shaped (directional) kernel.

# include <mln/linear/convolve.hh>
# include <mln/make/w_window_directional.hh>
# include <mln/literal/zero.hh>



namespace mln
{

  namespace linear
  {

    /*! Convolution of an image \p input by a line-shaped
     *  (directional) weighted window defined by the array of \p
     *  weights.
     *
     * \warning Computation of \p output(p) is performed with the
     * value type of \p output.
     *
     * \warning The weighted window is used as-is, considering that
     * its symmetrization is handled by the client.
     *
     * \pre input.is_valid
     */
    template <typename I, typename W, unsigned S>
    mln_ch_convolve(I, W)
    convolve_directional(const Image<I>& input, unsigned dir, W (&weights)[S]);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename W, unsigned S>
    inline
    mln_ch_convolve(I, W)
    convolve_directional(const Image<I>& input, unsigned dir, W (&weights)[S])
    {
      trace::entering("linear::convolve_directional");

      mlc_bool(S % 2 == 1)::check();

      mln_precondition(exact(input).is_valid());
      typedef mln_site(I) P;
      mln_precondition(dir < P::dim);

      typedef mln_delta(P) D;
      D dp = literal::zero;
      ++dp[dir];
      w_window<D,W> w_win = make::w_window_directional(dp, weights);
      mln_ch_convolve(I, W) output = convolve(input, w_win);

      trace::exiting("linear::convolve_directional");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CONVOLVE_DIRECTIONAL_HH
