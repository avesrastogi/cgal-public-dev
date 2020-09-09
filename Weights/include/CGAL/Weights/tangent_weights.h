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

#ifndef CGAL_TANGENT_WEIGHTS_H
#define CGAL_TANGENT_WEIGHTS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>

namespace CGAL {
namespace Weights {

  /// \cond SKIP_IN_MANUAL
  namespace tangent_ns {

    template<typename FT>
    const FT half_angle_tangent(
      const FT r, const FT d,
      const FT A, const FT D) {

      FT t = FT(0);
      const FT P = r * d + D;
      CGAL_precondition(P != FT(0));
      if (P != FT(0)) {
        const FT inv = FT(2) / P;
        t = A * inv;
      }
      return t;
    }

    template<typename FT>
    const FT half_weight(
      const FT t, const FT r) {

      FT w = FT(0);
      CGAL_precondition(r != FT(0));
      if (r != FT(0)) {
        const FT inv = FT(2) / r;
        w = t * inv;
      }
      return w;
    }

    template<typename FT>
    const FT weight(
      const FT d1, const FT r, const FT d2,
      const FT A1, const FT A2,
      const FT D1, const FT D2) {

      const FT P1 = d1 * r + D1;
      const FT P2 = d2 * r + D2;

      FT w = FT(0);
      CGAL_precondition(P1 != FT(0) && P2 != FT(0));
      if (P1 != FT(0) && P2 != FT(0)) {
        const FT inv1 = FT(2) / P1;
        const FT inv2 = FT(2) / P2;
        const FT t1 = A1 * inv1;
        const FT t2 = A2 * inv2;
        CGAL_precondition(r != FT(0));
        if (r != FT(0)) {
          const FT inv = FT(2) / r;
          w = (t1 + t2) * inv;
        }
      }
      return w;
    }
  }
  /// \endcond

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the tangent of the half angle.

    This function computes the tangent of the half angle using the precomputed
    distance, area, and dot product values. The returned value is
    \f$\frac{2\textbf{A}}{\textbf{d}\textbf{l} + \textbf{D}}\f$.

    \tparam FT
    a model of `FieldNumberType`

    \param d
    the distance value

    \param l
    the distance value

    \param A
    the area value

    \param D
    the dot product value

    \pre (d * l + D) != 0

    \sa `half_tangent_weight()`
  */
  template<typename FT>
  const FT tangent_half_angle(
    const FT d, const FT l, const FT A, const FT D) {

    return tangent_ns::half_angle_tangent(d, l, A, D);
  }

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the half value of the tangent weight.

    This function constructs the half of the tangent weight using the precomputed
    half angle tangent and distance values. The returned value is
    \f$\frac{2\textbf{tan05}}{\textbf{d}}\f$.

    \tparam FT
    a model of `FieldNumberType`

    \param tan05
    the half angle tangent value

    \param d
    the distance value

    \pre d != 0

    \sa `tangent_half_angle()`
    \sa `tangent_weight()`
  */
  template<typename FT>
  const FT half_tangent_weight(
    const FT tan05, const FT d) {

    return tangent_ns::half_weight(tan05, d);
  }

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the half value of the tangent weight.

    This function constructs the half of the tangent weight using the precomputed
    distance, area, and dot product values. The returned value is
    \f$\frac{2\textbf{t}}{\textbf{d}}\f$ where
    \f$\textbf{t} = \frac{2\textbf{A}}{\textbf{d}\textbf{l} + \textbf{D}}\f$.

    \tparam FT
    a model of `FieldNumberType`

    \param d
    the distance value

    \param l
    the distance value

    \param A
    the area value

    \param D
    the dot product value

    \pre (d * l + D) != 0 && d != 0

    \sa `tangent_weight()`
  */
  template<typename FT>
  const FT half_tangent_weight(
    const FT d, const FT l, const FT A, const FT D) {

    const FT tan05 = tangent_half_angle(d, l, A, D);
    return half_tangent_weight(tan05, d);
  }

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the tangent weight in 2D at `q` using the points `p0`, `p1`,
    and `p2`, given a traits class `traits` with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const typename GeomTraits::Point_2& p0,
    const typename GeomTraits::Point_2& p1,
    const typename GeomTraits::Point_2& p2,
    const typename GeomTraits::Point_2& q,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the tangent weight in 3D at `q` using the points `p0`, `p1`,
    and `p2`, given a traits class `traits` with geometric objects, predicates, and constructions.
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const typename GeomTraits::Point_3& p0,
    const typename GeomTraits::Point_3& p1,
    const typename GeomTraits::Point_3& p2,
    const typename GeomTraits::Point_3& q,
    const GeomTraits& traits) { }

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the tangent weight in 2D at `q` using the points `p0`, `p1`,
    and `p2`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT tangent_weight(
    const CGAL::Point_2<K>& p0,
    const CGAL::Point_2<K>& p1,
    const CGAL::Point_2<K>& p2,
    const CGAL::Point_2<K>& q) { }

  /*!
    \ingroup PkgWeightsRefTangentWeights

    \brief computes the tangent weight in 3D at `q` using the points `p0`, `p1`,
    and `p2`, which are parameterized by a `Kernel` K.
  */
  template<typename K>
  const typename K::FT tangent_weight(
    const CGAL::Point_3<K>& p0,
    const CGAL::Point_3<K>& p1,
    const CGAL::Point_3<K>& p2,
    const CGAL::Point_3<K>& q) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const typename GeomTraits::Point_2& t,
    const typename GeomTraits::Point_2& r,
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto dot_product_2 =
      traits.compute_scalar_product_2_object();
    const auto construct_vector_2 =
      traits.construct_vector_2_object();

    const auto v1 = construct_vector_2(q, t);
    const auto v2 = construct_vector_2(q, r);
    const auto v3 = construct_vector_2(q, p);

    const FT l1 = internal::length_2(traits, v1);
    const FT l2 = internal::length_2(traits, v2);
    const FT l3 = internal::length_2(traits, v3);

    const FT A1 = internal::area_2(traits, r, q, t);
    const FT A2 = internal::area_2(traits, p, q, r);

    const FT D1 = dot_product_2(v1, v2);
    const FT D2 = dot_product_2(v2, v3);

    return tangent_ns::weight(
      l1, l2, l3, A1, A2, D1, D2);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const CGAL::Point_2<GeomTraits>& t,
    const CGAL::Point_2<GeomTraits>& r,
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q) {

    const GeomTraits traits;
    return tangent_weight(t, r, p, q, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const typename GeomTraits::Point_3& t,
    const typename GeomTraits::Point_3& r,
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const auto dot_product_3 =
      traits.compute_scalar_product_3_object();
    const auto construct_vector_3 =
      traits.construct_vector_3_object();

    const auto v1 = construct_vector_3(q, t);
    const auto v2 = construct_vector_3(q, r);
    const auto v3 = construct_vector_3(q, p);

    const FT l1 = internal::length_3(traits, v1);
    const FT l2 = internal::length_3(traits, v2);
    const FT l3 = internal::length_3(traits, v3);

    const FT A1 = internal::positive_area_3(traits, r, q, t);
    const FT A2 = internal::positive_area_3(traits, p, q, r);

    const FT D1 = dot_product_3(v1, v2);
    const FT D2 = dot_product_3(v2, v3);

    return tangent_ns::weight(
      l1, l2, l3, A1, A2, D1, D2);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT tangent_weight(
    const CGAL::Point_3<GeomTraits>& t,
    const CGAL::Point_3<GeomTraits>& r,
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q) {

    const GeomTraits traits;
    return tangent_weight(t, r, p, q, traits);
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_TANGENT_WEIGHTS_H
