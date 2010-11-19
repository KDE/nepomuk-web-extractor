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

#ifndef MLN_ACCU_STAT_MAX_HH
# define MLN_ACCU_STAT_MAX_HH

/// \file
///
/// Define an accumulator that computes a max.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{


  // Forward declaration.
  namespace accu {
    namespace stat {
      template <typename T> struct max;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename T>
    struct accumulator_< accu::stat::max<T> >
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

	/// Meta accumulator for max.
	struct max : public Meta_Accumulator< max >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::stat::max<T> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta


    namespace stat
    {

      /// \brief Generic max accumulator class.
      ///
      /// The parameter \c T is the type of values.
      ///
      /// \ingroup modaccuvalues
      template <typename T>
      struct max : public mln::accu::internal::base< const T& , max<T> >
      {
	typedef T argument;

	max();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init_(const argument& t);
	void take(const argument& t);
	void take(const max<T>& other);
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


      template <typename I> struct max< util::pix<I> >;


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      max<T>::max()
      {
	init();
      }

      template <typename T>
      inline
      void
      max<T>::init()
      {
	t_ = mln_min(T);
      }

      template <typename T>
      inline
      void
      max<T>::take_as_init_(const argument& t)
      {
	t_ = t;
      }

      template <typename T>
      inline
      void
      max<T>::take(const argument& t)
      {
	if (t > t_)
	  t_ = t;
      }

      template <typename T>
      inline
      void
      max<T>::take(const max<T>& other)
      {
	if (other.t_ > t_)
	  t_ = other.t_;
      }

      template <typename T>
      inline
      void
      max<T>::set_value(const T& t)
      {
	t_ = t;
      }

      template <typename T>
      inline
      const T&
      max<T>::to_result() const
      {
	return t_;
      }

      template <typename T>
      inline
      bool
      max<T>::is_valid() const
      {
	return true;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MAX_HH
