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

#ifndef MLN_MAKE_NEIGHB2D_HH
# define MLN_MAKE_NEIGHB2D_HH

/// \file
///
/// \brief Routine to create a double neighborhood.
///
/// \todo Add overload with 'when_*' being Neighborhood<N>...

# include <mln/core/alias/neighb2d.hh>


namespace mln
{

  namespace make
  {

    template <unsigned S>
    mln::neighb2d
    neighb2d(bool const (&vals) [S]);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned S>
    inline
    mln::neighb2d
    neighb2d(bool const (&vals) [S])
    {
      enum { h = mlc_sqrt_int(S) / 2 };
      mlc_bool((2 * h + 1) * (2 * h + 1) == S)::check();
      mln::neighb2d nbh;
      convert::from_to(vals, nbh);
      return nbh;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_NEIGHB2D_HH
