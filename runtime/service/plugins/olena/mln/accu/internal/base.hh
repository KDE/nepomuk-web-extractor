// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ACCU_INTERNAL_BASE_HH
# define MLN_ACCU_INTERNAL_BASE_HH

/// \file
///
/// Define a base class for implementation of accumulator
/// classes.

# include <mln/core/concept/accumulator.hh>

namespace mln
{

  namespace accu
  {

    namespace internal
    {

      /// Generic min accumulator class.
      /// Base class for implementation of accumulator classes.
      template <typename R, typename E>
      class base : public Accumulator<E>,
		   public mln::internal::proxy_impl< R, E >
      {
      public:

	// As a proxy:
	R subj_();

	// As an accumulator:
	typedef R               q_result;
	typedef mlc_unqualif(R)   result;

      protected:
	base();
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename R, typename E>
      inline
      base<R,E>::base()
      {
      }

      template <typename R, typename E>
      inline
      R
      base<R,E>::subj_()
      {
	return exact(this)->to_result();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::internal

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_INTERNAL_BASE_HH
