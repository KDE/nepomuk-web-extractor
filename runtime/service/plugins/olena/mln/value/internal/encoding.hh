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

#ifndef MLN_VALUE_INTERNAL_ENCODING_HH
# define MLN_VALUE_INTERNAL_ENCODING_HH

/*! \file
 *
 * \brief Define some information about how to encode some value types.
 */


namespace mln
{

  namespace value
  {

    namespace internal
    {


      template <unsigned n> struct encoding_unsigned_;

      template <> struct encoding_unsigned_<8>  { typedef unsigned char  ret; };
      template <> struct encoding_unsigned_<16> { typedef unsigned short ret; };
      template <> struct encoding_unsigned_<32> { typedef unsigned int   ret; }; // FIXME!!!
      template <> struct encoding_unsigned_<64> { typedef unsigned long  ret; }; // FIXME!!!

      template <> struct encoding_unsigned_<99> { typedef void ret; }; // stopper

      template <unsigned n>
      struct encoding_unsigned_
      {
	typedef typename encoding_unsigned_<n+1>::ret ret;
      };



      template <unsigned n> struct encoding_signed_;

      template <> struct encoding_signed_<8>  { typedef signed char  ret; };
      template <> struct encoding_signed_<16> { typedef signed short ret; };
      template <> struct encoding_signed_<32> { typedef signed int   ret; }; // FIXME!!!
      template <> struct encoding_signed_<64> { typedef signed long  ret; }; // FIXME!!!

      template <> struct encoding_signed_<99> { typedef void ret; }; // stopper

      template <unsigned n>
      struct encoding_signed_
      {
	typedef typename encoding_signed_<n+1>::ret ret;
      };


    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INTERNAL_ENCODING_HH
