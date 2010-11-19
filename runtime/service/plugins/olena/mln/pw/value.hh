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

#ifndef MLN_PW_VALUE_HH
# define MLN_PW_VALUE_HH

/// \file
///
/// FIXME.

# include <mln/fun/internal/selector.hh>
# include <mln/core/concept/image.hh>



namespace mln
{

  namespace pw
  {


    // FIXME: Doc!

    template <typename I>
    struct value_
      : fun::internal::selector_from_result_< mln_value(I), value_<I> >::ret
    {
      typedef mln_value(I) result;

      value_();
      value_(const I& ima);

      mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    protected:
      const I* ima_;
    };



    // FIXME: Doc!

    template <typename I>
    value_<I> value(const Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY

    // pw::value_<I>

    template <typename I>
    inline
    value_<I>::value_()
      : ima_(0)
    {
    }

    template <typename I>
    inline
    value_<I>::value_(const I& ima)
      : ima_(&ima)
    {
    }

    template <typename I>
    inline
    mln_rvalue(I)
    value_<I>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_ != 0);
      mln_precondition(ima_->has(p));
      return (*ima_)(p);
    }


    // pw::value(ima)

    template <typename I>
    inline
    value_<I>
    value(const Image<I>& ima)
    {
      mln_precondition(exact(ima).is_valid());
      value_<I> tmp(exact(ima));
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::pw

} // end of namespace mln


#endif // ! MLN_PW_VALUE_HH
