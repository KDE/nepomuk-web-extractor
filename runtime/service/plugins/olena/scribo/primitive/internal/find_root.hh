// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_FIND_ROOT_HH
# define SCRIBO_PRIMITIVE_INTERNAL_FIND_ROOT_HH

/// \file
///
/// Find root in a parent array arrays.


# include <scribo/core/object_groups.hh>


namespace scribo
{

  namespace primitive
  {

      namespace internal
      {

	/// Find root in a parent array arrays.
	template <typename L>
	unsigned
        find_root(object_groups<L>& parent, unsigned x);

# ifndef MLN_INCLUDE_ONLY

	template <typename L>
	inline
	unsigned
        find_root(object_groups<L>& parent, unsigned x)
        {
          if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent, parent(x));
	}

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_FIND_ROOT_HH
