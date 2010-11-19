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

#ifndef MLN_CORE_INTERNAL_EXACT_HH
# define MLN_CORE_INTERNAL_EXACT_HH

/// \file
///
/// \brief Meta-code for the mln::exact downcast routines.



/// Forward declaration.
namespace mln
{

  template <typename T>
  struct Object;

} // End of namespace mln


namespace mln
{

  namespace internal
  {

    typedef char yes_;
    struct no_ { char dummy[2]; };

    template <typename T>
    struct make_
    {
      static T* ptr();
    };

    template <unsigned id, typename T>
    struct exact_ret_;

    template <typename T>
    struct exact_ret_< 1, T >
    {
      typedef typename T::exact_t ret;
    };

    template <typename T>
    struct exact_ret_< 2, T >
    {
      typedef T ret;
    };

    template <typename E>
    yes_ exact_selector_(Object<E>*);

    no_  exact_selector_(void*);

    template <typename E, typename T>
    E* exact_run_(Object<E>* t, T*);

    template <typename T>
    T* exact_run_(void*, T* t);

    template <typename T>
    struct exact_
    {
      enum { id = sizeof(exact_selector_(make_<T>::ptr())) };
      typedef exact_ret_<id, T> helper;
      typedef typename helper::ret ret;
      static ret* run(T* t)
      {
	return exact_run_(t, t);
      }
    };

    template <typename T>
    struct exact_<const T>
    {
      enum { id = sizeof(exact_selector_(make_<T>::ptr())) };
      typedef exact_ret_<id, T> helper;
      typedef const typename helper::ret ret;
      static ret* run(const T* t)
      {
	return exact_run_((T*)t, (T*)t);
      }
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    T*
    make_<T>::ptr() // This piece of code is defined to prevent an ICE from g++-2.95.
    {
      T* tmp;
      return tmp;
    }

    template <typename E, typename T>
    inline
    E* exact_run_(Object<E>* t, T*)
    {
      return (E*)(void*)t;
    }

    template <typename T>
    inline
    T* exact_run_(void*, T* t)
    {
      return t;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

# include <mln/core/concept/object.hh>

#endif // ! MLN_CORE_INTERNAL_EXACT_HH
