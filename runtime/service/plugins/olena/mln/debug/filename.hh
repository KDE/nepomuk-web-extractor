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

#ifndef MLN_DEBUG_FILENAME_HH
# define MLN_DEBUG_FILENAME_HH

/// \file
///
/// Constructs and returns a formatted output file name.


# include <string>
# include <sstream>


namespace mln
{

  namespace debug
  {

    /// Constructs and returns a formatted output file name.
    /*!
    ** The file name is formatted as follow:
    **
    ** `filename_prefix`_`id`_`filename`
    **
    ** Where:
    **  - `filename_prefix` can be set through the global variable
    **  debug::internal::filename_prefix.
    ** - `postfix_id` is autoincremented by default. Its value can be
         forced.
    **  - `filename` is the given filename
    */
    std::string
    filename(const std::string& filename, int id);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      std::string filename_prefix = "";

    } // end of namespace mln::debug::internal


    inline
    std::string
    filename(const std::string& filename, int id = -1)
    {
      static int file_id = 1;

      std::ostringstream os;

      if (! internal::filename_prefix.empty())
	os << internal::filename_prefix << "_";

      if (id == -1)
      {
	if (file_id < 10)
	  os << "0";
	if (file_id < 100)
	  os << "0";

	os << file_id++;
      }
      else
	os << id;

      os << "_"
	 << filename;

      return os.str();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln

#endif // ! MLN_DEBUG_FILENAME_HH
