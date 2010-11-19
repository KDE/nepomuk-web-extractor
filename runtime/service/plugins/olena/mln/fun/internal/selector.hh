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

#ifndef MLN_FUN_INTERNAL_SELECTOR_HH
# define MLN_FUN_INTERNAL_SELECTOR_HH

/// \file
///
/// \brief Select a concept (either Function_v2v or Function_v2b)
/// depending on the result type.

# include <mln/core/concept/function.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  namespace fun
  {

    namespace internal
    {


      template <typename R, typename A, typename E>
      struct helper_selector_
      {
	typedef Function_v2v<E> ret;
      };

      template <typename A, typename E>
      struct helper_selector_< bool, A, E >
      {
	typedef Function_v2b<E> ret;
      };


      template <typename R_, typename A_, typename E>
      struct selector_
      {
	typedef mlc_unqualif(R_) R;
	typedef mlc_unqualif(A_) A;
	typedef typename helper_selector_<R, A, E>::ret ret;
      private:
	selector_();
      };

      template <typename R_, typename E>
      struct selector_from_result_ : selector_< R_, void, E >
      {
      private:
	selector_from_result_();
      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_SELECTOR_HH
