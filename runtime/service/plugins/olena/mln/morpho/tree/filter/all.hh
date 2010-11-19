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

#ifndef MLN_MORPHO_TREE_FILTER_ALL_HH
# define MLN_MORPHO_TREE_FILTER_ALL_HH

/**
** \file   mln/morpho/tree/filter.hh
**
** Methods to handle component tree filtering strategies with
** non-increasing attribute. Nevertheless, it works on increasing
** predicate as well. In this case, all strategies have the same
** result but min filter or direct filter should be used in term
** of performance. If a predicate test is too slow, then consider
** the min filter that minimizes calls to predicate function.
*/

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      /// Namespace for attribute filtering.
      namespace filter {}

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln


// Pruning strategies.
# include <mln/morpho/tree/filter/min.hh>
# include <mln/morpho/tree/filter/max.hh>

// Non-pruning stategies.
# include <mln/morpho/tree/filter/direct.hh>
# include <mln/morpho/tree/filter/subtractive.hh>

#endif // ! MLN_MORPHO_TREE_FILTER_ALL_HH
