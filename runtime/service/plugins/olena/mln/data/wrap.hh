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

#ifndef MLN_DATA_WRAP_HH
# define MLN_DATA_WRAP_HH

/// \file
///
/// \brief Routine to wrap values such as 0 -> 0 and [1, lmax] maps to [1,
/// Lmax] (using modulus).


#include <mln/core/concept/image.hh>

#include <mln/data/transform.hh>
#include <mln/fun/v2v/wrap.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Routine to wrap values such as 0 -> 0 and [1, lmax] maps to [1,
               Lmax] (using modulus).

        \param[in] v     The target value type.
	\param[in] input Input image.

	\return An image with wrapped values.
    */
    template <typename V, typename I>
    mln_ch_value(I, V)
    wrap(const V& v, const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    template <typename V, typename I>
    mln_ch_value(I, V)
    wrap(const V& v, const Image<I>& input_)
    {
      trace::entering("mln::data::wrap");

      (void) v;
      const I& input = exact(input_);

      mln_precondition(input.is_valid());

      mln_ch_value(I, V)
	output = data::transform(input, fun::v2v::wrap<V>());

      trace::exiting("mln::data::wrap");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_WRAP_HH
