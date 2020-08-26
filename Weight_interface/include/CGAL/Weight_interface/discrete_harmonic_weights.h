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

#ifndef CGAL_WEIGHT_INTERFACE_DISCRETE_HARMONIC_WEIGHTS_H
#define CGAL_WEIGHT_INTERFACE_DISCRETE_HARMONIC_WEIGHTS_H

// #include <CGAL/license/Weight_interface.h>

// Internal includes.
#include <CGAL/Weight_interface/internal/utils.h>
#include <CGAL/Weight_interface/internal/polygon_utils.h>

namespace CGAL {
namespace Weights {

  /// \cond SKIP_IN_MANUAL
  namespace discrete_harmonic_ns {

    template<typename FT>
    const FT weight(
      const FT r1, const FT r2, const FT r3,
      const FT A1, const FT A2, const FT B) {

      FT w = FT(0);
      CGAL_assertion(A1 != FT(0) && A2 != FT(0));
      const FT prod = A1 * A2;
      if (prod != FT(0)) {
        const FT inv = FT(1) / prod;
        w = (r3 * A1 - r2 * B + r1 * A2) * inv;
      }
      return w;
    }
  }
  /// \endcond

  #if defined(DOXYGEN_RUNNING)

  /*!
    \ingroup PkgWeightInterfaceRefWeights

    \brief computes the discrete harmonic weight for 2D or 3D points.

    The type `GeomTraits::Point` must be either
    `GeomTraits::Point_2` or `GeomTraits::Point_3`.

    The weight is computed as
    \f$w = \frac{d_2^2 A_1 - d^2 B + d_1^2 A_2}{A_1 A_2}\f$
    with notations shown in the figure below.

    This weight is equal to the `CGAL::Weights::cotangent_weight()`.

    This weight is a special case of the `CGAL::Weights::three_point_family_weight()`.

    \cgalFigureBegin{discrete_harmonic_weight, discrete_harmonic.svg}
      Notation used for the discrete harmonic weight.
    \cgalFigureEnd

    \tparam GeomTraits
    a model of `AnalyticWeightTraits_2` or `AnalyticWeightTraits_3`.

    \param q
    a query point

    \param t
    the first neighbor

    \param r
    the second neighbor

    \param p
    the third neighbor

    \param traits
    an instance of `GeomTraits`
  */
  template<typename GeomTraits>
  const typename GeomTraits::FT discrete_harmonic_weight(
    const typename GeomTraits::Point& q,
    const typename GeomTraits::Point& t,
    const typename GeomTraits::Point& r,
    const typename GeomTraits::Point& p,
    const GeomTraits& traits) { }

  #endif // DOXYGEN_RUNNING

  /// \cond SKIP_IN_MANUAL
  template<typename GeomTraits>
  const typename GeomTraits::FT discrete_harmonic_weight(
    const typename GeomTraits::Point_2& q,
    const typename GeomTraits::Point_2& t,
    const typename GeomTraits::Point_2& r,
    const typename GeomTraits::Point_2& p,
    const GeomTraits& traits) {

    using FT = typename GeomTraits::FT;

    const auto squared_distance_2 =
      traits.compute_squared_distance_2_object();
    const FT d1 = squared_distance_2(q, t);
    const FT d2 = squared_distance_2(q, r);
    const FT d3 = squared_distance_2(q, p);

    const FT A1 = internal::area_2(traits, r, q, t);
    const FT A2 = internal::area_2(traits, p, q, r);
    const FT B  = internal::area_2(traits, p, q, t);

    return discrete_harmonic_ns::weight(
      d1, d2, d3, A1, A2, B);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT discrete_harmonic_weight(
    const CGAL::Point_2<GeomTraits>& q,
    const CGAL::Point_2<GeomTraits>& t,
    const CGAL::Point_2<GeomTraits>& r,
    const CGAL::Point_2<GeomTraits>& p) {

    const GeomTraits traits;
    return discrete_harmonic_weight(q, t, r, p, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT discrete_harmonic_weight(
    const typename GeomTraits::Point_3& q,
    const typename GeomTraits::Point_3& t,
    const typename GeomTraits::Point_3& r,
    const typename GeomTraits::Point_3& p,
    const GeomTraits& traits) {

    using Point_2 = typename GeomTraits::Point_2;
    Point_2 qf, tf, rf, pf;
    internal::flatten(
      traits,
      q,  t,  r,  p,
      qf, tf, rf, pf);
    return discrete_harmonic_weight(qf, tf, rf, pf, traits);
  }

  template<typename GeomTraits>
  const typename GeomTraits::FT discrete_harmonic_weight(
    const CGAL::Point_3<GeomTraits>& q,
    const CGAL::Point_3<GeomTraits>& t,
    const CGAL::Point_3<GeomTraits>& r,
    const CGAL::Point_3<GeomTraits>& p) {

    const GeomTraits traits;
    return discrete_harmonic_weight(q, t, r, p, traits);
  }
  /// \endcond

  /*!
    \ingroup PkgWeightInterfaceRefBarycentric

    \brief 2D discrete harmonic weights for polygons.

    This class implements 2D discrete harmonic weights ( \cite cgal:bc:fhk-gcbcocp-06,
    \cite cgal:pp-cdmsc-93, \cite cgal:bc:eddhls-maam-95 ), which can be computed
    at any point inside a strictly convex polygon.

    Discrete harmonic weights are well-defined inside a strictly convex polygon
    but they are not necessarily positive. The weights are computed analytically
    using the formulation from `CGAL::Weights::discrete_harmonic_weight()`.

    \tparam Polygon
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`.

    \tparam GeomTraits
    a model of `AnalyticWeightTraits_2`.

    \tparam VertexMap
    a model of `ReadablePropertyMap` whose key type is `Polygon::value_type` and
    value type is `Point_2`. The default is `CGAL::Identity_property_map`.
  */
  template<
  typename Polygon,
  typename GeomTraits,
  typename VertexMap = CGAL::Identity_property_map<typename GeomTraits::Point_2> >
  class Discrete_harmonic_weights_2 {

  public:

    /// \name Types
    /// @{

    /// \cond SKIP_IN_MANUAL
    using Polygon_ = Polygon;
    using GT = GeomTraits;
    using Vertex_map = VertexMap;

    using Area_2 = typename GeomTraits::Compute_area_2;
    using Squared_distance_2 = typename GeomTraits::Compute_squared_distance_2;
    /// \endcond

    /// Number type.
    typedef typename GeomTraits::FT FT;

    /// Point type.
    typedef typename GeomTraits::Point_2 Point_2;

    /// @}

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      This class implements the behavior of discrete harmonic weights
      for 2D query points inside strictly convex polygons.

      \param polygon
      An instance of `Polygon` with the vertices of a strictly convex polygon.

      \param traits
      An instance of `GeomTraits`. The default initialization is provided.

      \param vertex_map
      An instance of `VertexMap` that maps a vertex from `polygon`
      to `Point_2`. The default is the identity property map.

      \pre polygon.size() >= 3
      \pre polygon is simple
      \pre polygon is strictly convex
    */
    Discrete_harmonic_weights_2(
      const Polygon& polygon,
      const GeomTraits traits = GeomTraits(),
      const VertexMap vertex_map = VertexMap()) :
    m_polygon(polygon),
    m_traits(traits),
    m_vertex_map(vertex_map),
    m_area_2(m_traits.compute_area_2_object()),
    m_squared_distance_2(m_traits.compute_squared_distance_2_object()) {

      CGAL_precondition(
        polygon.size() >= 3);
      CGAL_precondition(
        internal::is_simple_2(polygon, traits, vertex_map));
      CGAL_precondition(
        internal::polygon_type_2(polygon, traits, vertex_map) ==
        internal::Polygon_type::STRICTLY_CONVEX);
      resize();
    }

    /// @}

    /// \name Access
    /// @{

    /*!
      \brief computes 2D discrete harmonic weights.

      This function fills `weights` with 2D discrete harmonic weights computed at the `query`
      point with respect to the vertices of the input polygon. If `query` belongs to
      the polygon boundary, the weights are not well-defined.

      The method `CGAL::Barycentric_coordinates::Discrete_harmonic_coordinates_2::weights()`
      provides a more precise version that also handles query points on the polygon boundary.

      The number of returned weights equals to the number of polygon vertices.

      \tparam OutputIterator
      the dereferenced output iterator type convertible to `FT`.

      \param query
      A query point.

      \param w_begin
      The beginning of the destination range with the computed weights.

      \return an output iterator to the element in the destination range,
      one past the last weight stored.
    */
    template<typename OutputIterator>
    OutputIterator operator()(
      const Point_2& query,
      OutputIterator w_begin) {

      const bool normalize = false;
      return operator()(query, w_begin, normalize);
    }

    /// @}

    /// \cond SKIP_IN_MANUAL
    template<typename OutputIterator>
    OutputIterator operator()(
      const Point_2& query,
      OutputIterator w_begin,
      const bool normalize) {

      return optimal_weights(
        query, w_begin, normalize);
    }
    /// \endcond

  private:

    // Fields.
    const Polygon& m_polygon;
    const GeomTraits m_traits;
    const VertexMap m_vertex_map;
    const Area_2 m_area_2;
    const Squared_distance_2 m_squared_distance_2;

    std::vector<FT> r;
    std::vector<FT> A;
    std::vector<FT> B;
    std::vector<FT> w;

    // Functions.
    void resize() {
      r.resize(m_polygon.size());
      A.resize(m_polygon.size());
      B.resize(m_polygon.size());
      w.resize(m_polygon.size());
    }

    template<typename OutputIterator>
    OutputIterator optimal_weights(
      const Point_2& query,
      OutputIterator weights,
      const bool normalize) {

      // Get the number of vertices in the polygon.
      const std::size_t n = m_polygon.size();

      // Compute areas A, B, and distances r following the notation from [1].
      // Split the loop to make this computation faster.
      const auto& p1 = get(m_vertex_map, *(m_polygon.begin() + 0));
      const auto& p2 = get(m_vertex_map, *(m_polygon.begin() + 1));
      const auto& pn = get(m_vertex_map, *(m_polygon.begin() + (n - 1)));

      r[0] = m_squared_distance_2(p1, query);
      A[0] = m_area_2(p1, p2, query);
      B[0] = m_area_2(pn, p2, query);

      for (std::size_t i = 1; i < n - 1; ++i) {
        const auto& pi0 = get(m_vertex_map, *(m_polygon.begin() + (i - 1)));
        const auto& pi1 = get(m_vertex_map, *(m_polygon.begin() + (i + 0)));
        const auto& pi2 = get(m_vertex_map, *(m_polygon.begin() + (i + 1)));

        r[i] = m_squared_distance_2(pi1, query);
        A[i] = m_area_2(pi1, pi2, query);
        B[i] = m_area_2(pi0, pi2, query);
      }

      const auto& pm = get(m_vertex_map, *(m_polygon.begin() + (n - 2)));
      r[n - 1] = m_squared_distance_2(pn, query);
      A[n - 1] = m_area_2(pn, p1, query);
      B[n - 1] = m_area_2(pm, p1, query);

      // Compute unnormalized weights following the formula (25) with p = 2 from [1].
      CGAL_assertion(A[n - 1] != FT(0) && A[0] != FT(0));
      w[0] = (r[1] * A[n - 1] - r[0] * B[0] + r[n - 1] * A[0]) / (A[n - 1] * A[0]);

      for (std::size_t i = 1; i < n - 1; ++i) {
        CGAL_assertion(A[i - 1] != FT(0) && A[i] != FT(0));
        w[i] = (r[i + 1] * A[i - 1] - r[i] * B[i] + r[i - 1] * A[i]) / (A[i - 1] * A[i]);
      }

      CGAL_assertion(A[n - 2] != FT(0) && A[n - 1] != FT(0));
      w[n - 1] = (r[0] * A[n - 2] - r[n - 1] * B[n - 1] + r[n - 2] * A[n - 1]) / (A[n - 2] * A[n - 1]);

      // Normalize if necessary.
      if (normalize)
        internal::normalize(w);

      // Return weights.
      for (std::size_t i = 0; i < n; ++i)
        *(weights++) = w[i];
      return weights;
    }
  };

  /*!
    \ingroup PkgWeightInterfaceRefBarycentric

    \brief computes 2D discrete harmonic weights for polygons.

    This function computes 2D discrete harmonic weights at a given `query` point
    with respect to the vertices of a strictly convex `polygon`, that is one
    weight per vertex. The weights are stored in a destination range
    beginning at `w_begin`.

    Internally, the class `CGAL::Weights::Discrete_harmonic_weights_2` is used.
    If one needs a flexible API, please refer to that class. If you want to handle
    multiple query points, you better use that class, too. When using this function,
    internal memory is allocated for each query point, while when using the class,
    it is allocated only once, which is much more efficient.

    \tparam PointRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`.

    \tparam OutputIterator
    the dereferenced output iterator type convertible to `GeomTraits::FT`.

    \tparam GeomTraits
    a model of `AnalyticWeightTraits_2`.

    \param polygon
    An instance of `PointRange` with 2D points, which form a strictly convex polygon.

    \param query
    A query point.

    \param w_begin
    The beginning of the destination range with the computed weights.

    \param traits
    An instance of `GeomTraits`.

    \return an output iterator to the element in the destination range,
    one past the last weight stored.

    \pre polygon.size() >= 3
    \pre polygon is simple
    \pre polygon is strictly convex
  */
  template<
  typename PointRange,
  typename OutputIterator,
  typename GeomTraits>
  OutputIterator discrete_harmonic_weights_2(
    const PointRange& polygon,
    const typename GeomTraits::Point_2& query,
    OutputIterator w_begin,
    const GeomTraits traits) {

    Discrete_harmonic_weights_2<PointRange, GeomTraits> discrete_harmonic(
      polygon, traits);
    return discrete_harmonic(query, w_begin);
  }

  /// \cond SKIP_IN_MANUAL
  template<
  typename PointRange,
  typename Point_2,
  typename OutputIterator>
  OutputIterator discrete_harmonic_weights_2(
    const PointRange& polygon,
    const Point_2& query,
    OutputIterator w_begin) {

    using GeomTraits = typename Kernel_traits<Point_2>::Kernel;
    return discrete_harmonic_weights_2(
      polygon, query, w_begin, GeomTraits());
  }
  /// \endcond

} // namespace Weights
} // namespace CGAL

#endif // CGAL_WEIGHT_INTERFACE_DISCRETE_HARMONIC_WEIGHTS_H
