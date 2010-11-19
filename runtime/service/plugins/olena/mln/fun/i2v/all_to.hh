// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_I2V_ALL_TO_HH
# define MLN_FUN_I2V_ALL_TO_HH

/// \file
///
/// FIXME.

# include <mln/core/concept/function.hh>


namespace mln
{

  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      struct all_to : public Function_v2v< all_to<T> >
      {
	typedef T result;
	all_to(T t);
	template <typename U>
	T operator()(const U&) const;
      private:
	T t_;
      };

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun

  template <typename T>
  fun::i2v::all_to<T> all_to(T t);


# ifndef MLN_INCLUDE_ONLY

  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      inline
      all_to<T>::all_to(T t)
	: t_(t)
      {
      }

      template <typename T>
      template <typename U>
      inline
      T
      all_to<T>::operator()(const U&) const
      {
	return t_;
      }

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun

  template <typename T>
  inline
  fun::i2v::all_to<T> all_to(T t)
  {
    fun::i2v::all_to<T> tmp(t);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_I2V_ALL_TO_HH
