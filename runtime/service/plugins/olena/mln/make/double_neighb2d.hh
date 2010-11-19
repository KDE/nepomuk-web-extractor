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

#ifndef MLN_MAKE_DOUBLE_NEIGHB2D_HH
# define MLN_MAKE_DOUBLE_NEIGHB2D_HH

/// \file
///
/// \brief Routine to create a double neighborhood.
///
/// \todo Add overload with 'when_*' being Neighborhood<N>...

# include <mln/convert/to.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/win/multiple.hh>
# include <mln/core/neighb.hh>


namespace mln
{

  namespace make
  {

    template <typename A, unsigned St, unsigned Sf>
    neighb< win::multiple<window2d, bool(*)(A)> >
    double_neighb2d(bool (*test)(A),
		    bool const (&when_true) [St],
		    bool const (&when_false)[Sf]);

    template <typename F, unsigned St, unsigned Sf>
    neighb< win::multiple<window2d, F> >
    double_neighb2d(const F& test,
		    bool const (&when_true) [St],
		    bool const (&when_false)[Sf]);


# ifndef MLN_INCLUDE_ONLY

    template <typename A, unsigned St, unsigned Sf>
    inline
    neighb< win::multiple<window2d, bool(*)(A)> >
    double_neighb2d(bool (*test)(A),
		    bool const (&when_true) [St],
		    bool const (&when_false)[Sf])
    {
      typedef win::multiple<window2d, bool(*)(A)> W;
      W wm(test);
      wm.set_window(false, convert::to<window2d>(when_false)); // 0
      wm.set_window(true,  convert::to<window2d>(when_true) ); // 1
      neighb<W> tmp(wm);
      return tmp;
    }

    template <typename F, unsigned St, unsigned Sf>
    inline
    neighb< win::multiple<window2d, F> >
    double_neighb2d(const F& test,
		    bool const (&when_true) [St],
		    bool const (&when_false)[Sf])
    {
      typedef win::multiple<window2d, F> W;
      W wm(test);
      wm.set_window(false, convert::to<window2d>(when_false)); // 0
      wm.set_window(true,  convert::to<window2d>(when_true) ); // 1
      neighb<W> tmp(wm);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_DOUBLE_NEIGHB2D_HH
