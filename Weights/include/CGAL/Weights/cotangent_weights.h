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

#ifndef CGAL_COTANGENT_WEIGHTS_H
#define CGAL_COTANGENT_WEIGHTS_H

// #include <CGAL/license/Weights.h>

// Internal includes.
#include <CGAL/Weights/internal/utils.h>

namespace CGAL {
namespace Weights {

  /// \cond SKIP_IN_MANUAL
  namespace cotangent_ns {

    template<typename FT>
    const FT half_weight(
      const FT cot) {

      return FT(2) * cot;
    }

    template<typename FT>
    const FT weight(
      const FT cot_beta, const FT cot_gamma) {

      return FT(2) * (cot_beta + cot_gamma);
    }
  }
  /// \endcond

  /*!
    \ingroup PkgWeightsRefUtils

    \brief computes the half value of the `cotangent_weight()`.

    This function constructs the half of the cotangent weight using the precomputed
    cotangent value.

    The returned value is
    \f$2\textbf{cot}\f$.

    \tparam FT
    a model of `FieldNumberType`

    \param cot
    the cotangent value

    \sa `cotangent_weight()`
  */
  template<typename FT>
  const FT half_cotangent_weight(
    const FT cot) {

    return cotangent_ns::half_weight(cot);
  }

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightsRefWeights

    \brief computes the cotangent weight in 2D or 3D.

    The weight is computed as
    \f$w = 2 (\cot\beta + \cot\gamma)\f$
    with notations shown in the figure below.

    - This weight is equal to the `discrete_harmonic_weight()`.
    - This weight is a special case of the `three_point_family_weight()`.

    The type `GeomTraits::Point` must be either
    `GeomTraits::Point_2` or `GeomTraits::Point_3`.

    \cgalFigureBegin{cotangent_weight, cotangent.svg}
      Notation used for the cotangent weight.
    \cgalFigureEnd

    \tparam GeomTraits
    a model of `AnalyticWeightTraits_2` or `AnalyticWeightTraits_3`

    \param p0
    the first point

    \param p1
    the second point

    \param p2
    the third point

    \param q
    a query point

    \param traits
    this parameter can be omitted if the traits class can be deduced from the point type

    \note the points `p0`, `p1`, `p2` are ordered
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent_weight(
    const typename GeomTraits::Point& p0,
    const typename GeomTraits::Point& p1,
    const typename GeomTraits::Point& p2,
    const typename GeomTraits::Point& q,
    const GeomTraits& traits) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent_weight(
    const typename GeomTraits::Point_2& t,
    const typename GeomTraits::Point_2& r,
    const typename GeomTraits::Point_2& p,
    const typename GeomTraits::Point_2& q,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const FT cot_beta  = internal::cotangent_2(traits, q, t, r);
    const FT cot_gamma = internal::cotangent_2(traits, r, p, q);
    return cotangent_ns::weight(cot_beta, cot_gamma);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent_weight(
    const CGAL::Point_2<GeomTraits>& t,
    const CGAL::Point_2<GeomTraits>& r,
    const CGAL::Point_2<GeomTraits>& p,
    const CGAL::Point_2<GeomTraits>& q) {

    const GeomTraits traits;
    return cotangent_weight(t, r, p, q, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent_weight(
    const typename GeomTraits::Point_3& t,
    const typename GeomTraits::Point_3& r,
    const typename GeomTraits::Point_3& p,
    const typename GeomTraits::Point_3& q,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;
    const FT cot_beta  = internal::cotangent_3(traits, q, t, r);
    const FT cot_gamma = internal::cotangent_3(traits, r, p, q);
    return cotangent_ns::weight(cot_beta, cot_gamma);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT cotangent_weight(
    const CGAL::Point_3<GeomTraits>& t,
    const CGAL::Point_3<GeomTraits>& r,
    const CGAL::Point_3<GeomTraits>& p,
    const CGAL::Point_3<GeomTraits>& q) {

    const GeomTraits traits;
    return cotangent_weight(t, r, p, q, traits);
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_COTANGENT_WEIGHTS_H
