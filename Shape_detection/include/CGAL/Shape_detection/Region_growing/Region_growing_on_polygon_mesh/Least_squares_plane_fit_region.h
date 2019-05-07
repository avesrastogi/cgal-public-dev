// Copyright (c) 2018 INRIA Sophia-Antipolis (France).
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
// Author(s)     : Florent Lafarge, Simon Giraudot, Thien Hoang, Dmitry Anisimov
//

#ifndef CGAL_SHAPE_DETECTION_REGION_GROWING_POLYGON_MESH_LEAST_SQUARES_PLANE_FIT_REGION_H
#define CGAL_SHAPE_DETECTION_REGION_GROWING_POLYGON_MESH_LEAST_SQUARES_PLANE_FIT_REGION_H

#include <CGAL/license/Shape_detection.h>

// STL includes.
#include <vector>

// Boost includes.
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>

// Face graph includes.
#include <CGAL/boost/graph/iterator.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>

// CGAL includes.
#include <CGAL/assertions.h>
#include <CGAL/number_utils.h>
#include <CGAL/Cartesian_converter.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// Internal includes.
#include <CGAL/Shape_detection/Region_growing/internal/utils.h>

namespace CGAL {
namespace Shape_detection {
namespace Polygon_mesh {
  
  /*!
    \ingroup PkgShapeDetectionRGOnMesh

    \brief Region type based on the quality of the least squares plane 
    fit applied to faces of a polygon mesh.

    This class fits a plane to chunks of faces in a polygon mesh and controls 
    the quality of this fit. If all quality conditions are satisfied, the chunk
    is accepted as a valid region, otherwise rejected.

    \tparam GeomTraits 
    must be a model of `Kernel`.

    \tparam PolygonMesh 
    must be a model of `FaceListGraph`.

    \tparam FaceRange 
    must be a model of `ConstRange` whose iterator type is `RandomAccessIterator` and 
    value type is the face type of a polygon mesh.

    \tparam VertexToPointMap 
    must be an `LvaluePropertyMap` whose key type is the vertex type of a polygon mesh and
    value type is `Kernel::Point_3`.
    
    \cgalModels `RegionType`
  */
  template<
  typename GeomTraits, 
  typename PolygonMesh,
  typename FaceRange = typename PolygonMesh::Face_range,
  typename VertexToPointMap = typename boost::property_map<PolygonMesh, CGAL::vertex_point_t>::type>
  class Least_squares_plane_fit_region {

  public:

    /// \cond SKIP_IN_MANUAL
    using Traits = GeomTraits;
    using Face_graph = PolygonMesh;
    using Face_range = FaceRange;
    using Vertex_to_point_map = VertexToPointMap;

    using Point_3 = typename Traits::Point_3;
    using Vector_3 = typename Traits::Vector_3;
    using Plane_3 = typename Traits::Plane_3;

    using Local_traits = Exact_predicates_inexact_constructions_kernel;
    using Local_point_3 = typename Local_traits::Point_3;
    using Local_plane_3 = typename Local_traits::Plane_3;
    using To_local_converter = Cartesian_converter<Traits, Local_traits>;

    using Squared_length_3 = typename Traits::Compute_squared_length_3;
    using Squared_distance_3 = typename Traits::Compute_squared_distance_3;
    using Scalar_product_3 = typename Traits::Compute_scalar_product_3;

    using Get_sqrt = internal::Get_sqrt<Traits>;
    using Sqrt = typename Get_sqrt::Sqrt;
    /// \endcond

    /// \name Types
    /// @{

    /// Number type.
    typedef typename GeomTraits::FT FT;

    /// @}

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \param pmesh 
      an instance of `PolygonMesh` that represents a polygon mesh

      \param distance_threshold 
      the maximum distance from the furthest vertex of a face to a plane. %Default is 1.

      \param angle_threshold 
      the maximum accepted angle in degrees between the normal of a face and 
      the normal of a plane. %Default is 25 degrees.

      \param min_region_size 
      the minimum number of faces a region must have. %Default is 1.
      
      \param vertex_to_point_map 
      an instance of `VertexToPointMap` that maps a polygon mesh 
      vertex to `Kernel::Point_3`

      \param traits
      an instance of `GeomTraits`

      \pre `faces(pmesh).size() > 0`
      \pre `distance_threshold >= 0`
      \pre `angle_threshold >= 0 && angle_threshold <= 90`
      \pre `min_region_size > 0`
    */
    Least_squares_plane_fit_region(
      const PolygonMesh& pmesh,
      const FT distance_threshold = FT(1), 
      const FT angle_threshold = FT(25), 
      const std::size_t min_region_size = 1, 
      const VertexToPointMap vertex_to_point_map = VertexToPointMap(), 
      const GeomTraits traits = GeomTraits()) :
    m_face_graph(pmesh),
    m_face_range(faces(m_face_graph)),
    m_distance_threshold(distance_threshold),
    m_normal_threshold(static_cast<FT>(
      std::cos(
        CGAL::to_double(
          (angle_threshold * static_cast<FT>(CGAL_PI)) / FT(180))))),
    m_min_region_size(min_region_size),
    m_vertex_to_point_map(vertex_to_point_map),
    m_squared_length_3(traits.compute_squared_length_3_object()),
    m_squared_distance_3(traits.compute_squared_distance_3_object()),
    m_scalar_product_3(traits.compute_scalar_product_3_object()),
    m_sqrt(Get_sqrt::sqrt_object(traits)),
    m_to_local_converter() {

      CGAL_precondition(m_face_range.size() > 0);

      CGAL_precondition(distance_threshold >= FT(0));
      CGAL_precondition(angle_threshold >= FT(0) && angle_threshold <= FT(90));
      CGAL_precondition(min_region_size > 0);
    }

    /// @}

    /// \name Access
    /// @{ 

    /*!
      \brief implements `RegionType::is_part_of_region()`.

      This function controls if a face with the index `query_index` is within
      the `distance_threshold` from the corresponding plane and if the angle
      between its normal and the plane's normal is within the `angle_threshold`.
      If both conditions are satisfied, it returns `true`, otherwise `false`.

      \param query_index
      index of the query face

      The first and third parameters are not used in this implementation.

      \return Boolean `true` or `false`

      \pre `query_index >= 0 && query_index < faces(pmesh).size()`
    */
    bool is_part_of_region(
      const std::size_t,
      const std::size_t query_index, 
      const std::vector<std::size_t>&) const {

      CGAL_precondition(query_index >= 0);
      CGAL_precondition(query_index < m_face_range.size());

      const auto face = *(m_face_range.begin() + query_index);
                
      Vector_3 face_normal;
      get_face_normal(face, face_normal);

      const FT distance_to_fitted_plane = 
      get_max_face_distance(face);
      
      const FT cos_value = 
      CGAL::abs(m_scalar_product_3(face_normal, m_normal_of_best_fit));

      return (( distance_to_fitted_plane <= m_distance_threshold ) && 
        ( cos_value >= m_normal_threshold ));
    }

    /*!
      \brief implements `RegionType::is_valid_region()`.

      This function controls if the `region` contains at least `min_region_size` faces.

      \param region
      indices of faces included in the region

      \return Boolean `true` or `false`
    */
    inline bool is_valid_region(const std::vector<std::size_t>& region) const {
      return ( region.size() >= m_min_region_size );
    }

    /*!
      \brief implements `RegionType::update()`.

      This function fits the least squares plane to all vertices of the faces 
      from the `region`.

      \param region
      indices of faces included in the region

      \pre `region.size() > 0`
    */
    void update(const std::vector<std::size_t>& region) {

      CGAL_precondition(region.size() > 0);
      if (region.size() == 1) { // create new reference plane and normal

        CGAL_precondition(region[0] >= 0);
        CGAL_precondition(region[0] < m_face_range.size());

        // The best fit plane will be a plane through this face centroid with 
        // its normal being the face's normal.
        const auto face = *(m_face_range.begin() + region[0]);
        Point_3 face_centroid;

        get_face_centroid(face, face_centroid);
        get_face_normal(face, m_normal_of_best_fit);

        m_plane_of_best_fit = 
        Plane_3(face_centroid, m_normal_of_best_fit);

      } else { // update reference plane and normal

        std::vector<Local_point_3> points;
        for (std::size_t i = 0; i < region.size(); ++i) {

          CGAL_precondition(region[i] >= 0);
          CGAL_precondition(region[i] < m_face_range.size());

          const auto face = *(m_face_range.begin() + region[i]);
          const auto hedge = halfedge(face, m_face_graph);

          const auto vertices = vertices_around_face(hedge, m_face_graph);
          for (const auto vertex : vertices) {
                            
            const Point_3& tmp_point = get(m_vertex_to_point_map, vertex);
            points.push_back(m_to_local_converter(tmp_point));
          }
        }
        CGAL_postcondition(points.size() > 0);

        Local_plane_3 fitted_plane;
        Local_point_3 fitted_centroid;

        // The best fit plane will be a plane fitted to all vertices of all
        // region faces with its normal being perpendicular to the plane.
        CGAL::linear_least_squares_fitting_3(
          points.begin(), points.end(), 
          fitted_plane, fitted_centroid, 
          CGAL::Dimension_tag<0>());

        m_plane_of_best_fit = 
        Plane_3(
          static_cast<FT>(fitted_plane.a()), 
          static_cast<FT>(fitted_plane.b()), 
          static_cast<FT>(fitted_plane.c()), 
          static_cast<FT>(fitted_plane.d()));

        const Vector_3 normal = m_plane_of_best_fit.orthogonal_vector();
        const FT normal_length = m_sqrt(m_squared_length_3(normal));

        CGAL_precondition(normal_length > FT(0));
        m_normal_of_best_fit = normal / normal_length;
      }
    }

    /// @}

  private:

    template<typename Face>  
    void get_face_centroid(
      const Face& face, 
      Point_3& face_centroid) const {

      const auto hedge = halfedge(face, m_face_graph);
      const auto vertices = vertices_around_face(hedge, m_face_graph);

      // Compute centroid.
      FT sum = FT(0);

      FT x = FT(0);
      FT y = FT(0);
      FT z = FT(0);
                
      for (const auto vertex : vertices) {
        const Point_3& point = get(m_vertex_to_point_map, vertex);

        x += point.x();
        y += point.y();
        z += point.z();

        sum += FT(1);
      }
      
      CGAL_precondition(sum > FT(0));
      x /= sum;
      y /= sum;
      z /= sum;

      face_centroid = Point_3(x, y, z);
    }

    template<typename Face>
    void get_face_normal(
      const Face& face, 
      Vector_3& face_normal) const {

      // Compute normal of the face.
      const auto hedge = halfedge(face, m_face_graph);
      const auto vertices = vertices_around_face(hedge, m_face_graph);

      CGAL_precondition(vertices.size() >= 3);

      auto vertex = vertices.begin();
      const Point_3& point1 = get(m_vertex_to_point_map, *vertex); ++vertex;
      const Point_3& point2 = get(m_vertex_to_point_map, *vertex); ++vertex;
      const Point_3& point3 = get(m_vertex_to_point_map, *vertex);

      const Vector_3 tmp_normal = CGAL::normal(point1, point2, point3);
      const FT tmp_normal_length = m_sqrt(m_squared_length_3(tmp_normal));

      CGAL_precondition(tmp_normal_length > FT(0));
      face_normal = tmp_normal / tmp_normal_length;
    }

    // The maximum distance from the vertices of the face to the best fit plane.
    template<typename Face>
    FT get_max_face_distance(const Face& face) const {

      const auto hedge = halfedge(face, m_face_graph);
      const auto vertices = vertices_around_face(hedge, m_face_graph);

      FT max_face_distance = -FT(1);
      for (const auto vertex : vertices) {
        
        const Point_3& point = 
        get(m_vertex_to_point_map, vertex);

        const FT distance = 
        m_sqrt(m_squared_distance_3(point, m_plane_of_best_fit));
        
        max_face_distance = 
        CGAL::max(distance, max_face_distance);
      }
      CGAL_postcondition(max_face_distance != -FT(1));

      return max_face_distance;
    }

    // Fields.
    const Face_graph& m_face_graph;
    const Face_range m_face_range;

    const FT m_distance_threshold;
    const FT m_normal_threshold;
    const std::size_t m_min_region_size;
            
    const Vertex_to_point_map m_vertex_to_point_map;

    const Squared_length_3 m_squared_length_3;
    const Squared_distance_3 m_squared_distance_3;
    const Scalar_product_3 m_scalar_product_3;
    const Sqrt m_sqrt;

    const To_local_converter m_to_local_converter;

    Plane_3 m_plane_of_best_fit;
    Vector_3 m_normal_of_best_fit;
  };

} // namespace Polygon_mesh
} // namespace Shape_detection
} // namespace CGAL

#endif // CGAL_SHAPE_DETECTION_REGION_GROWING_POLYGON_MESH_LEAST_SQUARES_PLANE_FIT_REGION_H
