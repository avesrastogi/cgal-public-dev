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
// Author(s)     : Dmitry Anisimov, Simon Giraudot
//

#ifndef CGAL_SHAPE_REGULARIZATION_USER_DEFINED_PRINCIPAL_DIRECTIONS_2_H
#define CGAL_SHAPE_REGULARIZATION_USER_DEFINED_PRINCIPAL_DIRECTIONS_2_H

// #include <CGAL/license/Shape_regularization.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/Contour_base_2.h>

namespace CGAL {
namespace Shape_regularization {
namespace Contours {

  /*!
    \ingroup PkgShapeRegularizationRefContours

    \brief Sets multiple user-specified principal directions of the contour.

    This algorithm finds the best-fit edges of the contour with respect to the
    user-specified principal directions and sets all other necessary data.

    \tparam GeomTraits
    a model of `Kernel`.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`.

    \tparam PointMap
    a model of `ReadablePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Point_2`. The default is
    `CGAL::Identity_property_map<typename GeomTraits::Point_2>`.

    \cgalModels `ContourDirections`
  */
  template<
  typename GeomTraits,
  typename InputRange,
  typename PointMap = CGAL::Identity_property_map<typename GeomTraits::Point_2> >
  class User_defined_directions_2 {

  public:
    /// \cond SKIP_IN_MANUAL
    using Traits = GeomTraits;
    using Input_range = InputRange;
    using Point_map = PointMap;

    using FT = typename Traits::FT;
    using Segment_2 = typename Traits::Segment_2;
    using Direction_2 = typename Traits::Direction_2;

    using FT_pair = std::pair<FT, FT>;
    using Base = internal::Contour_base_2<Traits>;
    using Segment_wrapper_2 = typename Base::Segment_wrapper_2;
    /// \endcond

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \tparam DirectionRange
      a model of `ConstRange` whose value type is `GeomTraits::Direction_2`.

      \param input_range
      a const range of ordered 2D points, which form a contour

      \param is_closed
      indicates whether the contour is closed or open

      \param direction_range
      a const range with user-specified principal directions

      \param point_map
      an instance of `PointMap` that maps an item from input range to `GeomTraits::Point_2`;
      this parameter can be omitted, the identity map `CGAL::Identity_property_map` is then used

      \pre direction_range.size() >= 1
      \pre direction_range.size() == input_range.size() for closed contours
      \pre direction_range.size() == input_range.size() - 1 for open contours

      \pre input_range.size() >= 3 for closed contours
      \pre input_range.size() >= 2 for open contours
    */
    template<typename DirectionRange>
    User_defined_directions_2(
      const InputRange& input_range,
      const bool is_closed,
      const DirectionRange& direction_range,
      const PointMap point_map = PointMap()) :
    m_input_range(input_range),
    m_point_map(point_map),
    m_max_angle_2(FT(5)) {

      CGAL_precondition(input_range.size() >= 2);
      CGAL_precondition(direction_range.size() >= 1);

      if (is_closed)
        estimate_closed(
          direction_range, m_bounds, m_directions, m_assigned);
      else
        estimate_open(
          direction_range, m_bounds, m_directions, m_assigned);

      if (verbose()) {
        std::cout << "* assigned directions: ";
        for (std::size_t direction_index : m_assigned)
          std::cout << direction_index << " ";
        std::cout << std::endl;
      }
    }

    /// @}

    /// \name Directions
    /// @{

    /*!
      \brief orients a given `segment` with the index `query_index` towards the
      best-fit user-specified principal direction.

      \param query_index an index of the contour vertex that emits the contour
      edge being `segment`.

      \param segment a segment to be rotated

      \pre query_index >= 0 && query_index < input_range.size() for closed contours
      \pre query_index >= 0 && query_index < input_range.size() - 1 for open contours
    */
    void orient(
      const std::size_t query_index,
      Segment_2& segment) const {

      m_base.apply_rotation_to_segment(
        m_bounds, m_directions, m_assigned,
        query_index, segment);
    }

    /// @}

    /// \name Miscellaneous
    /// @{

    /*!
      \brief returns the number of principal directions of the contour.

      The returned number equals to the number of the user-specified directions.
    */
    const std::size_t number_of_directions() const {
      return m_directions.size();
    }

    /// @}

    // EXTRA METHODS TO TEST THE CLASS!
    /// \cond SKIP_IN_MANUAL
    const std::vector<Direction_2>& get_directions() const {
      return m_directions;
    }
    /// \endcond

  private:
    const Input_range& m_input_range;
    const Point_map m_point_map;
    const Base m_base;

    const FT m_max_angle_2;

    std::vector<FT_pair> m_bounds;
    std::vector<Direction_2> m_directions;
    std::vector<std::size_t> m_assigned;

    const bool verbose() const {
      return m_base.verbose();
    }

    template<typename DirectionRange>
    void estimate_closed(
      const DirectionRange& direction_range,
      std::vector<FT_pair>& bounds,
      std::vector<Direction_2>& directions,
      std::vector<std::size_t>& assigned) const {

      CGAL_precondition(direction_range.size() >= 1);
      if (direction_range.size() == 0) return;

      std::vector<Segment_wrapper_2> wraps;
      m_base.initialize_closed(
        m_input_range, m_point_map, wraps);
      initialize_directions(direction_range, directions);

      bounds.clear(); bounds.reserve(directions.size());
      for (std::size_t i = 0; i < directions.size(); ++i)
        bounds.push_back(std::make_pair(FT(45), FT(45)));

      assigned.clear(); assigned.resize(wraps.size());
      set_directions(directions, wraps, assigned);

      m_base.unify_along_contours_closed(wraps, assigned);
      m_base.correct_directions_closed(wraps, assigned);
    }

    template<typename DirectionRange>
    void estimate_open(
      const DirectionRange& direction_range,
      std::vector<FT_pair>& bounds,
      std::vector<Direction_2>& directions,
      std::vector<std::size_t>& assigned) const {

      CGAL_precondition(direction_range.size() >= 1);
      if (direction_range.size() == 0) return;

      std::vector<Segment_wrapper_2> wraps;
      m_base.initialize_open(
        m_input_range, m_point_map, wraps);
      initialize_directions(direction_range, directions);

      bounds.clear(); bounds.reserve(directions.size());
      for (std::size_t i = 0; i < directions.size(); ++i)
        bounds.push_back(std::make_pair(FT(45), FT(45)));

      assigned.clear(); assigned.resize(wraps.size());
      set_directions(directions, wraps, assigned);

      m_base.unify_along_contours_open(wraps, assigned);
      m_base.correct_directions_open(wraps, assigned);
    }

    template<typename DirectionRange>
    void initialize_directions(
      const DirectionRange& direction_range,
      std::vector<Direction_2>& directions) const {

      directions.clear();
      directions.reserve(direction_range.size());

      for (const auto& direction : direction_range) {
        auto v = direction.to_vector();
        internal::normalize_vector(v);
        directions.push_back(Direction_2(v.x(), v.y()));
      }
      CGAL_assertion(directions.size() == direction_range.size());
    }

    void set_directions(
      const std::vector<Direction_2>& directions,
      std::vector<Segment_wrapper_2>& wraps,
      std::vector<std::size_t>& assigned) const {

      for (auto& wrap : wraps) {
        for (std::size_t i = 0; i < directions.size(); ++i) {
          if (does_satisify_angle_conditions(
            directions[i], wrap.direction)) {
            assigned[wrap.index] = i;
            wrap.is_used = true; break;
          }
        }
      }
    }

    bool does_satisify_angle_conditions(
      const Direction_2& longest,
      const Direction_2& segment) const {

      CGAL_precondition(
        m_max_angle_2 >= FT(0) && m_max_angle_2 <= FT(90));
      const FT bound_min = m_max_angle_2;
      const FT bound_max = FT(90) - bound_min;

      const FT angle_2 = internal::angle_2(longest, segment);
      return (angle_2 <= bound_min) || (angle_2 >= bound_max);
    }
  };

} // namespace Contours
} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_USER_DEFINED_PRINCIPAL_DIRECTIONS_2_H
