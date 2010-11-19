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

#ifndef MLN_WORLD_INTER_PIXEL_IS_SEPARATOR_HH
# define MLN_WORLD_INTER_PIXEL_IS_SEPARATOR_HH

/// \file
///
/// FIXME: doc.
///
/// \todo Make it work in n-D.

# include <mln/core/concept/function.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/point.hh>


namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {


      /// Functor returning whether a site is a separator in an
      /// inter-pixel image.
      struct is_separator : public Function_v2b< is_separator >
      {

	typedef bool result;

	template <typename P>
	bool operator()(const Gpoint<P>& p) const;

	template <typename P>
	bool operator()(const Site_Proxy<P>& p) const;

      };


# ifndef MLN_INCLUDE_ONLY

      template <typename P>
      inline
      bool
      is_separator::operator()(const Gpoint<P>& p_) const
      {
	const P& p = exact(p_);
	return p.row() % 2 + p.col() % 2 == 1;
      }

      template <typename P>
      inline
      bool
      is_separator::operator()(const Site_Proxy<P>& p) const
      {
	mlc_is_a(mln_site(P), Gpoint)::check();
	return this->operator()(exact(p).to_site());
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_IS_SEPARATOR_HH
