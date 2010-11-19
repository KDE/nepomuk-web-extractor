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

#ifndef MLN_ACCU_STAT_MIN_H_HH
# define MLN_ACCU_STAT_MIN_H_HH

/// \file
///
/// Define a generic min accumulator class based on histogram.

# include <mln/accu/internal/base.hh>
# include <mln/accu/histo.hh>
# include <mln/value/set.hh>
# include <mln/util/pix.hh>


namespace mln
{

  // Forward declaration.
  namespace accu {
    namespace stat {
      template <typename V> struct min_h;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename V>
    struct accumulator_< accu::stat::min_h<V> >
    {
      typedef accumulator::has_untake::yes   has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    namespace meta
    {

      namespace stat
      {

	/// Meta accumulator for min.
	struct min_h : public Meta_Accumulator< min_h >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::stat::min_h<T> ret;
	  };
	};

      } // end of namespace mln::meta::stat

    } // end of namespace mln::meta


    namespace stat
    {

      /// \brief Generic min function based on histogram over a value
      /// set with type \c V.
      ///
      /// \ingroup modaccuvalues
      //
      template <typename V>
      struct min_h : public mln::accu::internal::base< const V& , min_h<V> >
      {
	typedef V argument;

	min_h();

	/// Manipulators.
	/// \{
	void init();
	void   take(const argument& t);
	void   take_as_init_(const argument& t);
	void   take(const min_h<V>& other);
	void untake(const argument& t);
	/// \}

	unsigned card() const { return h_.sum(); }

	/// Get the value of the accumulator.
	const argument& to_result() const;

	const accu::histo<V>& histo() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	void debug_print_() const;

      protected:

	mutable accu::histo<V> h_;
	const value::set<V>& s_; // derived from h_

	mutable unsigned sum_;
	mutable bool valid_;
	mutable unsigned i_; // the min index
	mutable argument t_;    // the min value

	// Auxiliary methods
	void update_() const;
	void go_minus_() const;
	void go_plus_() const;
      };



      template <typename I> struct min_h< util::pix<I> >;


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      min_h<V>::min_h()
	: h_(),
	  s_(h_.vset())
      {
	init();
      }

      template <typename V>
      inline
      void
      min_h<V>::take(const argument& t)
      {
	if (h_.sum() == 0)
	  {
	    this->take_as_init_(t);
	    return;
	  }
	h_.take(t);
	if (t < t_)
	  {
	    ++sum_;
	    valid_ = false;
	  }
      }

      template <typename V>
      inline
      void
      min_h<V>::take(const min_h<V>& other)
      {
	// h_
	h_.take(other.h_);
	for (unsigned i = 0; i < i_; ++i)
	  sum_ += other.h_[i];
	valid_ = false;
	// FIXME: Optimize.
      }

      template <typename V>
      inline
      void
      min_h<V>::untake(const argument& t)
      {
	mln_precondition(h_(t) != 0);
	h_.untake(t);
	if (h_.sum() == 0)
	  {
	    init();
	    return;
	  }
	if (t < t_)
	  {
	    mln_invariant(sum_ >= 1);
	    --sum_;
	    valid_ = false;
	  }
	else
	  if (t == t_ && h_[i_] == 0)
	    valid_ = false;
      }

      template <typename V>
      inline
      void
      min_h<V>::update_() const
      {
	if (sum_ != 0)
	  go_minus_();
	else
	  if (h_[i_] == 0)
	    go_plus_();
	valid_ = true;
      }

      template <typename V>
      inline
      void
      min_h<V>::go_minus_() const
      {
	do
	  {
	    --i_;
	    if (h_[i_] != 0)
	      sum_ -= h_[i_];
	  }
	while (sum_ != 0);
	t_ = s_[i_];
      }

      template <typename V>
      inline
      void
      min_h<V>::go_plus_() const
      {
	do
	  ++i_;
	while (h_[i_] == 0);
	t_ = s_[i_];
      }

      template <typename V>
      inline
      void
      min_h<V>::init()
      {
	h_.init();
	sum_ = 0;
	i_ = mln_max(argument);
	t_ = s_[i_];
	valid_ = true;
      }

      template <typename V>
      inline
      void
      min_h<V>::take_as_init_(const argument& t)
      {
	h_.take(t);
	sum_ = 0;
	i_ = s_.index_of(t);
	t_ = t;
	valid_ = true;
      }

      template <typename V>
      inline
      const typename min_h<V>::argument&
      min_h<V>::to_result() const
      {
	if (! valid_)
	  update_();
	return t_;
      }

      template <typename V>
      inline
      const accu::histo<V>&
      min_h<V>::histo() const
      {
	return h_;
      }

      template <typename V>
      inline
      bool
      min_h<V>::is_valid() const
      {
	return true;
      }


      template <typename V>
      inline
      void
      min_h<V>::debug_print_() const
      {
	std::cout << "h={" << h_ << "} ";
	std::cout << "sum=" << sum_ << ' '
		  << "valid=" << valid_ << ' '
		  << "i=" << i_ << ' '
		  << "t=" << t_ << std::endl;
      }

      template <typename V>
      inline
      std::ostream& operator<<(std::ostream& ostr, const min_h<V>& m)
      {
	return ostr << m.to_result();
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_STAT_MIN_H_HH
