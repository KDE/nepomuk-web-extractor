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

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Point_Site.
     *
     * \see mln::Point_Site
     */
    template <typename E>
    struct Point_Site
    {
      /*! \var dim
       * \brief Dimension of the space.
       * \invariant dim > 0 
       */
      enum { dim };

      /*! \brief Mesh associated type.
       * \invariant This type has to derive from mln::Mesh.
       */
      typedef void mesh;

      /*! \brief Site associated type.
       * \invariant This type has to derive from mln::Point.
       */
      typedef void point;

      /*! \brief Dpsite associated type.
       * \invariant This type has to derive from mln::Dpoint.
       */
      typedef void dpoint;

      /*! Coordinate associated type.
       */
      typedef void coord;


      /*! \brief Give a reference to the corresponding point.
       *
       * This method allows for iterators to refer to a point.
       *
       * \return A point constant reference.
       */
      const point& to_point() const;

      /*! \brief Read-only access to the \p i-th coordinate value.
       *
       * \param[in] i The coordinate index.
       *
       * \pre \p i < \c dim
       *
       * \return The value of the \p i-th coordinate.
       */
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
