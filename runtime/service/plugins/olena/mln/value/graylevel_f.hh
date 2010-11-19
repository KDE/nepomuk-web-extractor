// Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_GRAYLEVEL_F_HH
# define MLN_VALUE_GRAYLEVEL_F_HH

/// \file
///
/// Definition of the mln::value::graylevel_f class.

# include <iostream>

# include <mln/value/ops.hh>

# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>
# include <mln/literal/ops.hh>

# include <mln/value/float01_f.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace literal
  {
    /// \{ Forward declarations.
    struct black_t;
    struct medium_gray_t;
    struct white_t;
    /// \}
  }
  namespace value
  {
    /// \{ Forward declarations.
    namespace internal {class gray_f; }
    struct graylevel_f;
    template <unsigned n> struct graylevel;
    struct float01_f;
    /// \}
  }



  namespace trait
  {

    template <>
    struct set_precise_binary_< op::plus, mln::value::graylevel_f, mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <unsigned n>
    struct set_precise_binary_< op::plus, mln::value::graylevel_f, mln::value::graylevel<n> >
    {
      typedef mln::value::internal::gray_f ret;
    };
    template <unsigned n>
    struct set_precise_binary_< op::plus, mln::value::graylevel<n>, mln::value::graylevel_f  >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <>
    struct set_precise_binary_< op::minus, mln::value::graylevel_f, mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <>
    struct set_precise_binary_< op::times, mln::value::graylevel_f, mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <>
    struct set_precise_binary_< op::div, mln::value::graylevel_f, mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < typename I >
    struct set_binary_< op::times,
			mln::value::Integer, mln::value::graylevel_f,
			mln::value::Integer, I >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < typename I >
    struct set_binary_< op::times,
			mln::value::Integer, I,
			mln::value::Integer, mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };


    template < typename F >
    struct set_binary_< op::times,
			mln::value::Integer,  mln::value::graylevel_f,
			mln::value::Floating, F >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < typename F >
    struct set_binary_< op::times,
			mln::value::Floating, F,
			mln::value::Integer,  mln::value::graylevel_f >
    {
      typedef mln::value::internal::gray_f ret;
    };


    template < typename S >
    struct set_precise_binary_< op::times, mln::value::graylevel_f, mln::value::scalar_<S> >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template < typename S >
    struct set_precise_binary_< op::div, mln::value::graylevel_f, mln::value::scalar_<S> >
    {
      typedef mln::value::internal::gray_f ret;
    };


    /// Forward declaration.
    template <typename T> struct value_;

    // 'graylevel_f' as a value.
    template <>
    struct value_<mln::value::graylevel_f>
    {
    private:
      typedef mln::value::graylevel_f self_;
      typedef mln::value::float01_f equiv_;

    public:

      enum {
	dim = 1,
	nbits = mln_nbits(equiv_),
	card  = 0
      };

      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::gray       kind;
      typedef mln_trait_value_quant_(equiv_)   quant;

      static const equiv_ min() { return 0; }
      static const equiv_ max() { return 1; }
      static const equiv_ epsilon() { return mln_epsilon(equiv_); }

      typedef float comp;

      typedef float sum;
    };

  } // end of namespace mln::trait


  namespace value
  {

    /// General gray-level class on n bits.
    struct graylevel_f
      :
      public Floating< graylevel_f >,

      public internal::value_like_< float01_f,          // Equivalent.
				    mln_enc_(float01_f),// Encoding.
				    internal::gray_f,   // Interoperation.
				    graylevel_f >       // Exact.
    {
      /// Constructor without argument.
      graylevel_f();
      /// Copy constructor.
      graylevel_f(const graylevel_f& rhs);
      /// Assigment.
      graylevel_f& operator=(const graylevel_f& rhs);


      /// Constructor from float.
      graylevel_f(float val);
      /// Assigment with float.
      graylevel_f& operator=(float val);

      /// Constructor from graylevel.
      template <unsigned n>
	graylevel_f(const graylevel<n>& rhs);
      /// Assigment with graylevel.
      template <unsigned n>
	graylevel_f& operator=(const graylevel<n>& rhs);

      /// Ctors with literals.
      /// \{
      graylevel_f(const mln::literal::black_t&);
      graylevel_f(const mln::literal::medium_gray_t&);
      graylevel_f(const mln::literal::white_t&);
      /// \}

      /// Assigment with literals.
      /// \{
      graylevel_f& operator=(const mln::literal::black_t&);
      graylevel_f& operator=(const mln::literal::medium_gray_t&);
      graylevel_f& operator=(const mln::literal::white_t&);
      /// \}

      /// Conversion to graylevel<n>.
      template <unsigned n>
	operator graylevel<n>() const;

      /// Access to std type.
      float value() const;
    };


    // Operators.

    /// Op<<.
    std::ostream& operator<<(std::ostream& ostr, const graylevel_f& g);

    // graylevel_f + graylevel_f.
    mln_trait_op_plus_(graylevel_f, graylevel_f)
      operator+(const graylevel_f& lhs, const graylevel_f& rhs);


    // graylevel_f + graylevel<n>.
    template <unsigned n>
    mln_trait_op_plus(graylevel_f, graylevel<n>)
      operator+(const graylevel_f& lhs, const graylevel<n>& rhs);
    // graylevel<n> + graylevel_f.
    template <unsigned n>
    mln_trait_op_plus(graylevel_f, graylevel<n>)
      operator+(const graylevel<n>& lhs, const graylevel_f& rhs);

    // graylevel_f - graylevel_f.
    mln_trait_op_minus_(graylevel_f, graylevel_f)
      operator-(const graylevel_f& lhs, const graylevel_f& rhs);

    // graylevel_f * graylevel_f.
    mln_trait_op_times_(graylevel_f, graylevel_f)
      operator*(const graylevel_f& lhs, const graylevel_f& rhs);


    // With Builtins.

    // graylevel_f * T.
    template <unsigned n, typename T>
    mln_trait_op_times(graylevel_f, T)
      operator*(const graylevel_f& lhs, const T& rhs);

    // T * graylevel_f.
    template <unsigned n, typename T>
    mln_trait_op_times(graylevel_f, T)
      operator*(const T& lhs, const graylevel_f& rhs);

    // graylevel_f / T.
    template <unsigned n, typename T>
    internal::gray_f
      operator/(const graylevel_f& lhs, const T& rhs);

    // With Integer.

    // graylevel_f * Integer<I>.
    template <typename I>
    mln_trait_op_times(graylevel_f, I)
      operator*(const graylevel_f& lhs, const Integer<I>& rhs);

    // Integer<I> * graylevel_f.
    template <typename I>
    mln_trait_op_times(I, graylevel_f)
      operator*(const Integer<I>& lhs, const graylevel_f& rhs);

    // graylevel_f / Integer<I>.
    template <typename I>
    mln_trait_op_div(graylevel_f, I)
      operator/(const graylevel_f& lhs, const Integer<I>& rhs);

    // Integer<I> / graylevel_f.
    template <typename I>
    mln_trait_op_div(I, graylevel_f)
      operator/(const Integer<I>& lhs, const graylevel_f& rhs);

    // With Floating.

    // graylevel_f * Floating<F>.
    template <typename F>
    mln_trait_op_times(graylevel_f, F)
      operator*(const graylevel_f& lhs, const Floating<F>& rhs);

    // Floating<F>, graylevel_f.
    template <typename F>
    mln_trait_op_times(F, graylevel_f)
      operator*(const Floating<F>& lhs, const graylevel_f& rhs);


    // graylevel_f / Floating<F>.
    template <typename F>
    mln_trait_op_div(graylevel_f, F)
      operator/(const graylevel_f& lhs, const Floating<F>& rhs);

    // Floating<F> / graylevel_f.
    template <typename F>
    mln_trait_op_div(F, graylevel_f)
      operator/(const Floating<F>& lhs, const graylevel_f& rhs);


# ifndef MLN_INCLUDE_ONLY

    // graylevel_f.

    inline
    graylevel_f::graylevel_f()
    {
    }


    inline
    graylevel_f::graylevel_f(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
      return *this;
    }

    template <unsigned n>
    graylevel_f::graylevel_f(const graylevel<n>& rhs)
    {
      mln_precondition(rhs.to_float() >= 0);
      mln_precondition(rhs.to_float() <= 1);
      this->v_ = rhs.to_float();
    }

    template <unsigned n>
    graylevel_f&
    graylevel_f::operator=(const graylevel<n>& rhs)
    {
      mln_precondition(rhs.to_float() >= 0);
      mln_precondition(rhs.to_float() <= 1);
      this->v_ = rhs.to_float();
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const graylevel_f& rhs)
      : Floating<graylevel_f>()
    {
      this->v_ = rhs.v_;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const graylevel_f& rhs)
    {
      mln_precondition(rhs.v_ >= 0);
      mln_precondition(rhs.v_ <= 1);
      this->v_ = rhs.v_;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::black_t&)
    {
      this->v_ = 0.0f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::black_t&)
    {
      this->v_ = 0.0f;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::medium_gray_t&)
    {
      this->v_ = 0.5f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::medium_gray_t&)
    {
      this->v_ = 0.5f;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::white_t&)
    {
      this->v_ = 1.0f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::white_t&)
    {
      this->v_ = 1.0f;
      return *this;
    }

    template <unsigned n>
    inline
    graylevel_f::operator graylevel<n>() const
    {
      return graylevel<n>(internal::gray_f(*this));
    }

    inline
    float
    graylevel_f::value() const
    {
      return this->v_;
    }

    // Operators.

    inline
    std::ostream& operator<<(std::ostream& ostr, const graylevel_f& g)
    {
      return ostr << g.value() << "/gl_f"; // FIXME: Be more explicit!.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


//# include <mln/value/internal/gray_f.hh>
///# include <mln/value/internal/gray_.hh>

#endif // ! MLN_VALUE_GRAYLEVEL_F_HH
