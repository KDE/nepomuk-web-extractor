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

#ifndef MLN_CORE_IMAGE_ALL_HH
# define MLN_CORE_IMAGE_ALL_HH

/// \file
///
/// File that includes all image types.


// Sub-directories.

# include <mln/core/image/dmorph/all.hh>
# include <mln/core/image/imorph/all.hh>
# include <mln/core/image/vmorph/all.hh>


// Files.

# include <mln/core/image/ch_piter.hh>
# include <mln/core/image/complex_image.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_window_piter.hh>
# include <mln/core/image/complex_windows.hh>
# include <mln/core/image/edge_image.hh>
# include <mln/core/image/flat_image.hh>
# include <mln/core/image/graph_elt_neighborhood.hh>
# include <mln/core/image/graph_elt_neighborhood_if.hh>
# include <mln/core/image/graph_elt_window.hh>
# include <mln/core/image/graph_elt_window_if.hh>
# include <mln/core/image/graph_window_if_piter.hh>
# include <mln/core/image/graph_window_piter.hh>
# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/image/vertex_image.hh>


#endif // ! MLN_CORE_IMAGE_ALL_HH
