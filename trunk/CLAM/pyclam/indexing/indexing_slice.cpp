// Module slice.cpp
//
// Copyright (c) 2003 Raoul M. Gough
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
//
// History
// =======
// 2003/ 9/10   rmg     File creation
//
// $Id: slice.cpp,v 1.1.2.7 2003/11/24 14:28:32 raoulgough Exp $
//

#include <boost/python/suite/indexing/slice.hpp>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// MSVC6 workarounds
/////////////////////////////////////////////////////////////////////////////

#if defined (BOOST_NO_MEMBER_TEMPLATES)
namespace boost { namespace python { namespace indexing {
  slice::slice (::boost::python::handle<> const &p) : object (p) { }
  slice::slice (::boost::python::detail::borrowed_reference p) : object (p) { }
  slice::slice (::boost::python::detail::new_reference p) : object (p) { }
  slice::slice (::boost::python::detail::new_non_null_reference p) : object (p) { }
} } }
#endif

/////////////////////////////////////////////////////////////////////////////
// slice copy constructor
/////////////////////////////////////////////////////////////////////////////

boost::python::indexing::slice::slice (slice const &copy)
  : object (copy)
{
}

/////////////////////////////////////////////////////////////////////////////
// integer_slice constructor
/////////////////////////////////////////////////////////////////////////////

boost::python::indexing::integer_slice
::integer_slice (slice const &sl, index_type length)
  : m_slice (sl)
  // Leave index members uninitialized
{
  PySlice_GetIndices(
      reinterpret_cast<PySliceObject *> (m_slice.ptr()),
      length,
      &m_start,
      &m_stop,
      &m_step);

  if (m_step == 0)
    {
      // Can happen with Python prior to 2.3
      PyErr_SetString (PyExc_ValueError, "slice step cannot be zero");
      boost::python::throw_error_already_set ();
    }

  m_start = std::max (static_cast<index_type> (0), std::min (length, m_start));
  m_stop = std::max (static_cast<index_type> (0), std::min (length, m_stop));
  m_direction = (m_step > 0) ? 1 : -1;
}

/////////////////////////////////////////////////////////////////////////////
// Check if an index is within the range of this integer_slice
/////////////////////////////////////////////////////////////////////////////

bool boost::python::indexing::integer_slice::in_range (index_type index)
{
  return ((m_stop - index) * m_direction) > 0;
}
