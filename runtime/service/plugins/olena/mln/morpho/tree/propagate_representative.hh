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

#ifndef MLN_MORPHO_TREE_PROPAGATE_REPRESENTATIVE_HH
# define MLN_MORPHO_TREE_PROPAGATE_REPRESENTATIVE_HH

/// \file
///
/// Component tree routines.


# include <mln/core/concept/image.hh>
# include <mln/morpho/tree/data.hh>


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      /// Propagate the representative node's value to
      /// non-representative points of the component.
      ///
      /// \param t   Component tree.
      /// \param f_  Value image.
      //
      template <typename T, typename F>
      void
      propagate_representative(const T& t, Image<F>& f_);



# ifndef MLN_INCLUDE_ONLY


      template <typename T, typename F>
      inline
      void
      propagate_representative(const T& t, Image<F>& f_)
      {
	F a = exact(f_);
	mln_precondition(a.is_valid());

	mln_up_site_piter(T) p(t);
	for_all(p)
	  if (! t.is_a_node(p))
	    {
	      mln_assertion(t.is_a_node(t.parent(p)));
	      a(p) = a(t.parent(p));
	    }
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_PROPAGATE_REPRESENTATIVE_HH
