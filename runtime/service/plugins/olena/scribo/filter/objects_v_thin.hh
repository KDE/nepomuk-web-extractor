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

#ifndef SCRIBO_FILTER_OBJECTS_V_THIN_HH
# define SCRIBO_FILTER_OBJECTS_V_THIN_HH

/// \file
///
/// Remove too thin components.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <scribo/core/component_set.hh>
# include <scribo/filter/internal/compute.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /// Remove components thinner or equal to \p min_thinness.
    ///
    /// \param[in] input_ a binary image.
    /// \param[in] nbh_ a neighborhood used in labeling algorithms.
    /// \param[in] label_type the label type used for labeling.
    /// \param[in] min_thinness the minimum thinness value.
    ///
    /// \result A binary image without v_thin components.
    //
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    objects_v_thin(const Image<I>& input_,
		   const Neighborhood<N>& nbh_,
		   const V& label_type,
		   unsigned min_thinness);

    /// Remove lines of text thinner or equal to \p min_thinness.
    ///
    /// \param[in] comps A component set.
    /// \param[in] min_thinness the minimum thinness value.
    ///
    /// \result An object image without too thin vertical components.
    //
    template <typename L>
    inline
    component_set<L>
    components_v_thin(const component_set<L>& text,
		      unsigned min_thinness);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      /// Filter Functor.
      /// Return false for all objects which are too large.
      template <typename L>
      struct objects_v_thin_filter
	: Function_v2b< objects_v_thin_filter<L> >
      {
	typedef accu::shape::bbox<mln_psite(L)> box_accu_t;

	/// Constructor
	///
	/// \param[in] comps A component set.
	/// \param[in] min_thinness the minimum of vertical thinness
	/// allowed.
	//
	objects_v_thin_filter(const component_set<L>& comps,
			      unsigned min_thinness)
	  : comps_(comps), min_thinness_(min_thinness)
	{
	}


	/// Constructor
	///
	/// \param[in] min_thinness the maximum thinness allowed.
	//
	objects_v_thin_filter(unsigned min_thinness)
	  : min_thinness_(min_thinness)
	{
	}

	/// Set the underlying component set.
	//
	void update_objects(const component_set<L>& comps)
	{
	  comps_ = comps;
	}


	/// Return false if the components is thinner than
	/// \p min_thinness_.
	///
	/// \param[in] l An image value.
	//
	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return false;
	  return comps_.bbox(l).nrows() > min_thinness_;
	}

	/// Component bounding boxes.
	component_set<L> comps_;

	/// The minimum vertical thinness.
	unsigned min_thinness_;
      };


    } //  end of namespace scribo::filter::internal


    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    objects_v_thin(const Image<I>& input_,
		   const Neighborhood<N>& nbh_,
		   const V& label_type,
		   unsigned min_thinness)
    {
      trace::entering("scribo::filter::objects_v_thin");

      (void) label_type;
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      internal::objects_v_thin_filter<V> fv2b(min_thinness);
      mln_concrete(I)
	output = internal::compute(input, nbh, label_type, fv2b);

      trace::exiting("scribo::filter::objects_v_thin");
      return output;
    }


    template <typename L>
    inline
    component_set<L>
    objects_v_thin(const component_set<L>& comps,
		   unsigned min_thinness)
    {
      trace::entering("scribo::filter::objects_v_thin");

      internal::objects_v_thin_filter<L>
	is_not_too_v_thin(comps, min_thinness);
      component_set<L> output = internal::compute(comps, is_not_too_v_thin);

      trace::exiting("scribo::filter::objects_v_thin");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_V_THIN_HH
