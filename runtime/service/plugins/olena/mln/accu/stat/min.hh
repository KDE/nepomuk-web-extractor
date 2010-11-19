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

#ifndef MLN_ACCU_STAT_MIN_HH
# define MLN_ACCU_STAT_MIN_HH

/// \file
///
/// Define an accumulator that computes a min.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{


  // Forward declaration.
  namespace accu {
    namespace stat {
      template <typename T> struct min;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename T>
    struct accumulator_< accu::stat::min<T> >
    {
      typedef accumulator::has_untake::no     has_untake;
      typedef accumulator::has_set_value::yes has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::use_v    when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    namespace meta
    {

      namespace stat
      {
	/// Meta accumulator for min.

	struct min : public Meta_Accumulator< min >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::stat::min<T> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta


    namespace stat
    {

      /// \brief Generic min accumulator class.
      /*!
      * The parameter \c T is the type of values.
      *
      * \ingroup modaccuvalues
      */
      template <typename T>
      struct min : public mln::accu::internal::base< const T&, min<T> >
      {
	typedef T argument;

	min();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init_(const argument& t);
	void take(const argument& t);
	void take(const min<T>& other);
	/// \}

	/// Force the value of the min to \a t.
	void set_value(const T& t);

	/// Get the value of the accumulator.
	const T& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	T t_;
      };


      template <typename I> struct min< util::pix<I> >;


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      min<T>::min()
      {
	init();
      }

      template <typename T>
      inline
      void
      min<T>::init()
      {
	t_ = mln_max(T);
      }

      template <typename T>
      inline
      void min<T>::take_as_init_(const argument& t)
      {
	t_ = t;
      }

      template <typename T>
      inline
      void min<T>::take(const argument& t)
      {
	if (t < t_)
	  t_ = t;
      }

      template <typename T>
      inline
      void
      min<T>::take(const min<T>& other)
      {
	if (other.t_ < t_)
	  t_ = other.t_;
      }

      template <typename T>
      inline
      void
      min<T>::set_value(const T& t)
      {
	t_ = t;
      }

      template <typename T>
      inline
      const T&
      min<T>::to_result() const
      {
	return t_;
      }

      template <typename T>
      inline
      bool
      min<T>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MIN_HH
