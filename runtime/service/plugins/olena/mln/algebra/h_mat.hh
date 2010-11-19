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

#ifndef MLN_ALGEBRA_H_MAT_HH
# define MLN_ALGEBRA_H_MAT_HH

/*! \file
 *
 * \brief Definition of a matrix with homogeneous coordinates.
 *
 * \todo Add traits.
 */

# include <mln/algebra/mat.hh>


namespace mln
{

  namespace algebra
  {

    /*! \brief N-Dimensional matrix with homogeneous coordinates.
     *
     */
    template <unsigned d, typename T>
    struct h_mat : public mat<d+1, d+1, T>
    {
      /// Dimension is the 'natural' one (3 for 3D), not the one of the vector (dim + 1)
      enum { N = d,
	     M = d,
	     dim = d * d };

      /// Constructor without argument.
      h_mat();
      /// Constructor with the underlying matrix.
      h_mat(const mat<d+1, d+1, T>& x);
    };


# ifndef MLN_INCLUDE_ONLY

    template <unsigned d, typename T>
    inline
    h_mat<d,T>::h_mat()
      : mat<d+1, d+1, T>(mat<d+1, d+1, T>::Id)
    {
    }

    template <unsigned d, typename T>
    inline
    h_mat<d,T>::h_mat(const mat<d+1, d+1, T>& x)
      : mat<d+1, d+1, T>(x)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::algebra

} // end of namespace mln



#endif // ! MLN_ALGEBRA_H_MAT_HH
