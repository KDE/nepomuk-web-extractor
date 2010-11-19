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

#ifndef MLN_METAL_ALL_HH
# define MLN_METAL_ALL_HH

/// \file
///
/// File that includes all meta-programming tools.


namespace mln
{

  /// Namespace of meta-programming tools.
  namespace metal
  {
    /// Implementation namespace of metal namespace.
    namespace impl {}

    /// Internal namespace of metal namespace.
    namespace internal {}

  }

} // end of namespace mln


# include <mln/metal/none.hh>

# include <mln/metal/abort.hh>
# include <mln/metal/ands.hh>
# include <mln/metal/bexpr.hh>
# include <mln/metal/bool.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/not_equal.hh>
# include <mln/metal/if.hh>

# include <mln/metal/converts_to.hh>
# include <mln/metal/is.hh>
# include <mln/metal/is_not.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/is_not_a.hh>
# include <mln/metal/goes_to.hh>

# include <mln/metal/const.hh>
# include <mln/metal/unconst.hh>
# include <mln/metal/is_const.hh>
# include <mln/metal/is_not_const.hh>

# include <mln/metal/unptr.hh>
# include <mln/metal/unref.hh>

# include <mln/metal/unqualif.hh>
# include <mln/metal/is_unqualif.hh>

# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>

# include <mln/metal/math/all.hh>

# include <mln/metal/array.hh>
# include <mln/metal/array1d.hh>
# include <mln/metal/array2d.hh>
# include <mln/metal/array3d.hh>

// FIXME: Remove the following includes below!
# include <mln/metal/same_coord.hh>
# include <mln/metal/same_point.hh>


#endif // ! MLN_METAL_ALL_HH
