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

#ifndef MLN_ARITH_PLUS_HH
# define MLN_ARITH_PLUS_HH

/// \file
///
/// Point-wise addition between images.
///
/// \todo Speedup; some versions are not optimal.

# include <mln/arith/includes.hh>


namespace mln
{


  namespace trait
  {

    template <typename L, typename R>
    struct set_binary_< op::plus, Image, L, Image, R >
    {
      typedef mln_trait_op_plus(mln_value(L), mln_value(R)) value;
      typedef mln_ch_value(L, value) ret;
    };

    template <typename I, typename S>
    struct set_binary_< op::plus, Image, I, mln::value::Scalar, S >
    {
      typedef mln_trait_op_plus(mln_value(I), S) value;
      typedef mln_ch_value(I, value) ret;
    };

  } // end of namespace mln::trait



  template <typename L, typename R>
  mln_trait_op_plus(L,R)
  operator+(const Image<L>& lhs, const Image<R>& rhs);


  template <typename L, typename R>
  L&
  operator+=(Image<L>& lhs, const Image<R>& rhs);


  template <typename I, typename S>
  mln_trait_op_plus(I,S)
  operator+(const Image<I>& ima, const value::Scalar<S>& s);


  template <typename I, typename S>
  I&
  operator+=(Image<I>& ima, const value::Scalar<S>& s);



  namespace arith
  {

    /// Point-wise addition of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R>
    mln_trait_op_plus(L, R)
      plus(const Image<L>& lhs, const Image<R>& rhs);


    /// Point-wise addition of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \param[in] f   Function.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R, typename F>
    mln_ch_value(L, mln_result(F))
      plus(const Image<L>& lhs, const Image<R>& rhs, const Function_v2v<F>& f);


    /// Point-wise addition of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * The free parameter \c V sets the destination value type.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename V, typename L, typename R>
    mln_ch_value(L, V)
      plus(const Image<L>& lhs, const Image<R>& rhs);


    /// Point-wise addition of image \p rhs in image \p lhs.
    /*!
     * \param[in,out] lhs First operand image (subject to addition).
     * \param[in] rhs Second operand image (to be added to \p lhs).
     *
     * This addition performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) += rhs(p)
     *
     * \pre \p rhs.domain == \p lhs.domain
     */
    template <typename L, typename R>
    void
    plus_inplace(Image<L>& lhs, const Image<R>& rhs);


    /// Point-wise addition of the value \p val to image \p input.
    /*!
     * \param[in] input The image.
     * \param[in] val The value.
     * \result The result image.
     *
     * \pre \p input.is_valid
     */
    template <typename I, typename V>
    mln_trait_op_plus(I, V)
      plus_cst(const Image<I>& input, const V& val);


    /// Point-wise addition of the value \p val to image \p input.
    /*!
     * \param[in] input The image.
     * \param[in] val The value.
     * \param[in] f   Function.
     * \result The result image.
     *
     * \pre \p input.is_valid
     */
    template <typename I, typename V, typename F>
    mln_ch_value(I, mln_result(F))
      plus_cst(const Image<I>& input, const V& val, const Function_v2v<F>& f);


    /// Point-wise addition of the value \p val to image \p input.
    /*!
     * \param[in] input The image.
     * \param[in] val The value.
     * \result The result image.
     *
     * \pre \p input.is_valid
     */
    template <typename W, typename I, typename V>
    mln_ch_value(I, W)
      plus_cst(const Image<I>& input, const V& val);


    /// Point-wise addition of the value \p val to image \p input.
    /*!
     * \param[in,out] input The image.
     * \param[in] val The value.
     *
     * \pre \p input.is_valid
     */
    template <typename I, typename V>
    I&
    plus_cst_inplace(Image<I>& input, const V& val);


  } // end of namespace mln::arith




# ifndef MLN_INCLUDE_ONLY


  template <typename L, typename R>
  inline
  mln_trait_op_plus(L,R)
  operator+(const Image<L>& lhs, const Image<R>& rhs)
  {
    trace::entering("operator::plus");

    mln_precondition(exact(rhs).domain() == exact(lhs).domain());

    mln_trait_op_plus(L,R) output = arith::plus(lhs, rhs);

    trace::exiting("operator::plus");
    return output;
  }

  template <typename L, typename R>
  inline
  L&
  operator+=(Image<L>& lhs, const Image<R>& rhs)
  {
    trace::entering("operator::plus_eq");

    mln_precondition(exact(rhs).domain() == exact(lhs).domain());

    arith::plus_inplace(lhs, rhs);

    trace::exiting("operator::plus_eq");
    return exact(lhs);
  }


  template <typename I, typename S>
  inline
  mln_trait_op_plus(I,S)
  operator+(const Image<I>& ima, const value::Scalar<S>& s)
  {
    trace::entering("operator::plus");

    mln_precondition(exact(ima).is_valid());

    mln_trait_op_plus(I,S) output = arith::plus_cst(ima, exact(s));

    trace::exiting("operator::plus");
    return output;
  }

  template <typename I, typename S>
  inline
  I&
  operator+=(Image<I>& ima, const value::Scalar<S>& s)
  {
    trace::entering("operator::plus_eq");

    mln_precondition(exact(ima).is_valid());

    arith::plus_cst_inplace(ima, exact(s));

    trace::exiting("operator::plus_eq");
    return exact(ima);
  }



  namespace arith
  {

    namespace impl
    {

      template <typename L, typename R, typename O>
      inline
      void plus_(trait::image::speed::any, const L& lhs,
		 trait::image::speed::any, const R& rhs, O& output)
      {
	mln_piter(L) p(lhs.domain());
	for_all(p)
	  output(p) = lhs(p) + rhs(p);
      }

      template <typename L, typename R, typename F, typename O>
      inline
      void plus_(trait::image::speed::any, const L& lhs,
		 trait::image::speed::any, const R& rhs, const F& f, O& output)
      {
	mln_piter(L) p(lhs.domain());
	for_all(p)
	  output(p) = f(lhs(p) + rhs(p));
      }

      template <typename L, typename R, typename O>
      inline
      void plus_(trait::image::speed::fastest, const L& lhs,
		 trait::image::speed::fastest, const R& rhs, O& output)
      {
	mln_pixter(const L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	mln_pixter(O)       op(output);
	for_all_3(lp, rp, op)
	  op.val() = lp.val() + rp.val();
      }

      template <typename L, typename R, typename F, typename O>
      inline
      void plus_(trait::image::speed::fastest, const L& lhs,
		 trait::image::speed::fastest, const R& rhs, const F& f, O& output)
      {
	mln_pixter(const L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	mln_pixter(O)       op(output);
	for_all_3(lp, rp, op)
	  op.val() = f(lp.val() + rp.val());
      }

      template <typename L, typename R>
      inline
      void plus_inplace_(trait::image::speed::any, L& lhs,
			 trait::image::speed::any, const R& rhs)
      {
	mln_piter(L) p(lhs.domain());
	for_all(p)
	  lhs(p) += rhs(p);
      }

      template <typename L, typename R>
      inline
      void plus_inplace_(trait::image::speed::fastest, L& lhs,
			 trait::image::speed::fastest, const R& rhs)
      {
	mln_pixter(L) lp(lhs);
	mln_pixter(const R) rp(rhs);
	for_all_2(lp, rp)
	  lp.val() += rp.val();
      }

    } // end of namespace mln::arith::impl


    // Facades.


    template <typename L, typename R>
    inline
    mln_trait_op_plus(L, R)
      plus(const Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("arith::plus");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_trait_op_plus(L, R) output;
      initialize(output, lhs);
      impl::plus_(mln_trait_image_speed(L)(), exact(lhs),
		  mln_trait_image_speed(R)(), exact(rhs), output);

      trace::exiting("arith::plus");
      return output;
    }


    template <typename L, typename R, typename F>
    inline
    mln_ch_value(L, mln_result(F))
      plus(const Image<L>& lhs, const Image<R>& rhs, const Function_v2v<F>& f)
    {
      trace::entering("arith::plus");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_ch_value(L, mln_result(F)) output;
      initialize(output, lhs);
      impl::plus_(mln_trait_image_speed(L)(), exact(lhs),
		  mln_trait_image_speed(R)(), exact(rhs), exact(f), output);

      trace::exiting("arith::plus");
      return output;
    }


    template <typename V, typename L, typename R>
    inline
    mln_ch_value(L, V)
      plus(const Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("arith::plus");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      // Calls the previous version.
      mln_ch_value(L, V) output = plus(lhs, rhs,
				       mln::fun::v2v::cast<V>());

      trace::exiting("arith::plus");
      return output;
    }


    template <typename I, typename V>
    inline
    mln_trait_op_plus(I, V)
      plus_cst(const Image<I>& input, const V& val)
    {
      trace::entering("arith::plus_cst");

      mln_precondition(exact(input).is_valid());

      // Calls the previous version.
      mln_trait_op_plus(I, V) output = plus(input,
					    pw::cst(val) | exact(input).domain());

      trace::exiting("arith::plus_cst");
      return output;
    }


    template <typename I, typename V, typename F>
    inline
    mln_ch_value(I, mln_result(F))
      plus_cst(const Image<I>& input, const V& val, const Function_v2v<F>& f)
    {
      trace::entering("arith::plus_cst");

      mln_precondition(exact(input).is_valid());

      // Calls the previous version.
      mln_ch_value(I, mln_result(F)) output = plus(input,
						   pw::cst(val) | exact(input).domain(),
						   f);

      trace::exiting("arith::plus_cst");
      return output;
    }


    template <typename W, typename I, typename V>
    inline
    mln_ch_value(I, W)
      plus_cst(const Image<I>& input, const V& val)
    {
      trace::entering("arith::plus_cst");

      mln_precondition(exact(input).is_valid());

      // Calls the previous version.
      mln_ch_value(I, W) output = plus_cst(input, val,
					   mln::fun::v2v::cast<W>());

      trace::exiting("arith::plus_cst");
      return output;
    }


    template <typename L, typename R>
    inline
    void
    plus_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("arith::plus_inplace");

      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      impl::plus_inplace_(mln_trait_image_speed(L)(), exact(lhs),
			  mln_trait_image_speed(R)(), exact(rhs));

      trace::exiting("arith::plus_inplace");
    }


    template <typename I, typename V>
    inline
    I&
    plus_cst_inplace(Image<I>& input, const V& val)
    {
      trace::entering("arith::plus_cst_inplace");

      mln_precondition(exact(input).is_valid());

      // Calls the previous version.
      plus_inplace(input,
		   pw::cst(val) | exact(input).domain());

      trace::exiting("arith::plus_cst_inplace");
      return exact(input);
    }

  } // end of namespace mln::arith

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_ARITH_PLUS_HH
