// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ELEMENTARY_GRADIENT_INTERNAL_HH
# define MLN_MORPHO_ELEMENTARY_GRADIENT_INTERNAL_HH

/// \file
///
/// \todo Fix the extension issue (see todo in like_ero_fun and _set).

# include <mln/morpho/elementary/like_ero_fun.hh>
# include <mln/morpho/elementary/like_ero_set.hh>


namespace mln
{

  namespace morpho
  {

    namespace elementary
    {


      template <typename I, typename N>
      mln_concrete(I)
      gradient_internal(const Image<I>& input, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	struct f_grad_int
	{
	  template <typename V, typename A>
	  V operator()(const V& input_p, const A& a) const
	  {
	    return input_p - a.to_result();
	  }
	};

	// Dispatch.

	template <typename I, typename N>
	mln_concrete(I)
	gradient_internal_dispatch(trait::image::kind::any,
			 const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  return like_ero_fun(accu::meta::stat::min(), f_grad_int(), input, nbh);
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_internal_dispatch(trait::image::kind::logic,
				   const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  bool val[] =
	    {
	      1, // ext_value
	      0, // do_duplicate
	      1, // on_input_p
	      0, // on_input_n
	      1, // output_p
	    };
	  return like_ero_set(val, input, nbh);
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_internal_dispatch(const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  return gradient_internal_dispatch(mln_trait_image_kind(I)(),
					    input, nbh);
	}

      } // end of namespace mln::morpho::elementary::internal


      // Facade.

      template <typename I, typename N>
      mln_concrete(I)
      gradient_internal(const Image<I>& input, const Neighborhood<N>& nbh)
      {
	trace::entering("morpho::elementary::gradient_internal");

	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	mln_concrete(I) output = internal::gradient_internal_dispatch(input, nbh);

	mln_postcondition(output <= input);
	trace::exiting("morpho::elementary::gradient_internal");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::elementary

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ELEMENTARY_GRADIENT_INTERNAL_HH
