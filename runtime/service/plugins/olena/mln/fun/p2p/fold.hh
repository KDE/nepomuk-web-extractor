// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_P2P_FOLD_HH
# define MLN_FUN_P2P_FOLD_HH

/// \file
///
/// FIXME: Doc!

# include <mln/core/concept/function.hh>
# include <mln/core/site_set/box.hh>


namespace mln
{

  namespace fun
  {

    namespace p2p
    {

      // Forward declaration.
      namespace internal {
	template <typename F, typename P>
	P do_fold(const P& p, const box<P>& b);
      } 


      template < typename P,
		 int dir_0 = -1,
		 int dir_1 = -1,
		 int dir_2 = -1 >
      struct fold : Function_v2v< fold<P,dir_0,dir_1,dir_2> >
      {
	fold();
	fold(const box<P>& b);

	typedef P result;
	P operator()(const P& p) const;

	box<P> b;
      };


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <int dim, typename F>
	struct do_fold_helper;

	template <typename P>
	struct do_fold_helper< 1, fold< P, -1, -1, -1 > >
	{
	  static P run(const P& p, const box<P>& b)
	  {
	    P tmp(p[0] % b.len(0));
	    return tmp;
	  }
	};

	template <typename P, int dir_0, int dir_1>
	struct do_fold_helper< 2, fold< P, dir_0, dir_1, -1 > >
	{
	  static P run(const P& p, const box<P>& b)
	  {
	    P tmp(dir_0 ? p[0] % b.len(0) : p[0],
		  dir_1 ? p[1] % b.len(1) : p[1]);
	    return tmp;
	  }
	};

	template <typename P, int dir_0, int dir_1, int dir_2>
	struct do_fold_helper< 3, fold< P, dir_0, dir_1, dir_2 > >
	{
	  static P run(const P& p, const box<P>& b)
	  {
	    P tmp(dir_0 ? p[0] % b.len(0) : p[0],
		  dir_1 ? p[1] % b.len(1) : p[1],
		  dir_2 ? p[2] % b.len(2) : p[2]);
	    return tmp;
	  }
	};

	template <typename F, typename P>
	inline
	P
	do_fold(const F&, const P& p, const box<P>& b)
	{
	  return do_fold_helper<P::dim, F>::run(p, b);
	}

      } // end of namespace mln::fun::p2p::internal


      // fold.

      template <typename P, int dir_0, int dir_1, int dir_2>
      inline
      fold<P,dir_0,dir_1,dir_2>::fold()
      {
      }

      template <typename P, int dir_0, int dir_1, int dir_2>
      inline
      fold<P,dir_0,dir_1,dir_2>::fold(const box<P>& b)
	: b(b)
      {
      }

      template <typename P, int dir_0, int dir_1, int dir_2>
      inline
      P
      fold<P,dir_0,dir_1,dir_2>::operator()(const P& p) const
      {
	return internal::do_fold(*this, p, b);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2p

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2P_FOLD_HH
