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

#ifndef MLN_CORE_BOX_RUNSTART_PITER_HH
# define MLN_CORE_BOX_RUNSTART_PITER_HH

/// \file
///
/// Definition of iterators on points by lines.

# include <mln/core/internal/site_iterator_base.hh>
# include <mln/core/site_set/box.hh>


#define mln_box_runstart_piter(I)  typename mln::box_runstart_piter<mln_psite(I)>
#define mln_box_runstart_piter_(I)          mln::box_runstart_piter<mln_psite_(I)>


namespace mln
{

  /*! \brief A generic forward iterator on points by lines.
   *
   * The parameter \c P is the type of points.
   */
  template <typename P>
  class box_runstart_piter :
    public internal::site_set_iterator_base< box<P>,
					     box_runstart_piter<P> >
  {
    typedef box_runstart_piter<P> self_;
    typedef internal::site_set_iterator_base< box<P>, self_ > super_;
  public:

    // Make definitions from super class available.
    enum { dim = super_::dim };

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    box_runstart_piter(const box<P>& b);

    /// Delayed initialization.
    void init_(const box<P>& b);

    box_runstart_piter();

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Give the lenght of the run
    unsigned run_length() const;

  private:
    using super_::p_;
    using super_::s_;
  };


# ifndef MLN_INCLUDE_ONLY


  // box_runstart_piter<P>

  template <typename P>
  inline
  box_runstart_piter<P>::box_runstart_piter()
  {
  }

  template <typename P>
  inline
  box_runstart_piter<P>::box_runstart_piter(const box<P>& b)
  {
    init_(b);
  }

  template <typename P>
  inline
  void
  box_runstart_piter<P>::init_(const box<P>& b)
  {
    this->change_target(b);
  }


  template <typename P>
  inline
  bool
  box_runstart_piter<P>::is_valid_() const
  {
    return p_[0] != static_cast<mln_coord(P)>(s_->pmax()[0] + 1);
  }

  template <typename P>
  inline
  void
  box_runstart_piter<P>::invalidate_()
  {
    p_[0] = static_cast<mln_coord(P)>(s_->pmax()[0] + 1);
  }

  template <typename P>
  inline
  void
  box_runstart_piter<P>::start_()
  {
    p_ = s_->pmin();
  }

  template <typename P>
  inline
  void
  box_runstart_piter<P>::next_()
  {
    // Do we want this run for image in 3d?
    for (int c = dim - 2; c >= 0; --c)
    {
      if (p_[c] != s_->pmax()[c])
      {
	++p_[c];
	break;
      }
      else
	p_[c] = s_->pmin()[c];
    }

    if (p_ == s_->pmin())
      invalidate_();
  }

  template <typename P>
  inline
  unsigned
  box_runstart_piter<P>::run_length() const
  {
    return s_->len(dim - 1);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_BOX_RUNSTART_PITER_HH
