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

#ifndef MLN_UTIL_LEMMINGS_HH
# define MLN_UTIL_LEMMINGS_HH

/// \file
///
/// Definition of an "lemmings" object.

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace util
  {

    /*! \brief Lemmings tool.
     *
     */
    template <typename I>
    struct lemmings_ : public Object< lemmings_<I> >
    {
      lemmings_(const Image<I>& ima, const mln_psite(I)& pt,
		const mln_deduce(I, psite, delta)& dpt, const mln_value(I)& val);

      mln_psite(I) operator()();

      const I& ima_;
      mln_psite(I) pt_;
      const mln_deduce(I, psite, delta)& dpt_;
      const mln_value(I)& val_;
    };

    /*! \brief Launch a lemmings on an image.
    **
    **  A lemmings is the point \p pt that you put on an image \p ima
    **  . This point will move through the image using the delta-point
    **  \p dpt while consider his value on the given image.
    **
    ** @return The first point that is not in the domain \p domain or
    ** which value on the given image is different to the value \p
    ** val.
    **
    ** \pre The domain \p domain must be contained in the domain of \p
    ** ima.
    */
    template <typename I>
    mln_psite(I)
    lemmings(const Image<I>& ima, const mln_psite(I)& pt,
	     const mln_deduce(I, psite, delta)& dpt, const mln_value(I)& val);


# ifndef MLN_INCLUDE_ONLY

    // lemmings

    template <typename I>
    inline
    lemmings_<I>::lemmings_(const Image<I>& ima, const mln_psite(I)& pt,
			    const mln_deduce(I, psite, delta)& dpt, const mln_value(I)& val)
      : ima_(exact(ima)),
	pt_(pt),
	dpt_(dpt),
	val_(val)
    {
    }

    template <typename I>
    mln_psite(I)
    lemmings_<I>::operator()()
    {
      while (ima_.domain().has(pt_) && ima_(pt_) == val_)
	pt_ += dpt_;
      return pt_;
    }

    template <typename I>
    mln_psite(I)
    lemmings(const Image<I>& ima, const mln_psite(I)& pt,
	     const mln_deduce(I, psite, delta)& dpt, const mln_value(I)& val)
    {
      lemmings_<I> lm(ima, pt, dpt, val);
      return lm();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_LEMMINGS_HH
