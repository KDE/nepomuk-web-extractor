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

#ifndef MLN_ACCU_SHAPE_BBOX_HH
# define MLN_ACCU_SHAPE_BBOX_HH

/// \file
///
/// Define an accumulator that computes a bbox.

# include <mln/core/site_set/box.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace accu
  {

    namespace shape
    {


      /// \brief Generic bounding box accumulator class.
      ///
      /// The parameter \c P is the type of points.
      ///
      /// \ingroup modaccusiteset
      //
      template <typename P>
      struct bbox : public mln::accu::internal::base< const box<P>& , bbox<P> >
      {
	typedef P argument;

	bbox();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init_(const P& p);
	void take(const P& p);
	void take(const bbox<P>& other);
	void take(const box<P>& b);
	/// \}

	/// Get the value of the accumulator.
	const box<P>& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	bool is_valid_;
	box<P> b_;
      };


    } // end of namespace mln::accu::shape


    namespace meta
    {

      namespace shape
      {

	/// Meta accumulator for bbox.
	struct bbox : public Meta_Accumulator< bbox >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::shape::bbox<T> ret;
	  };
	};

      } // end of namespace mln::accu::meta::shape

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    namespace shape
    {

      template <typename P>
      inline
      bbox<P>::bbox()
      {
	init();
      }

      template <typename P>
      inline
      void
      bbox<P>::init()
      {
	is_valid_ = false;
      }

      template <typename P>
      inline
      void
      bbox<P>::take_as_init_(const P& p)
      {
	b_.pmin() = p;
	b_.pmax() = p;
	is_valid_ = true;
      }

      template <typename P>
      inline
      void
      bbox<P>::take(const P& p)
      {
	if (! is_valid_)
	  {
	    b_.pmin() = p;
	    b_.pmax() = p;
	    is_valid_ = true;
	    return;
	  }
	for (unsigned i = 0; i < P::dim; ++i)
	  if (p[i] < b_.pmin()[i])
	    b_.pmin()[i] = p[i];
	  else if (p[i] > b_.pmax()[i])
	    b_.pmax()[i] = p[i];
      }

      template <typename P>
      inline
      void
      bbox<P>::take(const bbox<P>& other)
      {
	if (! other.is_valid_)
	  {
	    // no-op
	    return;
	  }
	if (! this->is_valid_)
	  {
	    // 'other' makes '*this' valid
	    *this = other;
	    is_valid_ = true;
	    return;
	  }
	// both are valids so:
	const box<P>& o_b = other.b_;
	for (unsigned i = 0; i < P::dim; ++i)
	  {
	    if (o_b.pmin()[i] < b_.pmin()[i])
	      b_.pmin()[i] = o_b.pmin()[i];
	    if (o_b.pmax()[i] > b_.pmax()[i])
	      b_.pmax()[i] = o_b.pmax()[i];
	  }
      }

      template <typename P>
      inline
      void
      bbox<P>::take(const box<P>& b)
      {
	if (! b.is_valid())
	  {
	    // no-op
	    return;
	  }
	if (! this->is_valid_)
	  {
	    b_ = b;
	    is_valid_ = true;
	    return;
	  }
	// both are valids so:
	for (unsigned i = 0; i < P::dim; ++i)
	  {
	    if (b.pmin()[i] < b_.pmin()[i])
	      b_.pmin()[i] = b.pmin()[i];
	    if (b.pmax()[i] > b_.pmax()[i])
	      b_.pmax()[i] = b.pmax()[i];
	  }
      }

      template <typename P>
      inline
      const box<P>&
      bbox<P>::to_result() const
      {
	//      mln_precondition(is_valid_);
	return b_;
      }

      template <typename P>
      inline
      bool
      bbox<P>::is_valid() const
      {
	return is_valid_;
      }

    } // end of namespace mln::accu::shape

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SHAPE_BBOX_HH
