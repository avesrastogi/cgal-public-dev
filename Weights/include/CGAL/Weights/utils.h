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
#include <CGAL/Weights/internal/polygon_utils_2.h>

namespace CGAL {
namespace Weights {

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefTangents

    \brief computes the tangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 2D points `p`, `q` and `r`, given a traits class `traits` with geometric objects,
    predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefTangents

    \brief computes the tangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 3D points `p`, `q` and `r`, given a traits class `traits` with geometric objects,
    predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefTangents

    \brief computes the tangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 2D points `p`, `q` and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT tangent(
    const CGAL::Point_2<K>& p,
    const CGAL::Point_2<K>& q,
    const CGAL::Point_2<K>& r) { }

  /*!
    \ingroup PkgWeightsRefTangents

    \brief computes the tangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 3D points `p`, `q` and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT tangent(
    const CGAL::Point_3<K>& p,
    const CGAL::Point_3<K>& q,
    const CGAL::Point_3<K>& r) { }

  /*!
    \ingroup PkgWeightsRefCotangents

    \brief computes the cotangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 2D points `p`, `q` and `r`, given a traits class `traits` with geometric objects,
    predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefCotangents

    \brief computes the cotangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 3D points `p`, `q` and `r`, given a traits class `traits` with geometric objects,
    predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent(
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& r,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefCotangents

    \brief computes the cotangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 2D points `p`, `q` and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT cotangent(
    const CGAL::Point_2<K>& p,
    const CGAL::Point_2<K>& q,
    const CGAL::Point_2<K>& r) { }

  /*!
    \ingroup PkgWeightsRefCotangents

    \brief computes the cotangent of the angle between the vectors `[q, r]` and `[q, p]`
    using the 3D points `p`, `q` and `r`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT cotangent(
    const CGAL::Point_3<K>& p,
    const CGAL::Point_3<K>& q,
    const CGAL::Point_3<K>& r) { }

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
