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

#ifndef MLN_CORE_CATEGORY_HH
# define MLN_CORE_CATEGORY_HH

/// \file
///
/// \brief Definition of the category holder type.

# include <mln/metal/equal.hh>



namespace mln
{

  // FIXME: Doc!


  template <typename E>
  struct Unknown;


  template <>
  struct Unknown<void>
  {
    typedef Unknown<void> super;
    // Unknown is used to terminate the meta-program that solves a
    // call to a trait.  This meta-program can browse several branches
    // at the same time---if the trait takes 2 types as arguments, the
    // program runs over the trellis formed by the "couples of
    // categories"--- and when the end of one branch is reached, we do
    // not want the program to end; so it continues on that branch
    // with the no-op "jump to the super type of Unknown"!
  };


  template <typename T>
  struct category
  {
    typedef typename T::category ret; // FIXME: if found or Unknown<void> => write a meta-program...
  };

  template <typename T>
  struct category< const T >
  {
    typedef typename category<T>::ret ret;
  };



  // Utility meta-function: from a category and a type, get the super category.

  namespace internal
  {

    // The code above could be merged into a couple of structures.
    // Yet g++-2.95 needs help so we first decompose Category<S>
    // into (Category, S) then we solve.

    template < template <class> class Category, typename S, typename T >
    struct helper_super_category_solve_
      :
      private metal::equal< typename Category<void>::super, void* >::check_t
    {
      // New case.
      typedef S ret;
    };

    template < template <class> class Category, typename T >
    struct helper_super_category_solve_< Category, void, T >
    {
      typedef typename Category<void>::super ret; // One super category: keep it.
    };

    template < typename Category, typename T >
    struct helper_super_category_;

    template < template <class> class Category, typename S, typename T >
    struct helper_super_category_< Category<S>, T > : helper_super_category_solve_< Category, S, T >
    {
    };



    // For bwd in-compatibility.
    template < template <class> class Category, typename T >
    struct helper_super_category_< Category<void*>, T >; 


    template < typename Category, typename T >
    struct super_category_ // Entry.
    {
      typedef typename helper_super_category_< Category, T >::ret ret;
    };

  } // end of namespace mln::internal


} // end of namespace mln


#endif // ! MLN_CORE_CATEGORY_HH
