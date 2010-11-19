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

#ifndef SCRIBO_CORE_TAG_COMPONENT_HH
# define SCRIBO_CORE_TAG_COMPONENT_HH

# include <iostream>

/// \file
///
/// \brief Component tags.

namespace scribo
{

  // Component id tag.
  struct ComponentId;



  namespace component
  {

    enum Tag
    {
      None = 0,
      Ignored
    };


    enum Type
    {
      Undefined = 0,
      Character,
      Separator,
      Noise,
      Punctuation
    };

# ifndef MLN_INCLUDE_ONLY


    std::ostream&
    operator<<(std::ostream& ostr, const Tag& tag)
    {
      std::string str;
      switch(tag)
      {
	default:
	case None:
	  str = "None";
	  break;
	case Ignored:
	  str = "Ignored";
	  break;
      }

      return ostr << str;
    }


    std::ostream&
    operator<<(std::ostream& ostr, const Type& type)
    {
      std::string str;
      switch(type)
      {
	default:
	case Undefined:
	  str = "Undefined";
	  break;
	case Character:
	  str = "Character";
	  break;
	case Separator:
	  str = "Separator";
	  break;
	case Noise:
	  str = "Noise";
	  break;
	case Punctuation:
	  str = "Punctuation";
	  break;
      }

      return ostr << str;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::component

} // end of namespace scribo


#endif // ! SCRIBO_CORE_TAG_COMP_HH
