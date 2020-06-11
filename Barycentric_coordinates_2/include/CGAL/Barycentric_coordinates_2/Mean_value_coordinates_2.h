// Copyright (c) 2019 INRIA Sophia-Antipolis (France).
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
// Author(s)     : Dmitry Anisimov, David Bommes, Kai Hormann, Pierre Alliez
//

#ifndef CGAL_BARYCENTRIC_MEAN_VALUE_COORDINATES_2_H
#define CGAL_BARYCENTRIC_MEAN_VALUE_COORDINATES_2_H

#include <CGAL/license/Barycentric_coordinates_2.h>

// Internal includes.
#include <CGAL/Barycentric_coordinates_2/barycentric_enum_2.h>
#include <CGAL/Barycentric_coordinates_2/internal/Mean_value_weights_2.h>

// [1] Reference: "K. Hormann and M. Floater.
// Mean value coordinates for arbitrary planar polygons.
// ACM Transactions on Graphics, 25(4):1424-1441, 2006.".

// [2] Reference: "M. S. Floater.
// Wachspress and mean value coordinates.
// Proceedings of the 14th International Conference on Approximation Theory.
// G. Fasshauer and L. L. Schumaker (eds.)."

namespace CGAL {
namespace Barycentric_coordinates {

  /*!
    \ingroup PkgBarycentricCoordinates2RefAnalytic

    \brief 2D mean value coordinates.

    This class implements 2D mean value coordinates ( \cite cgal:bc:hf-mvcapp-06,
    \cite cgal:bc:fhk-gcbcocp-06, \cite cgal:f-mvc-03 ), which can be computed
    at any point in the plane.

    Mean value coordinates are well-defined everywhere in the plane and are
    non-negative in the kernel of a star-shaped polygon. The coordinates are
    computed analytically.

    \tparam Polygon
    must be a model of `ConstRange` whose iterator type is `RandomAccessIterator`.

    \tparam GeomTraits
    must be a model of `BarycentricTraits_2`.

    \tparam VertexMap
    must be a `ReadablePropertyMap` whose key type is `Polygon::value_type` and
    value type is `Point_2`. The default is `CGAL::Identity_property_map`.
  */
  template<
  typename Polygon,
  typename GeomTraits,
  typename VertexMap = CGAL::Identity_property_map<typename GeomTraits::Point_2> >
  class Mean_value_coordinates_2 {

  public:

    /// \name Types
    /// @{

    /// \cond SKIP_IN_MANUAL
    using Polygon_ = Polygon;
    using GT = GeomTraits;
    using Vertex_map = VertexMap;

    using Vector_2 = typename GeomTraits::Vector_2;
    using Area_2 = typename GeomTraits::Compute_area_2;
    using Squared_length_2 = typename GeomTraits::Compute_squared_length_2;
    using Scalar_product_2 = typename GeomTraits::Compute_scalar_product_2;
    using Get_sqrt = internal::Get_sqrt<GeomTraits>;
    using Sqrt = typename Get_sqrt::Sqrt;

    using Mean_value_weights_2 =
      internal::Mean_value_weights_2<Polygon, GeomTraits, Vertex_map>;
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

      This class implements the behavior of mean value coordinates
      for 2D query points.

      \param polygon
      An instance of `Polygon` with the vertices of a simple polygon.

      \param policy
      One of the `CGAL::Barycentric_coordinates::Computation_policy_2`.
      The default is `CGAL::Barycentric_coordinates::Computation_policy_2::DEFAULT`.

      \param traits
      An instance of `GeomTraits`. The default initialization is provided.

      \param vertex_map
      An instance of `VertexMap` that maps a vertex from `polygon`
      to `Point_2`. The default is the identity property map.

      \pre `polygon.size() >= 3`
      \pre `polygon is simple`
    */
    Mean_value_coordinates_2(
      const Polygon& polygon,
      const Computation_policy_2 policy
      = Computation_policy_2::DEFAULT,
      const GeomTraits traits = GeomTraits(),
      const VertexMap vertex_map = VertexMap()) :
    m_polygon(polygon),
    m_computation_policy(policy),
    m_traits(traits),
    m_vertex_map(vertex_map),
    m_area_2(m_traits.compute_area_2_object()),
    m_squared_length_2(m_traits.compute_squared_length_2_object()),
    m_scalar_product_2(m_traits.compute_scalar_product_2_object()),
    m_sqrt(Get_sqrt::sqrt_object(m_traits)),
    m_mean_value_weights_2(
      polygon, traits, vertex_map) {

      CGAL_precondition(
        polygon.size() >= 3);
      CGAL_precondition(
        internal::is_simple_2(polygon, traits, vertex_map));
      resize();
    }

    /// @}

    /// \name Access
    /// @{

    /*!
      \brief computes 2D mean value weights.

      This function fills `weights` with 2D mean value weights computed at the `query`
      point with respect to the vertices of the input polygon. If `query` belongs to
      the polygon boundary, the returned weights are normalized.

      The number of returned weights equals to the number of polygon vertices.

      \tparam OutputIterator
      must be an output iterator whose value type is `FT`.

      \param query
      A query point.

      \param weights
      An output iterator that stores the computed weights.

      \return an output iterator.
    */
    template<typename OutputIterator>
    OutputIterator weights(
      const Point_2& query,
      OutputIterator weights) {

      const bool normalize = false;
      return compute(query, weights, normalize);
    }

    /*!
      \brief computes 2D mean value coordinates.

      This function fills `coordinates` with 2D mean value coordinates computed at the `query`
      point with respect to the vertices of the input polygon.

      The number of returned coordinates equals to the number of polygon vertices.

      \tparam OutputIterator
      must be an output iterator whose value type is `FT`.

      \param query
      A query point.

      \param coordinates
      An output iterator that stores the computed coordinates.

      \return an output iterator.
    */
    template<typename OutputIterator>
    OutputIterator operator()(
      const Point_2& query,
      OutputIterator coordinates) {

      const bool normalize = true;
      return compute(query, coordinates, normalize);
    }

    /// @}

  private:

    // Fields.
    const Polygon& m_polygon;
    const Computation_policy_2 m_computation_policy;
    const GeomTraits m_traits;
    const VertexMap m_vertex_map;
    const Area_2 m_area_2;
    const Squared_length_2 m_squared_length_2;
    const Scalar_product_2 m_scalar_product_2;
    const Sqrt m_sqrt;

    Mean_value_weights_2 m_mean_value_weights_2;

    std::vector<Vector_2> s;
    std::vector<FT> r;
    std::vector<FT> A;
    std::vector<FT> B;
    std::vector<FT> P;
    std::vector<FT> w;

    // Functions.
    void resize() {
      s.resize(m_polygon.size());
      r.resize(m_polygon.size());
      A.resize(m_polygon.size());
      B.resize(m_polygon.size());
      P.resize(m_polygon.size());
      w.resize(m_polygon.size());
    }

    template<typename OutputIterator>
    OutputIterator compute(
      const Point_2& query,
      OutputIterator output,
      const bool normalize) {

      switch (m_computation_policy) {

        case Computation_policy_2::PRECISE: {
          if (normalize) {
            return max_precision_coordinates(query, output);
          } else {
            std::cerr << "WARNING: you can't use the precise version of unnormalized weights! ";
            std::cerr << "They are not valid weights!" << std::endl;
            internal::get_default(m_polygon.size(), output);
            return output;
          }
        }

        case Computation_policy_2::PRECISE_WITH_EDGE_CASES: {
          const auto edge_case = verify(query, output);
          if (edge_case == internal::Edge_case::BOUNDARY)
            return output;
          if (normalize) {
            return max_precision_coordinates(query, output);
          } else {
            std::cerr << "WARNING: you can't use the precise version of unnormalized weights! ";
            std::cerr << "They are not valid weights!" << std::endl;
            internal::get_default(m_polygon.size(), output);
            return output;
          }
        }

        case Computation_policy_2::FAST: {
          return m_mean_value_weights_2(query, output, normalize);
        }

        case Computation_policy_2::FAST_WITH_EDGE_CASES: {
          const auto edge_case = verify(query, output);
          if (edge_case == internal::Edge_case::BOUNDARY)
            return output;
          return m_mean_value_weights_2(query, output, normalize);
        }

        default: {
          internal::get_default(m_polygon.size(), output);
          return output;
        }
      }
      return output;
    }

    template<typename OutputIterator>
    internal::Edge_case verify(
      const Point_2& query,
      OutputIterator output) const {

      const auto result = internal::locate_wrt_polygon_2(
        m_polygon, query, m_traits, m_vertex_map);
      if (!result)
        return internal::Edge_case::EXTERIOR;

      const auto location = (*result).first;
      const std::size_t index = (*result).second;
      if (location == internal::Query_point_location::ON_UNBOUNDED_SIDE)
        return internal::Edge_case::EXTERIOR;

      if (
        location == internal::Query_point_location::ON_VERTEX ||
        location == internal::Query_point_location::ON_EDGE ) {
        internal::boundary_coordinates_2(
          m_polygon, query, location, index, output, m_traits, m_vertex_map);
        return internal::Edge_case::BOUNDARY;
      }
      return internal::Edge_case::INTERIOR;
    }

    template<typename OutputIterator>
    OutputIterator max_precision_coordinates(
      const Point_2& query,
      OutputIterator coordinates) {

      // Get the number of vertices in the polygon.
      const std::size_t n = m_polygon.size();

      // Compute vectors s and its lengths r following the pseudo-code
      // in the Figure 10 from [1].
      const auto& p1 = get(m_vertex_map, *(m_polygon.begin() + 0));
      const auto& p2 = get(m_vertex_map, *(m_polygon.begin() + 1));
      const auto& pn = get(m_vertex_map, *(m_polygon.begin() + (n - 1)));

      s[0] = p1 - query;
      r[0] = m_sqrt(m_squared_length_2(s[0]));

      // Compute areas A and B following the notation from [1] (see Figure 2).
      // Split the loop to make this computation faster.
      A[0] = m_area_2(p1, p2, query);
      B[0] = m_area_2(pn, p2, query);

      for (std::size_t i = 1; i < n - 1; ++i) {
        const auto& pi0 = get(m_vertex_map, *(m_polygon.begin() + (i - 1)));
        const auto& pi1 = get(m_vertex_map, *(m_polygon.begin() + (i + 0)));
        const auto& pi2 = get(m_vertex_map, *(m_polygon.begin() + (i + 1)));

        s[i] = pi1 - query;
        r[i] = m_sqrt(m_squared_length_2(s[i]));

        A[i] = m_area_2(pi1, pi2, query);
        B[i] = m_area_2(pi0, pi2, query);
      }

      const auto& pm = get(m_vertex_map, *(m_polygon.begin() + (n - 2)));
      s[n - 1] = pn - query;
      r[n - 1] = m_sqrt(m_squared_length_2(s[n - 1]));

      A[n - 1] = m_area_2(pn, p1, query);
      B[n - 1] = m_area_2(pm, p1, query);

      // Following section 4.2 from [2] we denote P_j = r_j*r_{j+1} + dot_product(d_j, d_{j+1}).
      // Vector s_i from [1] corresponds to that one with the name d_i in [2].
      for (std::size_t j = 0; j < n - 1; ++j)
        P[j] = CGAL::max(r[j] * r[j + 1] + m_scalar_product_2(s[j], s[j + 1]), FT(0));
      P[n - 1] = CGAL::max(r[n - 1] * r[0] + m_scalar_product_2(s[n - 1], s[0]), FT(0));

      // Compute mean value weights using the formula (16) from [2].
      // Since the formula (16) always gives positive values,
      // we have to add a proper sign to all the weight functions.
      w[0] = r[n - 1] * r[1] - m_scalar_product_2(s[n - 1], s[1]);
      for (std::size_t j = 1; j < n - 1; ++j)
        w[0] *= P[j];
      w[0] = sign_of_weight(A[n - 1], A[0], B[0]) * m_sqrt(w[0]);

      for (std::size_t i = 1; i < n - 1; ++i) {
        w[i] = r[i - 1] * r[i + 1] - m_scalar_product_2(s[i - 1], s[i + 1]);

        for (std::size_t j = 0; j < i - 1; ++j)
          w[i] *= P[j];

        for(std::size_t j = i + 1; j < n; ++j)
          w[i] *= P[j];

        w[i] = sign_of_weight(A[i - 1], A[i], B[i]) * m_sqrt(w[i]);
      }

      w[n - 1] = r[n - 2] * r[0] - m_scalar_product_2(s[n - 2], s[0]);
      for (std::size_t j = 0; j < n - 2; ++j)
        w[n - 1] *= P[j];
      w[n - 1] = sign_of_weight(A[n - 2], A[n - 1], B[n - 1]) * m_sqrt(w[n - 1]);

      // Return coordinates.
      internal::normalize(w);
      for (std::size_t i = 0; i < n; ++i)
        *(coordinates++) = w[i];

      return coordinates;
    }

    // Return the sign of a mean value weight function.
    // We can have 3 different values: 0 if the weight = 0,
    // -1 if the weight is negative, and +1 if the weight is positive.
    FT sign_of_weight(const FT& A_prev, const FT& A, const FT& B) const {

      if (A_prev > FT(0) && A > FT(0) && B <= FT(0)) return  FT(1);
      if (A_prev < FT(0) && A < FT(0) && B >= FT(0)) return -FT(1);
      if (B > FT(0)) return  FT(1);
      if (B < FT(0)) return -FT(1);

      return FT(0);
    }
  };

} // namespace Barycentric_coordinates
} // namespace CGAL

#endif // CGAL_BARYCENTRIC_MEAN_VALUE_COORDINATES_2_H
