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

#ifndef MLN_ESTIM_MEAN_HH
# define MLN_ESTIM_MEAN_HH

/// \file
///
/// Compute the mean pixel value.

# include <mln/accu/stat/mean.hh>
# include <mln/data/compute.hh>


namespace mln
{

  namespace estim
  {

    /// Compute the mean value of the pixels of image \p input.
    /*!
     * \param[in] input The image.
     * \return The mean value.
     */
    template <typename I>
    mln_sum(mln_value(I)) mean(const Image<I>& input);


    /// Compute the mean value of the pixels of image \p input.
    /*!
     * \param[in] input The image.
     * \param[out] result The mean value.
     *
     * The free parameter \c S is the type used to compute the
     * summation.
     */
    template <typename S, typename I, typename M>
    void mean(const Image<I>& input, M& result);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_sum(mln_value(I)) mean(const Image<I>& input)
    {
      mln_precondition(exact(input).is_valid());
      return data::compute(accu::meta::stat::mean(), input);
    }

    template <typename S, typename I, typename M>
    inline
    void mean(const Image<I>& input, M& result)
    {
      mln_precondition(exact(input).is_valid());
      accu::stat::mean<mln_value(I), S, M> a;
      result = data::compute(a, input);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::estim

} // end of namespace mln


#endif // ! MLN_ESTIM_MEAN_HH
