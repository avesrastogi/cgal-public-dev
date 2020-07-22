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
// Author(s)     : Jean-Philippe Bauchet, Florent Lafarge, Gennadii Sytov, Dmitry Anisimov
//

#ifndef CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H
#define CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H

// #include <CGAL/license/Shape_regularization.h>

// Boost includes.
#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/boost/graph/Named_function_parameters.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/Segment_wrapper_2.h>
#include <CGAL/Shape_regularization/internal/Orthogonal_groups_2.h>
#include <CGAL/Shape_regularization/internal/Parallel_groups_2.h>

namespace CGAL {
namespace Shape_regularization {
namespace Segments {

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief An angle-based regularization type for 2D segments that reinforces
    parallelism and orthogonality relationships.

    \tparam GeomTraits
    must be a model of `Kernel`.

    \tparam InputRange
    must be a model of `Range` whose iterator type is `RandomAccessIterator`.

    \tparam SegmentMap
    must be a model of `LvaluePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Segment_2`. %Default is the
    `CGAL::Identity_property_map<typename GeomTraits::Segment_2>`.

    \cgalModels `RegularizationType`
  */
  template<
  typename GeomTraits,
  typename InputRange,
  typename SegmentMap = CGAL::Identity_property_map<typename GeomTraits::Segment_2> >
  class Angle_regularization_2 {
  public:

    /// \name Types
    /// @{

    /// \cond SKIP_IN_MANUAL
    using Traits = GeomTraits;
    using Input_range = InputRange;
    using Segment_map = SegmentMap;
    /// \endcond

    /// Number type.
    typedef typename GeomTraits::FT FT;

    /// \cond SKIP_IN_MANUAL
    using Point_2 = typename Traits::Point_2;
    using Vector_2 = typename Traits::Vector_2;
    using Segment_2 = typename Traits::Segment_2;
    using Direction_2 = typename Traits::Direction_2;

    using Segment_wrapper_2 = internal::Segment_wrapper_2<Traits>;
    using Orthogonal_groups_2 = internal::Orthogonal_groups_2<Traits, Input_range, Segment_map>;
    using Parallel_groups_2 = internal::Parallel_groups_2<Traits, Input_range, Segment_map>;
    using Indices = std::vector<std::size_t>;
    /// \endcond

    /// @}

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \tparam NamedParameters
      a sequence of \ref sr_namedparameters "Named Parameters".

      \param input_range
      a range of 2D segments to be regularized

      \param np
      optional sequence of \ref sr_namedparameters "Named Parameters"
      among the ones listed below

      \param segment_map
      an instance of `SegmentMap` that maps an item from input range to `GeomTraits::Segment_2`,
      if not provided, the default is used

      \cgalNamedParamsBegin
        \cgalParamBegin{max_angle}
          max angle deviation in degrees of a segment from its initial orientation,
          the default is 25 degrees
        \cgalParamEnd
      \cgalNamedParamsEnd

      \pre input_range.size() >= 2
      \pre max_angle >= 0 && max_angle <= 90
    */
    template<typename NamedParameters>
    Angle_regularization_2(
      InputRange& input_range,
      const NamedParameters np,
      const SegmentMap segment_map = SegmentMap()) :
    m_input_range(input_range),
    m_segment_map(segment_map) {

      CGAL_precondition(
        input_range.size() >= 2);
      const FT max_angle = parameters::choose_parameter(
        parameters::get_parameter(np, internal_np::max_angle), FT(25));
      CGAL_precondition(max_angle >= FT(0) && max_angle <= FT(90));

      m_max_angle = max_angle;
      if (m_max_angle < FT(0) || m_max_angle > FT(90)) {
        std::cerr << "WARNING: The max angle bound has to be within [0, 90]! ";
        std::cerr << " Setting to the default value: 25 degrees." << std::endl;
        m_max_angle = FT(25);
      }
      clear();
      create_unique_group();
    }

    /*!
      \brief inserts a group of segments from `input_range`.

      Each group of segments is provided as a vector of their indices and only
      segments within the group are being regularized that is no relationships
      between segments from different groups are taken into account.

      The user does not have to use this method until one has well-defined
      groups of segments. By default, all segments are inserted as a group.

      \tparam IndexRange
      must be a model of `ConstRange` whose iterator type is `RandomAccessIterator`
      and value type is `std::size_t`.

      \param index_range
      a const range of segment indices

      \pre index_range.size() >= 2
    */
    template<typename IndexRange>
  	void add_group(
      const IndexRange& index_range) {

      if (m_is_first_call) {
        clear();
        m_is_first_call = false;
      }

      if (index_range.size() < 2) return;
      update_wrappers(index_range);
      ++m_num_groups;
    }

    /// @}

    /// \name Access
    /// @{

    /*!
      \brief calculates the target value between 2 segments, which are
      direct neighbors to each other.

      The target value is the angle difference between initial orientations
      of two segments `i` and `j`.

      \param i
      index of the first segment

      \param j
      index of the second segment

      \pre i >= 0 && i < input_range.size()
      \pre j >= 0 && j < input_range.size()
    */
    FT target(
      const std::size_t i,
      const std::size_t j) const {

      CGAL_precondition(i >= 0 && i < m_input_range.size());
      CGAL_precondition(j >= 0 && j < m_input_range.size());
      CGAL_assertion(m_wraps.size() == m_input_range.size());

      const auto& wrapi = m_wraps[i];
      CGAL_assertion(wrapi.is_used);
      const auto& wrapj = m_wraps[j];
      CGAL_assertion(wrapj.is_used);

      const FT angle_deg = internal::mod90_angle_difference_2(
        wrapi.orientation, wrapj.orientation);
      const FT target_value = angle_deg;
      return target_value;
    }

    /*!
      \brief returns `max_angle`.
    */
    const FT bound(const std::size_t) const {
      return m_max_angle;
    }

    /*!
      \brief applies new orientations computed by the `QPSolver`
      to the initial segments.

      Number of values in `solution` equals to the number n of segments being
      regularized + the number m of neighbor pairs between these segments. Each
      of n values is an angle that is added to the initial segment orientation.

      \param solution
      a vector with angles in degrees

      \pre solution.size() >= 1
    */
    void update(
      const std::vector<FT>& solution) {

      CGAL_precondition(solution.size() >= 1);
      m_num_modified_segments = 0;
      for (const auto& wrap : m_wraps) {
        if (!wrap.is_used) continue;

        // Get angle.
        const std::size_t seg_index = wrap.index;
        CGAL_assertion(
          seg_index >= 0 && seg_index < solution.size());
        const FT difference = solution[seg_index];
        FT angle_deg = wrap.orientation + difference;
        if (angle_deg < FT(0)) angle_deg += FT(180);
        else if (angle_deg > FT(180)) angle_deg -= FT(180);
        const double angle_rad = internal::radians_2(angle_deg);

        // Get update values.
        const FT x = static_cast<FT>(std::cos(angle_rad));
        const FT y = static_cast<FT>(std::sin(angle_rad));
        Vector_2 v = Vector_2(x, y);
        const auto direction = internal::direction_2(v);
        FT a, b, c;
        internal::line_coefficients_2(
          wrap.barycenter, direction, a, b, c);

        // Update segment.
        Segment_2 modified;
        orient_segment(
          direction, a, b, c, wrap, modified);
        put(m_segment_map,
          *(m_input_range.begin() + seg_index), modified);
        ++m_num_modified_segments;
      }
    }

    /// @}

    /// \name Groups
    /// @{

    /*!
      \brief returns indices of parallel segments organized into groups.

      This method calls `CGAL::Shape_regularization::Segments::parallel_groups()`.

      \tparam OutputIterator
      must be a model of `OutputIterator`

      \param groups
      an instance of `OutputIterator`,
      whose value type is `std::vector<std::size_t>`
    */
    template<typename OutputIterator>
    OutputIterator parallel_groups(OutputIterator groups) const {

      const Parallel_groups_2 grouping(
        m_input_range,
        CGAL::parameters::max_angle(m_max_angle),
        m_segment_map, Traits());
      return grouping.groups(groups);
    }

    /*!
      \brief returns indices of orthogonal segments organized into groups.

      This method calls `CGAL::Shape_regularization::Segments::orthogonal_groups()`.

      \tparam OutputIterator
      must be a model of `OutputIterator`

      \param groups
      an instance of `OutputIterator`,
      whose value type is `std::vector<std::size_t>`
    */
    template<typename OutputIterator>
    OutputIterator orthogonal_groups(OutputIterator groups) const {

      const Orthogonal_groups_2 grouping(
        m_input_range,
        CGAL::parameters::max_angle(m_max_angle),
        m_segment_map, Traits());
      return grouping.groups(groups);
    }

    /// @}

    /// \name Miscellaneous
    /// @{

    /*!
      \brief returns the number of modified segments.
    */
    std::size_t number_of_modified_segments() const {
      return m_num_modified_segments;
    }

    /// @}

    /// \name Memory Management
    /// @{

    /*!
      \brief clears all internal data structures.
    */
    void clear() {
      m_num_groups = 0;
      m_is_first_call = true;
      m_num_modified_segments = 0;
      for (auto& wrap : m_wraps)
        wrap.is_used = false;
    }

    /// @}

    // EXTRA METHODS TO TEST THE CLASS!
    /// \cond SKIP_IN_MANUAL
    const std::size_t number_of_groups() const {
      return m_num_groups;
    }
    /// \endcond

  private:
    Input_range& m_input_range;
    const Segment_map m_segment_map;

    FT m_max_angle;
    std::vector<Segment_wrapper_2> m_wraps;

    std::size_t m_num_modified_segments;
    std::size_t m_num_groups;
    bool m_is_first_call;

    void create_unique_group() {

      CGAL_precondition(m_input_range.size() >= 2);
      if (m_input_range.size() < 2) return;

      m_wraps.clear();
      m_wraps.resize(m_input_range.size());

      Indices group(m_input_range.size());
      std::iota(group.begin(), group.end(), 0);
      update_segment_data(group);
      m_num_groups = 1;
    }

    template<typename IndexRange>
    void update_segment_data(
      const IndexRange& index_range) {

      for (const auto seg_index : index_range) {
        CGAL_assertion(
          seg_index >= 0 && seg_index < m_wraps.size());
        auto& wrap = m_wraps[seg_index];

        const auto& segment = get(m_segment_map,
          *(m_input_range.begin() + seg_index));
        wrap.set_qp(seg_index, segment);
      }
    }

    template<typename IndexRange>
    void update_wrappers(
      const IndexRange& index_range) {

      for (const auto seg_index : index_range) {
        CGAL_assertion(
          seg_index >= 0 && seg_index < m_wraps.size());
        auto& wrap = m_wraps[seg_index];

        wrap.is_used = true;
      }
    }

    void orient_segment(
      const Direction_2& direction,
      const FT a, const FT b, const FT c,
      const Segment_wrapper_2& wrap,
      Segment_2& modified) const {

      FT x1, y1, x2, y2;
      if (
        CGAL::abs(direction.dx()) >
        CGAL::abs(direction.dy())) {

        x1 = wrap.barycenter.x() - wrap.length * direction.dx() / FT(2);
        x2 = wrap.barycenter.x() + wrap.length * direction.dx() / FT(2);
        y1 = (-c - a * x1) / b;
        y2 = (-c - a * x2) / b;
      }  else {
        y1 = wrap.barycenter.y() - wrap.length * direction.dy() / FT(2);
        y2 = wrap.barycenter.y() + wrap.length * direction.dy() / FT(2);
        x1 = (-c - b * y1) / a;
        x2 = (-c - b * y2) / a;
      }
      const Point_2 source = Point_2(x1, y1);
      const Point_2 target = Point_2(x2, y2);
      modified = Segment_2(source, target);
    }
  };

} // namespace Segments
} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_ANGLE_REGULARIZATION_2_H
