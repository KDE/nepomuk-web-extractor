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

#ifndef MLN_UTIL_YES_HH
# define MLN_UTIL_YES_HH

/// \file
///
/// Definition of an object that always says "yes".


# include <mln/core/concept/object.hh>
# include <mln/core/routine/ops.hh>


namespace mln
{

  // Forward declaration.
  namespace util { struct yes; }


  namespace trait
  {

    template <>
    struct set_precise_unary_< op::not_, util::yes >
    {
      typedef util::yes ret;
    };

    template <template<class, class> class Op>
    struct set_precise_binary_< Op, util::yes, bool >
    {
      typedef util::yes ret;
    };

    template <template<class, class> class Op>
    struct set_precise_binary_< Op, util::yes, util::yes >
    {
      typedef util::yes ret;
    };

  } // end of namespace mln::trait


  namespace util
  {

    /// Object that always says "yes".
    ///
    /// \ingroup modutil
    //
    struct yes : public Object< yes >
    {
      yes();
      explicit yes(bool);
      operator bool() const;
    };

  } // end of namespace mln::util


  // Negate.
  util::yes operator ! (const util::yes& y);

  // Equal.
  util::yes operator == (const util::yes&, bool);
  util::yes operator == (const util::yes&, const util::yes&);

  // Not equal.
  util::yes operator != (const util::yes&, bool);
  util::yes operator != (const util::yes&, const util::yes&);

  // And.
  util::yes operator && (const util::yes&, bool);

  // Or.
  util::yes operator || (const util::yes&, bool);


  // FIXME: Is-it suffisiant?


# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    // yes

    inline
    yes::yes()
    {
    }

    inline
    yes::yes(bool)
    {
    }

    inline
    yes::operator bool() const
    {
      return true;
    }

  } // end of namespace mln::util

  inline
  util::yes
  operator ! (const util::yes& y)
  {
    return y;
  }

  inline
  util::yes operator == (const util::yes&, bool)
  {
    return util::yes(true);
  }

  inline
  util::yes operator == (const util::yes&, const util::yes&)
  {
    return util::yes(true);
  }

  inline
  util::yes operator != (const util::yes&, bool)
  {
    return util::yes(true);
  }

  inline
  util::yes operator != (const util::yes&, const util::yes&)
  {
    return util::yes(true);
  }

  inline
  util::yes operator && (const util::yes&, bool)
  {
    return util::yes(true);
  }

  inline
  util::yes operator || (const util::yes&, bool)
  {
    return util::yes(true);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_UTIL_YES_HH
