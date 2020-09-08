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

#ifndef CGAL_VORONOI_REGION_WEIGHTS_H
#define CGAL_VORONOI_REGION_WEIGHTS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>

namespace CGAL {
namespace Weights {

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightInterfaceRefRegions

    \brief computes the area of the Voronoi cell in 2D or 3D.

    This area is the area of the shaded region in the figure below. The region
    is formed by the two midpoints of the edges incident to `q` and the circumcenter of
    the triangle `[p, q, r]`.

    The type `GeomTraits::Point` must be either
    `GeomTraits::Point_2` or `GeomTraits::Point_3`.

    \cgalFigureBegin{voronoi_area, voronoi_cell.svg}
      Notation used for the Voronoi cell.
    \cgalFigureEnd

    \tparam GeomTraits
    a model of `AnalyticWeightTraits_2` or `AnalyticWeightTraits_3`

    \param p
    the first point

    \param q
    the second point

    \param r
    the third point

    \param traits
    this parameter can be omitted if the traits class can be deduced from the point type

    \sa `mixed_voronoi_area()`
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT voronoi_area(
    const typename GeomTraits::Point& p,
    const typename GeomTraits::Point& q,
    const typename GeomTraits::Point& r,
    const GeomTraits& traits) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT voronoi_area(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto circumcenter_2 =
      traits.construct_circumcenter_2_object();
    const auto midpoint_2 =
      traits.construct_midpoint_2_object();

    const auto center = circumcenter_2(p, q, r);
    const auto m1 = midpoint_2(q, r);
    const auto m2 = midpoint_2(q, p);

    const FT A1 = internal::positive_area_2(traits, q, m1, center);
    const FT A2 = internal::positive_area_2(traits, q, center, m2);
    return A1 + A2;
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT voronoi_area(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    return voronoi_area(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT voronoi_area(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto circumcenter_3 =
      traits.construct_circumcenter_3_object();
    const auto midpoint_3 =
      traits.construct_midpoint_3_object();

    const auto center = circumcenter_3(p, q, r);
    const auto m1 = midpoint_3(q, r);
    const auto m2 = midpoint_3(q, p);

    const FT A1 = internal::positive_area_3(traits, q, m1, center);
    const FT A2 = internal::positive_area_3(traits, q, center, m2);
    return A1 + A2;
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT voronoi_area(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    return voronoi_area(p, q, r, traits);
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_VORONOI_REGION_WEIGHTS_H
