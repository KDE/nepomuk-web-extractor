// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_CORE_COMPONENT_SET_HH
# define SCRIBO_CORE_COMPONENT_SET_HH

/// \file
///
/// \brief Definition of a component set.

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/function.hh>

# include <mln/data/fill.hh>

# include <mln/util/array.hh>
# include <mln/value/next.hh>

# include <mln/accu/pair.hh>
# include <mln/accu/center.hh>
# include <mln/accu/shape/bbox.hh>

# include <mln/labeling/compute.hh>
# include <mln/labeling/relabel.hh>

# include <mln/convert/from_to.hh>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/core/routine/duplicate.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_info.hh>


namespace scribo
{

  // Forward declaration.
  template <typename L> struct component_set;


  namespace internal
  {
    /// Data structure for \c scribo::component_set<I>.
    template <typename L>
    struct component_set_data
    {
      typedef mln::accu::center<mln_site(L)> center_accu_t;
      typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
      typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;

      typedef std::pair<mln_box(L),
			std::pair<mln_site(L), unsigned> > pair_data_t;

      typedef mln_result(center_accu_t) center_t;

      component_set_data();
      component_set_data(const L& ima, const mln_value(L)& ncomps);
      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<pair_accu_t>& attribs);
      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<pair_data_t>& attribs);

      component_set_data(const L& ima, const mln_value(L)& ncomps,
			 const mln::util::array<scribo::component_info>& infos);

      void fill_infos(const mln::util::array<pair_accu_t>& attribs);

      void fill_infos(const mln::util::array<pair_data_t>& attribs);


      L ima_;
      mln_value(L) ncomps_;
      mln::util::array<scribo::component_info> infos_;

      mln_ch_value(L, bool) separators_;
    };

  } // end of namespace scribo::internal


  template <typename L>
  class component_set
  {
    typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
    typedef mln::accu::center<mln_site(L)> center_accu_t;
    typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;

    typedef std::pair<mln_box(L),
		      std::pair<mln_site(L), unsigned> > pair_data_t;

    typedef mln_result(center_accu_t) center_t;

    typedef internal::component_set_data<L> data_t;
    typedef data_t* id_t;

  public:

    /// Constructors
    /// @{
    /// Constructor without argument.
    component_set();

    /// Constructor from an image \p ima and the number of labels \p ncomps.
    component_set(const L& ima, const mln_value(L)& ncomps);

    /// Constructor from an image \p ima, the number of labels \p ncomps and
    /// attributes values (bounding box and mass center).
    component_set(const L& ima, const mln_value(L)& ncomps,
		  const mln::util::array<pair_accu_t>& attribs);

    component_set(const L& ima, const mln_value(L)& ncomps,
		  const mln::util::array<pair_data_t>& attribs);
    /// @}

    /// Return the component count.
    mln_value(L) nelements() const;

    /// Return component information for a given component id \p id.
    const component_info& info(const mln_value(L)& id) const;

    /// Return component information for a given component id \p id.
    component_info& info(const mln_value(L)& id);

    /// Return component information for a given component id \p id.
    component_info& operator()(const component_id_t& id);

    /// Return component information for a given component id \p id.
    const component_info& operator()(const component_id_t& id) const;


    /// Update tag of components set to 'false' in \p f with \p tag.
    template <typename F>
    void update_tags(const mln::Function_v2b<F>& f, component::Tag tag);

    /// Create a copy of this component_set<L>
    component_set<L> duplicate() const;

    /// Return the underlying labeled image
    const L& labeled_image() const;

    /// Is this component set valid?
    bool is_valid() const;


    /// Separators components related routines.
    /// @{

    /// Return true if an image of separator exists.
    bool has_separators() const;

    /// Add separators in the underlying labeled image.
    void add_separators(const mln_ch_value(L, bool)& ima);

    /// Return the Boolean image of separators.
    const mln_ch_value(L, bool)& separators() const;

    /// Remove any existing separators.
    void clear_separators();

    /// @}



    /// Internal methods
    /// @{

    /// Return all the component infos.
    const mln::util::array<scribo::component_info>& infos_() const;

    /// Unique set Id.
    id_t id_() const;


    /// Read/Write access to the underlying labeled image.
    /// Careful! Write in this image at your own risks! It may lead to
    /// non-synchronised related data.
    //
    L& labeled_image_();

    /// Return the underlying labeled image where invalid components
    /// have been erased.
    ///
    /// WARNING: this image is computed on the fly...!
    //
    mln_concrete(L) valid_comps_image_() const;

    /// @}

  private:
    /// Duplicate the underlying image and create a new component_set.
    void init_(const component_set<L>& model);

    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  bool
  operator==(const component_set<L>& lhs, const component_set<L>& rhs);


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_set<L>& info);




# ifndef MLN_INCLUDE_ONLY


  // component_set_data<L> >

  namespace internal
  {

    // data< component_set<L> >


    template <typename L>
    inline
    component_set_data<L>::component_set_data()
    {
    }


    template <typename L>
    inline
    component_set_data<L>::component_set_data(const L& ima,
					      const mln_value(L)& ncomps)
      : ima_(ima), ncomps_(ncomps)
    {
      initialize(separators_, ima); // FIXME: do we really want that?
      mln::data::fill(separators_, false);

      typedef mln::accu::shape::bbox<mln_site(L)> bbox_accu_t;
      typedef mln::accu::center<mln_site(L)> center_accu_t;
      typedef mln::accu::pair<bbox_accu_t, center_accu_t> pair_accu_t;


      mln::util::array<pair_accu_t> attribs;
      mln::labeling::compute(attribs, ima_, ncomps_);

      fill_infos(attribs);
    }


    template <typename L>
    inline
    component_set_data<L>::component_set_data(const L& ima,
					      const mln_value(L)& ncomps,
					      const mln::util::array<pair_accu_t>& attribs)
      : ima_(ima), ncomps_(ncomps)
    {
      initialize(separators_, ima);  // FIXME: do we really want that?
      mln::data::fill(separators_, false);

      fill_infos(attribs);
    }

    template <typename L>
    inline
    component_set_data<L>::component_set_data(const L& ima,
					      const mln_value(L)& ncomps,
					      const mln::util::array<pair_data_t>& attribs)
      : ima_(ima), ncomps_(ncomps)
    {
      initialize(separators_, ima);  // FIXME: do we really want that?
      mln::data::fill(separators_, false);

      fill_infos(attribs);
    }

    template <typename L>
    inline
    component_set_data<L>::component_set_data(const L& ima,
					      const mln_value(L)& ncomps,
					      const mln::util::array<scribo::component_info>& infos)
      : ima_(ima), ncomps_(ncomps), infos_(infos)
    {
      initialize(separators_, ima); // FIXME: do we really want that?
      mln::data::fill(separators_, false);
    }


    template <typename L>
    inline
    void
    component_set_data<L>::fill_infos(const mln::util::array<pair_accu_t>& attribs)
    {
      typedef mln_site(L) P;

      infos_.reserve(mln::value::next(ncomps_));

      infos_.append(component_info()); // Component 0, i.e. the background.
      for_all_comp_data(i, attribs)
      {
 	component_info info(i, attribs[i].first(),
			    attribs[i].second(), attribs[i].second_accu().nsites());
 	infos_.append(info);
      }
    }

    template <typename L>
    inline
    void
    component_set_data<L>::fill_infos(const mln::util::array<pair_data_t>& attribs)
    {
      typedef mln_site(L) P;

      infos_.reserve(mln::value::next(ncomps_));

      infos_.append(component_info()); // Component 0, i.e. the background.
      for_all_comp_data(i, attribs)
      {
 	component_info info(i, attribs[i].first,
			    attribs[i].second.first, attribs[i].second.second);
 	infos_.append(info);
      }
    }


  } // end of namespace mln::internal




  template <typename L>
  inline
  component_set<L>::component_set()
  {
  }

  template <typename L>
  inline
  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps);
  }


  template <typename L>
  inline
  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps,
				  const mln::util::array<pair_accu_t>& attribs)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps, attribs);
  }


  template <typename L>
  inline

  component_set<L>::component_set(const L& ima, const mln_value(L)& ncomps,
				  const mln::util::array<pair_data_t>& attribs)
  {
    data_ = new internal::component_set_data<L>(ima, ncomps, attribs);
  }


  template <typename L>
  inline

  mln_value(L)
  component_set<L>::nelements() const
  {
    return data_->ncomps_;
  }

  template <typename L>
  inline
  const component_info&
  component_set<L>::info(const mln_value(L)& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  component_info&
  component_set<L>::info(const mln_value(L)& id)
  {
    return this->data_->infos_[id];
  }

//   template <typename L>
//   inline
//   const component_info&
//   component_set<L>::operator()(const mln_value(L)& id) const
//   {
//     return this->data_->infos_[id];
//   }

//   template <typename L>
//   inline
//   component_info&
//   component_set<L>::operator()(const mln_value(L)& id)
//   {
//     return this->data_->infos_[id];
//   }

  template <typename L>
  inline
  const component_info&
  component_set<L>::operator()(const component_id_t& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  component_info&
  component_set<L>::operator()(const component_id_t& id)
  {
    return this->data_->infos_[id];
  }


  template <typename L>
  template <typename F>
  inline
  void
  component_set<L>::update_tags(const mln::Function_v2b<F>& f_,
				component::Tag tag)
  {
    const F& f = exact(f_);

    for_all_comp_data(i, data_->infos_)
      if (!f(i))
	data_->infos_[i].update_tag(tag);
  }


  template <typename L>
  inline
  component_set<L>
  component_set<L>::duplicate() const
  {
    component_set<L> output;
    output.init_(*this);
    return output;
  }



  template <typename L>
  inline
  const L&
  component_set<L>::labeled_image() const
  {
    return this->data_->ima_;
  }


  template <typename L>
  inline
  bool
  component_set<L>::is_valid() const
  {
    return this->data_->ima_.is_valid();
  }


  template <typename L>
  inline
  typename component_set<L>::id_t
  component_set<L>::id_() const
  {
    return data_.ptr_;
  }


  template <typename L>
  inline
  L&
  component_set<L>::labeled_image_()
  {
    return this->data_->ima_;
  }


  template <typename L>
  inline
  mln_concrete(L)
  component_set<L>::valid_comps_image_() const
  {
    mln::util::array<bool> f(mln::value::next(this->data_->ncomps_));
    f(0) = true;

    for_all_comps(c, (*this))
      f(c) = (*this)(c).is_valid();

    mln_value(L) new_ncomps;
    mln_concrete(L)
      output = mln::labeling::relabel(this->data_->ima_,
				      this->data_->ncomps_,
				      new_ncomps,
				      f);

    return output;
  }


  template <typename L>
  inline
  bool
  component_set<L>::has_separators() const
  {
    return this->data_->separators_.is_valid();
  }


  template <typename L>
  inline
  void
  component_set<L>::add_separators(const mln_ch_value(L, bool)& ima)
  {
    this->data_->separators_ = ima;
  }


  template <typename L>
  inline
  const mln_ch_value(L, bool)&
  component_set<L>::separators() const
  {
    return this->data_->separators_;
  }


  template <typename L>
  inline
  void
  component_set<L>::clear_separators()
  {
    this->data_->separators_.destroy();
  }


  template <typename L>
  inline
  const mln::util::array<scribo::component_info>&
  component_set<L>::infos_() const
  {
    return data_->infos_;
  }

  template <typename L>
  inline
  void
  component_set<L>::init_(const component_set<L>& set)
  {
    data_ = new internal::component_set_data<L>();
    data_->ima_ = mln::duplicate(set.labeled_image());
    data_->ncomps_ = set.nelements();
    data_->infos_ = set.infos_();
    data_->separators_ = set.separators();
  }


  template <typename L>
  bool
  operator==(const component_set<L>& lhs, const component_set<L>& rhs)
  {
    return lhs.id_() == rhs.id_();
  }

  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const component_set<L>& info)
  {
    ostr << "component_set[" << std::endl;
    for_all_comps(i, info)
      ostr << info(i);
    ostr << "]" << std::endl;

    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_COMPONENT_SET_HH
