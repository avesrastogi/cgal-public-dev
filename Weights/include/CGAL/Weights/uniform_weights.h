// Copyright (c) 2020 GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
//
// Author(s)     : Dmitry Anisimov
//

#ifndef CGAL_UNIFORM_WEIGHTS_H
#define CGAL_UNIFORM_WEIGHTS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>

namespace CGAL {
namespace Weights {

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefUniformWeights

    \brief this function always returns 1, given four points in 2D and a traits class
    with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const GeomTraits&) { }

  /*!
    \ingroup PkgWeightsRefUniformWeights

    \brief this function always returns 1, given four points in 3D and a traits class
    with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const GeomTraits&) { }

  /*!
    \ingroup PkgWeightsRefUniformWeights

    \brief this function always returns 1, given four points in 2D, which are
    parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT uniform_weight(
    const CGAL::Point_2<K>&,
    const CGAL::Point_2<K>&,
    const CGAL::Point_2<K>&,
    const CGAL::Point_2<K>&) { }

  /*!
    \ingroup PkgWeightsRefUniformWeights

    \brief this function always returns 1, given four points in 3D, which are
    parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT uniform_weight(
    const CGAL::Point_3<K>&,
    const CGAL::Point_3<K>&,
    const CGAL::Point_3<K>&,
    const CGAL::Point_3<K>&) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const typename GeomTraits::Point_2&,
    const GeomTraits&) {

    using FT = typename GeomTraits::FT;
    return FT(1);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& t,
    const CGAL::Point_2<GeomTraits>& r,
    const CGAL::Point_2<GeomTraits>& p) {

    const GeomTraits traits;
    return uniform_weight(q, t, r, p, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const typename GeomTraits::Point_3&,
    const GeomTraits&) {

    using FT = typename GeomTraits::FT;
    return FT(1);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT uniform_weight(
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& t,
    const CGAL::Point_3<GeomTraits>& r,
    const CGAL::Point_3<GeomTraits>& p) {

    const GeomTraits traits;
    return uniform_weight(q, t, r, p, traits);
  }
  /// \endcond

  /*!
    \ingroup PkgWeightsRefUniformWeights

    \brief this function always returns 1.
  */
  double uniform_weight() {
    return 1.0;
  }

} // namespace Weights
} // namespace CGAL

#endif // CGAL_UNIFORM_WEIGHTS_H
