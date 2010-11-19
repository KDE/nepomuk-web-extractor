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

#ifndef MLN_IO_PGMS_LOAD_HH
# define MLN_IO_PGMS_LOAD_HH

/// \file
///
/// \brief Define a function which loads multiple pgm images into
/// a 3D image.

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pnms/load.hh>
# include <mln/make/image3d.hh>


namespace mln
{

  namespace io
  {

    namespace pgms
    {

      /// Load pgm images as slices of a 3D Milena image.
      ///
      /// \param[out] ima A reference to the 3D image which will receive
      /// data.
      /// \param[in] filenames The list of 2D images to load..
      ///
      template <typename V>
      void load(image3d<V>& ima,
		const util::array<std::string>& filenames);

# ifndef MLN_INCLUDE_ONLY


      template <typename V>
      inline
      void load(image3d<V>& ima,
		const util::array<std::string>& filenames)
      {
	trace::entering("mln::io::pgms::load");

	io::pnms::load<V>(PGM, ima, filenames);

	trace::exiting("mln::io::pgms::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pgms

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PGMS_LOAD_HH
