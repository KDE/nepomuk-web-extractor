// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_INTERNAL_EDGE_IMPL_HH
# define MLN_UTIL_INTERNAL_EDGE_IMPL_HH

/// \file
///
/// Define a couple of implementation classes to provide methods
/// to classes of generalized edges.

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace util
  {

    namespace internal
    {

      /// Implementation class to equip generalized edge classes.
      template <typename G>
      class edge_impl_
      {
	protected:
	  edge_impl_();
      };

    } // end of namespace internal

  } // end of namespace util

} // end of namespace mln

#ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace util
  {

    namespace internal
    {

    template <typename G>
    inline
    edge_impl_<G>::edge_impl_()
    {
    }

    } // end of namespace internal

  } // end of namespace util

} // end of namespace mln

#endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_INTERNAL_EDGE_IMPL_HH
