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

#ifndef CGAL_BARYCENTRIC_REGION_WEIGHTS_H
#define CGAL_BARYCENTRIC_REGION_WEIGHTS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>

namespace CGAL {
namespace Weights {

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefBarycentricRegionWeights

    \brief computes the area of the barycentric cell in 2D using the points `p`, `q`
    and `r`, given a traits class `traits` with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefBarycentricRegionWeights

    \brief computes the area of the barycentric cell in 3D using the points `p`, `q`
    and `r`, given a traits class `traits` with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefBarycentricRegionWeights

    \brief computes the area of the barycentric cell in 2D using the points `p`, `q`
    and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT barycentric_area(
    const CGAL::Point_2<K>& p,
    const CGAL::Point_2<K>& q,
    const CGAL::Point_2<K>& r) { }

  /*!
    \ingroup PkgWeightsRefBarycentricRegionWeights

    \brief computes the area of the barycentric cell in 3D using the points `p`, `q`
    and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT barycentric_area(
    const CGAL::Point_3<K>& p,
    const CGAL::Point_3<K>& q,
    const CGAL::Point_3<K>& r) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto midpoint_2 =
      traits.construct_midpoint_2_object();
    const auto centroid_2 =
      traits.construct_centroid_2_object();

    const auto center = centroid_2(p, q, r);
    const auto m1 = midpoint_2(q, r);
    const auto m2 = midpoint_2(q, p);

    const FT A1 = internal::positive_area_2(traits, q, m1, center);
    const FT A2 = internal::positive_area_2(traits, q, center, m2);
    return A1 + A2;
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    return barycentric_area(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto midpoint_3 =
      traits.construct_midpoint_3_object();
    const auto centroid_3 =
      traits.construct_centroid_3_object();

    const auto center = centroid_3(p, q, r);
    const auto m1 = midpoint_3(q, r);
    const auto m2 = midpoint_3(q, p);

    const FT A1 = internal::positive_area_3(traits, q, m1, center);
    const FT A2 = internal::positive_area_3(traits, q, center, m2);
    return A1 + A2;
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT barycentric_area(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    return barycentric_area(p, q, r, traits);
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_BARYCENTRIC_REGION_WEIGHTS_H
