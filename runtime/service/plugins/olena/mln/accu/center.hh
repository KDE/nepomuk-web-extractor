// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ACCU_CENTER_HH
# define MLN_ACCU_CENTER_HH

/// \file
///
/// Define an accumulator that computes the mass center of a site set.
///
/// \todo Fix to_result() value when invalid...

# include <mln/accu/internal/base.hh>
# include <mln/accu/shape/bbox.hh>

namespace mln
{

  namespace accu
  {


    /// \brief Mass center accumulator.
    ///
    /// \tparam P the type of site.
    /// \tparam V the type of vector to be used as result.
    ///		  The default vector type is the one provided by P.
    ///
    /// \ingroup modaccusiteset
    //
    template <typename P, typename V = typename P::vec>
    struct center
      : public mln::accu::internal::base<V, center<P,V> >
    {
      typedef P argument;
      typedef V result;

      center();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const center<P,V>& other);
      /// \}

      /// Get the value of the accumulator.
      V to_result() const;
      operator P() const;

      /// Check whether this accu is able to return a result.
      bool is_valid() const;

      /// Return the number of sites taken in consideration.
      unsigned nsites() const;

    protected:
      algebra::vec<P::dim, mln_sum(mln_coord(P))> center_;
      unsigned nsites_;
    };

    namespace meta
    {

      /// Meta accumulator for center.
      struct center : public Meta_Accumulator< center >
      {

	template <typename P>
	struct with
	{
	  typedef accu::center<P> ret;
	};

      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename V>
    inline
    center<P,V>::center()
    {
      init();
    }

    template <typename P, typename V>
    inline
    void
    center<P,V>::init()
    {
      center_ = literal::zero;
      nsites_ = 0;
    }

    template <typename P, typename V>
    inline
    void center<P,V>::take(const argument& t)
    {
      center_ += t.to_vec();
      ++nsites_;
    }

    template <typename P, typename V>
    inline
    void
    center<P,V>::take(const center<P,V>& other)
    {
      center_ += other.center_;
      nsites_ += other.nsites_;
    }

    template <typename P, typename V>
    inline
    V
    center<P,V>::to_result() const
    {
      // mln_precondition(is_valid());
      if (! is_valid())
	return V();
      return center_ / nsites_;
    }

    template <typename P, typename V>
    inline
    center<P,V>::operator P() const
    {
      return P(to_result());
    }

    template <typename P, typename V>
    inline
    bool
    center<P,V>::is_valid() const
    {
      return nsites_ > 0;
    }

    template <typename P, typename V>
    inline
    unsigned
    center<P,V>::nsites() const
    {
      return nsites_;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_CENTER_HH
