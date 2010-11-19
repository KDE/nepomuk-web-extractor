// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_VALUE_OPS_HH
# define MLN_VALUE_OPS_HH

/// \file
///
/// Definitions of operators for value types.

# include <mln/trait/op/all.hh>
# include <mln/value/builtin/all.hh>
# include <mln/value/concept/all.hh>
# include <mln/value/equiv.hh>
# include <mln/trait/value_.hh>
# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>
# include <mln/literal/ops.hh>
# include <mln/metal/ret.hh>


/// Type trait for value sum.
# define mln_trait_value_sum_product(T, U)			\
   typename mln::trait::value_< mln_trait_op_times(T,U) >::sum
# define mln_trait_value_sum_product_(T, U)			\
   mln::trait::value_< mln_trait_op_times_(T,U) >::sum


/// Shortcut for type trait for value sum.
# define mln_sum_product(T, U)  mln_trait_value_sum_product(T, U)
# define mln_sum_product_(T, U)  mln_trait_value_sum_product_(T, U)


// FIXME: In the definitions below, is that equiv or interop?



namespace mln
{

  namespace trait
  {

    // Unary traits for any Scalar type.

    template < template <class> class Name,
	       typename V >
    struct set_unary_< Name, mln::value::Scalar, V >
    {
      typedef mln_trait_unary(Name, mln_value_equiv(V)) ret;
    };


    // Binary traits for any Scalar type...

    template < template <class, class> class Name,
	       typename Vl, typename Vr >
    struct set_binary_< Name, mln::value::Scalar, Vl, mln::value::Scalar, Vr >
    {
      typedef mln_trait_binary(Name, mln_value_equiv(Vl), mln_value_equiv(Vr)) ret;
    };

    // ...and for the special case of a couple of value::scalar_.

    template < template <class, class> class Name,
	       typename Sl, typename Sr >
    struct set_binary_< Name,
			mln::value::Scalar, mln::value::scalar_<Sl>,
			mln::value::Scalar, mln::value::scalar_<Sr> >
    {
      typedef mln_trait_binary(Name, mln_value_equiv(Sl), mln_value_equiv(Sr)) ret;
    };


    template < template <class, class> class Name,
	       typename S, typename L >
    struct set_binary_< Name,
			mln::value::Scalar, S,
			mln::Literal,       L >
    {
      typedef mln_value_equiv(S) S_;
      typedef mln_trait_binary(Name, S_, S_) ret;
    };

    template < template <class, class> class Name,
	       typename L, typename S >
    struct set_binary_< Name,
			mln::Literal,       L,
			mln::value::Scalar, S >
    {
      typedef mln_value_equiv(S) S_;
      typedef mln_trait_binary(Name, S_, S_) ret;
    };


    // Some binary traits for "scalar(s) OP obj" when OP commutes => "obj OP scalar(s)".

    template < typename S, typename O >
    struct set_binary_< op::plus,
			mln::value::Scalar, mln::value::scalar_<S>,
			mln::Object,        O >
    {
      typedef mln_trait_op_plus(O, mln::value::scalar_<S>) ret;
    };

    template < typename S, typename O >
    struct set_binary_< op::times,
			mln::value::Scalar, mln::value::scalar_<S>,
			mln::Object,        O >
    {
      typedef mln_trait_op_times(O, mln::value::scalar_<S>) ret;
    };

  } // end of namespace mln::trait


  // Arithmetical binary operators.

  template <typename Vl, typename Vr>
  mln_trait_op_plus(Vl, Vr)
    operator + (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_minus(Vl, Vr)
    operator - (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_times(Vl, Vr)
    operator * (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_div(Vl, Vr)
    operator / (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);

  template <typename Vl, typename Vr>
  mln_trait_op_mod(Vl, Vr)
    operator % (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs);


  // Swap arguments so that "scalar_ OP Object" is "Object OP
  // scalar_".  As a consequence, the user only has to define what
  // happens with a scalar as a rhs.

  template <typename S, typename O>
  mln_trait_op_plus(O, value::scalar_<S>)
    operator + (const value::scalar_<S>& lhs, const Object<O>& rhs);

  template <typename S, typename O>
  mln_trait_op_times(O, value::scalar_<S>)
    operator * (const value::scalar_<S>& lhs, const Object<O>& rhs);


  // Arithmetical unary operators.

  template <typename S>
  mln_trait_op_uminus(S)
    operator - (const value::scalar_<S>& rhs); // Overload of op-(Object) in core/ops.
  // FIXME: It is dedicated to value::scalar_ so move elsewhere?



  // Logical operators.

  // FIXME: ...



  // Case of value::scalar_ OP value::scalar_.

  template <typename Sl, typename Sr>
  mln_trait_op_plus(Sl, Sr)
    operator + (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_minus(Sl, Sr)
    operator - (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_times(Sl, Sr)
    operator * (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_div(Sl, Sr)
    operator / (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename Sl, typename Sr>
  mln_trait_op_mod(Sl, Sr)
    operator % (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);


  // Operator ==.

  template <typename Sl, typename Sr>
  mln_trait_op_eq(Sl, Sr)
  operator == (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  bool
  operator == (const value::scalar_<int>& lhs, const value::scalar_<unsigned>& rhs);

  bool
  operator == (const value::scalar_<unsigned>& lhs, const value::scalar_<int>& rhs);

  template <typename O, typename L>
  mln_trait_op_eq(O, O)
  operator==(const value::scalar_<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_eq(O, O)
  operator==(const Literal<L>& lhs, const value::scalar_<O>& rhs);


  // Operator <.

  template <typename Sl, typename Sr>
  mln_trait_op_less(Sl, Sr)
  operator < (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs);

  template <typename O, typename L>
  mln_trait_op_less(O, O)
  operator < (const value::scalar_<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  mln_trait_op_less(O, O)
  operator < (const Literal<L>& lhs, const value::scalar_<O>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename Vl, typename Vr>
  inline
  mln_trait_op_plus(Vl, Vr)
    operator + (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    typedef mln_trait_op_plus(Vl, Vr) R;
    return static_cast<R>(value::equiv(lhs)) + static_cast<R>(value::equiv(rhs));
  }

  template <typename Vl, typename Vr>
  inline
  mln_trait_op_minus(Vl, Vr)
    operator - (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    typedef mln_trait_op_minus(Vl, Vr) R;
    return static_cast<R>(value::equiv(lhs)) - static_cast<R>(value::equiv(rhs));
  }

  template <typename Vl, typename Vr>
  inline
  mln_trait_op_times(Vl, Vr)
  operator * (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    typedef mln_trait_op_times(Vl, Vr) R;
    return static_cast<R>(value::equiv(lhs)) * static_cast<R>(value::equiv(rhs));
  }

  template <typename Vl, typename Vr>
  inline
  mln_trait_op_div(Vl, Vr)
  operator / (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    typedef mln_trait_op_div(Vl, Vr) R;
    return static_cast<R>(value::equiv(lhs)) / static_cast<R>(value::equiv(rhs));
  }

  template <typename Vl, typename Vr>
  inline
  mln_trait_op_mod(Vl, Vr)
  operator % (const value::Scalar<Vl>& lhs, const value::Scalar<Vr>& rhs)
  {
    return value::equiv(lhs) % value::equiv(rhs);
  }

  template <typename S>
  inline
  mln_trait_op_uminus(S)
  operator - (const value::scalar_<S>& rhs)
  {
    typedef mln_trait_op_uminus(S) R;
    return static_cast<R>(- rhs.to_equiv());
  }

  template <typename S, typename O>
  inline
  mln_trait_op_plus(O, value::scalar_<S>)
  operator + (const value::scalar_<S>& lhs, const Object<O>& rhs)
  {
    return exact(rhs) + lhs;
  }

  template <typename S, typename O>
  inline
  mln_trait_op_times(O, value::scalar_<S>)
  operator * (const value::scalar_<S>& lhs, const Object<O>& rhs)
  {
    return exact(rhs) * lhs;
  }

  // ...


  // With scalar_ OP scalar_.

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_plus(Sl, Sr)
  operator + (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) + value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_minus(Sl, Sr)
  operator - (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) - value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_times(Sl, Sr)
  operator * (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) * value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_div(Sl, Sr)
  operator / (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) / value::equiv(rhs);
  }

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_mod(Sl, Sr)
  operator % (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) % value::equiv(rhs);
  }


  // Operator ==.

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_eq(Sl, Sr)
  operator == (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) == value::equiv(rhs);
  }

  inline
  bool
  operator == (const value::scalar_<int>& lhs, const value::scalar_<unsigned>& rhs)
  {
    return lhs.to_equiv() == int(rhs.to_equiv());
  }

  inline
  bool
  operator == (const value::scalar_<unsigned>& lhs, const value::scalar_<int>& rhs)
  {
    return int(lhs.to_equiv()) == rhs.to_equiv();
  }

  template <typename O, typename L>
  inline
  mln_trait_op_eq(O, O)
  operator==(const value::scalar_<O>& lhs, const Literal<L>& rhs)
  {
    return exact(lhs) == mln_value_equiv(O)(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_eq(O, O)
  operator==(const Literal<L>& lhs, const value::scalar_<O>& rhs)
  {
    return mln_value_equiv(O)(exact(lhs)) == exact(rhs);
  }


  // Operator <.

  template <typename Sl, typename Sr>
  inline
  mln_trait_op_less(Sl, Sr)
  operator < (const value::scalar_<Sl>& lhs, const value::scalar_<Sr>& rhs)
  {
    return value::equiv(lhs) < value::equiv(rhs);
  }

  template <typename O, typename L>
  inline
  mln_trait_op_less(O, O)
  operator < (const value::scalar_<O>& lhs, const Literal<L>& rhs)
  {
    return exact(lhs) < mln_value_equiv(O)(exact(rhs));
  }

  template <typename L, typename O>
  inline
  mln_trait_op_less(O, O)
  operator < (const Literal<L>& lhs, const value::scalar_<O>& rhs)
  {
    return mln_value_equiv(O)(exact(lhs)) < exact(rhs);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_OPS_HH
