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

#ifndef CGAL_WEIGHTS_UTILS_H
#define CGAL_WEIGHTS_UTILS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>
#include <CGAL/Weights/internal/polygon_utils.h>

namespace CGAL {
namespace Weights {

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefUtils

    \brief computes the tangent of the angle between 2D or 3D vectors `[q, r]` and `[q, p]`.

    The type `GeomTraits::Point` must be either
    `GeomTraits::Point_2` or `GeomTraits::Point_3`.

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
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const typename GeomTraits::Point& p,
    const typename GeomTraits::Point& q,
    const typename GeomTraits::Point& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefUtils

    \brief computes the cotangent of the angle between 2D or 3D vectors `[q, r]` and `[q, p]`.

    The type `GeomTraits::Point` must be either
    `GeomTraits::Point_2` or `GeomTraits::Point_3`.

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
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const typename GeomTraits::Point& p,
    const typename GeomTraits::Point& q,
    const typename GeomTraits::Point& r,
    const GeomTraits& traits) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) {

    return internal::tangent_2(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    return tangent(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) {

    return internal::tangent_3(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    return tangent(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) {

    return internal::cotangent_2(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    return cotangent(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) {

    return internal::cotangent_3(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    return cotangent(p, q, r, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT squared_distance(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q) {

    const GeomTraits traits;
    const auto squared_distance_2 =
      traits.compute_squared_distance_2_object();
    return squared_distance_2(p, q);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT squared_distance(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q) {

    const GeomTraits traits;
    const auto squared_distance_3 =
      traits.compute_squared_distance_3_object();
    return squared_distance_3(p, q);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT distance(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q) {

    const GeomTraits traits;
    return internal::distance_2(traits, p, q);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT distance(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q) {

    const GeomTraits traits;
    return internal::distance_3(traits, p, q);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT area(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    return internal::area_2(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT area(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    return internal::positive_area_3(traits, p, q, r);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT scalar_product(
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& r) {

    const GeomTraits traits;
    const auto scalar_product_2 =
      traits.compute_scalar_product_2_object();
    const auto construct_vector_2 =
      traits.construct_vector_2_object();

    const auto v1 = construct_vector_2(q, r);
    const auto v2 = construct_vector_2(q, p);
    return scalar_product_2(v1, v2);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT scalar_product(
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& r) {

    const GeomTraits traits;
    const auto scalar_product_3 =
      traits.compute_scalar_product_3_object();
    const auto construct_vector_3 =
      traits.construct_vector_3_object();

    const auto v1 = construct_vector_3(q, r);
    const auto v2 = construct_vector_3(q, p);
    return scalar_product_3(v1, v2);
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_WEIGHTS_UTILS_H
