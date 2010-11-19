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

#ifndef MLN_VALUE_SET_HH
# define MLN_VALUE_SET_HH

/// \file
///
/// Define some basic sets of values from value types.

# include <mln/value/internal/iterable_set.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {

      template <typename T, typename E, bool is_lowq = false>
      struct set_selector_ // no inheritance
      {};

      template <typename T, typename E>
      struct set_selector_< T, E, true > // lowq so iterable
	:
	public iterable_set< T, E >
      {};

    } // end of namespace mln::value::internal



    /// Class that defines the set of values of type \c T.
    ///
    /// This is the exhaustive set of values obtainable from type \c T.
    ///
    template <typename T>
    struct set : public internal::set_selector_< T, set<T>,
						 mlc_equal( mln_trait_value_quant(T),
							    mln::trait::value::quant::low )::value
						 >
    {
      /// Return a singleton.
      static const set<T>& the();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    const set<T>&
    set<T>::the()
    {
      static set<T> the_;
      return the_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_SET_HH
