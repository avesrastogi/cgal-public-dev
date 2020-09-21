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

#ifndef CGAL_SHAPE_REGULARIZATION_REGULARIZE_SEGMENTS_H
#define CGAL_SHAPE_REGULARIZATION_REGULARIZE_SEGMENTS_H

// #include <CGAL/license/Shape_regularization.h>

// Boost includes.
#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/boost/graph/Named_function_parameters.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/utils.h>
#include <CGAL/Shape_regularization/internal/Parallel_groups_2.h>
#include <CGAL/Shape_regularization/internal/Orthogonal_groups_2.h>
#include <CGAL/Shape_regularization/internal/Collinear_groups_2.h>

#if defined(CGAL_USE_OSQP)
#include <CGAL/OSQP_quadratic_program_traits.h>
#endif // CGAL_USE_OSQP

#include <CGAL/Shape_regularization/QP_regularization.h>
#include <CGAL/Shape_regularization/Segments/Angle_regularization_2.h>
#include <CGAL/Shape_regularization/Segments/Offset_regularization_2.h>
#include <CGAL/Shape_regularization/Segments/Delaunay_neighbor_query_2.h>

namespace CGAL {
namespace Shape_regularization {
namespace Segments {

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief regularizes a set of 2D segments.

    Given a set of unordered 2D segments, this function enables to reinforce
    three types of regularities among these segments:
    - *Parallelism*: segments, which are detected as near parallel, are made exactly parallel.
    - *Orthogonality*: segments, which are detected as near orthogonal, are made exactly orthogonal.
    - *Collinearity*: parallel segments, which are detected as near collinear, are made exactly collinear.

    The user has to provide a `NeighborQuery` model to access local neighbors
    of a segment and a `RegularizationType` model to define the type of regularities
    that should be addressed. The function is based on the class `QP_regularization`.
    Please refer to that class and these concepts for more information.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam NeighQuery
    a model of `NeighborQuery`

    \tparam RegType
    a model of `RegularizationType`

    \tparam QPSolver
    a model of `QuadraticProgramTraits`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments for shape regularization

    \param neighbor_query
    an instance of `NeighQuery` that is used internally to
    access neighbors of a segment; this parameter can be omitted together
    with the `regularization_type` parameter, in this case, all types of regularities
    will be reinforced on the whole input range

    \param regularization_type
    an instance of `RegType` that is used internally to
    obtain bounds and target values required by the regularization;
    this parameter can be omitted together with the `neighbor_query` parameter,
    in this case, all types of regularities will be reinforced on the whole input range

    \param quadratic_program
    an instance of `QPSolver` to solve the quadratic programming problem;
    this parameter can be omitted, the default solver is `CGAL::OSQP_quadratic_program_traits`

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \pre input_range.size() >= 2

    \sa `regularize_angles()`
    \sa `regularize_offsets()`
  */
  template<
  typename InputRange,
  typename NeighQuery,
  typename RegType,
  typename QPSolver,
  typename GeomTraits>
  void regularize_segments(
    InputRange& input_range,
    NeighQuery& neighbor_query,
    RegType& regularization_type,
    QPSolver& quadratic_program,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 2);
    using Regularizer = QP_regularization<
      GeomTraits, InputRange, NeighQuery, RegType, QPSolver>;

    Regularizer regularizer(
      input_range, neighbor_query, regularization_type, quadratic_program, traits);
    regularizer.regularize();
  }

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief regularizes angles in a set of 2D segments.

    Given a set of unordered 2D segments, this function enables to reinforce
    two types of regularities among these segments:
    - *Parallelism*: segments, which are detected as near parallel, are made exactly parallel.
    - *Orthogonality*: segments, which are detected as near orthogonal, are made exactly orthogonal.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments for angle regularization

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \pre input_range.size() >= 2

    \sa `regularize_segments()`
    \sa `regularize_offsets()`
  */
  template<
  typename InputRange,
  typename GeomTraits>
  void regularize_angles(
    InputRange& input_range,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 2);
    using Indices = std::vector<std::size_t>;
    using Neighbor_query = Delaunay_neighbor_query_2<GeomTraits, InputRange>;
    using Angle_regularization = Angle_regularization_2<GeomTraits, InputRange>;

    Neighbor_query neighbor_query(input_range);
    Angle_regularization angle_regularization(input_range);
    regularize_segments(
      input_range, neighbor_query, angle_regularization, traits);
  }

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief regularizes offsets in a set of 2D segments.

    Given a set of parallel 2D segments, this function enables to reinforce
    the collinearity property among these segments that is all parallel segments,
    which are detected as near collinear, are made exactly collinear.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments for offset regularization

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \pre input_range.size() >= 2

    \sa `regularize_segments()`
    \sa `regularize_angles()`
  */
  template<
  typename InputRange,
  typename GeomTraits>
  void regularize_offsets(
    InputRange& input_range,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 2);
    using Indices = std::vector<std::size_t>;
    using Neighbor_query = Delaunay_neighbor_query_2<GeomTraits, InputRange>;
    using Offset_regularization = Offset_regularization_2<GeomTraits, InputRange>;

    Neighbor_query neighbor_query(input_range);
    Offset_regularization offset_regularization(input_range);
    regularize_segments(
      input_range, neighbor_query, offset_regularization, traits);
  }

  #if defined(CGAL_USE_OSQP) || defined(DOXYGEN_RUNNING)

  /// \cond SKIP_IN_MANUAL
  template<
  typename InputRange,
  typename NeighQuery,
  typename RegType,
  typename GeomTraits>
  void regularize_segments(
    InputRange& input_range,
    NeighQuery& neighbor_query,
    RegType& regularization_type,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 2);
    using FT = typename GeomTraits::FT;
    using QP = CGAL::OSQP_quadratic_program_traits<FT>;
    QP quadratic_program;

    regularize_segments(
      input_range, neighbor_query, regularization_type, quadratic_program, traits);
  }

  template<
  typename InputRange,
  typename NeighQuery,
  typename RegType>
  void regularize_segments(
    InputRange& input_range,
    NeighQuery& neighbor_query,
    RegType& regularization_type) {

    CGAL_precondition(input_range.size() >= 2);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    using FT = typename GeomTraits::FT;
    using QP = CGAL::OSQP_quadratic_program_traits<FT>;
    QP quadratic_program;

    regularize_segments(
      input_range, neighbor_query, regularization_type, quadratic_program, traits);
  }

  template<
  typename InputRange,
  typename GeomTraits>
  void regularize_segments(
    InputRange& input_range,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 2);
    using Indices = std::vector<std::size_t>;
    using Neighbor_query = Delaunay_neighbor_query_2<GeomTraits, InputRange>;
    using Angle_regularization = Angle_regularization_2<GeomTraits, InputRange>;
    using Offset_regularization = Offset_regularization_2<GeomTraits, InputRange>;

    // Regularize angles.
    Neighbor_query neighbor_query(input_range);
    Angle_regularization angle_regularization(input_range);
    regularize_segments(
      input_range, neighbor_query, angle_regularization, traits);

    std::vector<Indices> parallel_groups;
    angle_regularization.parallel_groups(
      std::back_inserter(parallel_groups));

    // Regularize offsets.
    Offset_regularization offset_regularization(input_range);
    neighbor_query.clear();
    for (const auto& parallel_group : parallel_groups) {
      neighbor_query.add_group(parallel_group);
      offset_regularization.add_group(parallel_group);
    }
    regularize_segments(
      input_range, neighbor_query, offset_regularization, traits);
  }

  template<typename InputRange>
  void regularize_segments(
    InputRange& input_range) {

    CGAL_precondition(input_range.size() >= 2);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    regularize_segments(input_range, traits);
  }

  template<typename InputRange>
  void regularize_angles(
    InputRange& input_range) {

    CGAL_precondition(input_range.size() >= 2);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    regularize_angles(input_range, traits);
  }

  template<typename InputRange>
  void regularize_offsets(
    InputRange& input_range) {

    CGAL_precondition(input_range.size() >= 2);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    regularize_offsets(input_range, traits);
  }
  /// \endcond

  #endif // CGAL_USE_OSQP or DOXYGEN_RUNNING

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief finds groups of parallel segments in a set of 2D segments.

    This function enables to find groups of near parallel segments
    in a set of 2D segments. The groups are returned as vectors of indices.
    Note that two segments may be included at the same group even if they are
    far away from each other. This algorithm concerns only the angle relationship
    among segments, but not the distance.

    This function does not regularize input segments, but only groups them.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam OutIterator
    a model of `OutputIterator` that accepts elements of type `std::vector<std::size_t>`

    \tparam NamedParameters
    a sequence of \ref bgl_namedparameters "Named Parameters"

    \tparam SegmentMap
    a model of `ReadablePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Segment_2`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments

    \param groups
    an output iterator with groups of segment indices

    \param np
    an optional sequence of \ref bgl_namedparameters "Named Parameters"
    among the ones listed below; this parameter can be omitted,
    the default values are then used

    \param segment_map
    an instance of `SegmentMap`; this parameter can be omitted,
    the identity map `CGAL::Identity_property_map` is then used

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \cgalNamedParamsBegin
      \cgalParamNBegin{max_angle}
        \cgalParamDescription{maximum allowed angle deviation in degrees between two segments
          such that they are considered to be parallel}
        \cgalParamType{`GeomTraits::FT`}
        \cgalParamDefault{5 degrees}
      \cgalParamNEnd
      \cgalParamNBegin{preserve_order}
        \cgalParamDescription{indicates whether the order of input segments should be
          preserved or not}
        \cgalParamType{boolean}
        \cgalParamDefault{false}
      \cgalParamNEnd
    \cgalNamedParamsEnd

    \return an output iterator to the element in the destination range,
    one past the last group stored

    \pre input_range.size() >= 1
    \pre max_angle >= 0 && max_angle <= 90
  */
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap,
  typename GeomTraits>
  OutIterator parallel_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 1);
    using Parallel_groups_2 = internal::Parallel_groups_2<
      GeomTraits, InputRange, SegmentMap>;

    Parallel_groups_2 grouping(
      input_range, np, segment_map, traits);
    return grouping.groups(groups);
  }

  /// \cond SKIP_IN_MANUAL
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap = CGAL::Identity_property_map<
  typename std::iterator_traits< typename InputRange::const_iterator >::value_type > >
  OutIterator parallel_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map = SegmentMap()) {

    CGAL_precondition(input_range.size() >= 1);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    return parallel_groups(
      input_range, groups, np, segment_map, traits);
  }

  template<
  typename InputRange,
  typename OutIterator>
  OutIterator parallel_groups(
    const InputRange& input_range,
    OutIterator groups) {

    CGAL_precondition(input_range.size() >= 1);
    return parallel_groups(
      input_range, groups, CGAL::parameters::all_default());
  }
  /// \endcond

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief finds groups of collinear segments in a set of 2D segments.

    This function enables to find groups of near collinear segments
    in a set of 2D segments. The groups are returned as vectors of indices.
    This algorithm first finds the groups of parallel segments using the function
    `Segments::parallel_groups()` and then splits these groups into groups of collinear segments.

    This function does not regularize input segments, but only groups them.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam OutIterator
    a model of `OutputIterator` that accepts elements of type `std::vector<std::size_t>`

    \tparam NamedParameters
    a sequence of \ref bgl_namedparameters "Named Parameters"

    \tparam SegmentMap
    a model of `ReadablePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Segment_2`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments

    \param groups
    an output iterator with groups of segment indices

    \param np
    an optional sequence of \ref bgl_namedparameters "Named Parameters"
    among the ones listed below; this parameter can be omitted,
    the default values are then used

    \param segment_map
    an instance of `SegmentMap`; this parameter can be omitted,
    the identity map `CGAL::Identity_property_map` is then used

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \cgalNamedParamsBegin
      \cgalParamNBegin{max_offset}
        \cgalParamDescription{maximum allowed orthogonal distance between two parallel segments
          such that they are considered to be collinear}
        \cgalParamType{`GeomTraits::FT`}
        \cgalParamDefault{0.2 unit length}
      \cgalParamNEnd
      \cgalParamNBegin{preserve_order}
        \cgalParamDescription{indicates whether the order of input segments should be
          preserved or not}
        \cgalParamType{boolean}
        \cgalParamDefault{false}
      \cgalParamNEnd
    \cgalNamedParamsEnd

    \return an output iterator to the element in the destination range,
    one past the last group stored

    \pre input_range.size() >= 1
    \pre max_offset >= 0
  */
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap,
  typename GeomTraits>
  OutIterator collinear_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 1);
    using Collinear_groups_2 = internal::Collinear_groups_2<
      GeomTraits, InputRange, SegmentMap>;

    Collinear_groups_2 grouping(
      input_range, np, segment_map, traits);
    return grouping.groups(groups);
  }

  /// \cond SKIP_IN_MANUAL
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap = CGAL::Identity_property_map<
  typename std::iterator_traits< typename InputRange::const_iterator >::value_type > >
  OutIterator collinear_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map = SegmentMap()) {

    CGAL_precondition(input_range.size() >= 1);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    return collinear_groups(
      input_range, groups, np, segment_map, traits);
  }

  template<
  typename InputRange,
  typename OutIterator>
  OutIterator collinear_groups(
    const InputRange& input_range,
    OutIterator groups) {

    CGAL_precondition(input_range.size() >= 1);
    return collinear_groups(
      input_range, groups, CGAL::parameters::all_default());
  }
  /// \endcond

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief finds groups of orthogonal segments in a set of 2D segments.

    This function enables to find groups of near orthogonal segments
    in a set of 2D segments. The groups are returned as vectors of indices.
    This algorithm first finds the groups of parallel segments using the function
    `Segments::parallel_groups()` and then merges these groups into groups of orthogonal segments.

    This function does not regularize input segments, but only groups them.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam OutIterator
    a model of `OutputIterator` that accepts elements of type `std::vector<std::size_t>`

    \tparam NamedParameters
    a sequence of \ref bgl_namedparameters "Named Parameters"

    \tparam SegmentMap
    a model of `ReadablePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Segment_2`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments

    \param groups
    an output iterator with groups of segment indices

    \param np
    an optional sequence of \ref bgl_namedparameters "Named Parameters"
    among the ones listed below; this parameter can be omitted,
    the default values are then used

    \param segment_map
    an instance of `SegmentMap`; this parameter can be omitted,
    the identity map `CGAL::Identity_property_map` is then used

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \cgalNamedParamsBegin
      \cgalParamNBegin{max_angle}
        \cgalParamDescription{maximum allowed angle deviation in degrees between two segments
          such that they are considered to be parallel or orthogonal}
        \cgalParamType{`GeomTraits::FT`}
        \cgalParamDefault{5 degrees}
      \cgalParamNEnd
      \cgalParamNBegin{preserve_order}
        \cgalParamDescription{indicates whether the order of input segments should be
          preserved or not}
        \cgalParamType{boolean}
        \cgalParamDefault{false}
      \cgalParamNEnd
    \cgalNamedParamsEnd

    \return an output iterator to the element in the destination range,
    one past the last group stored

    \pre input_range.size() >= 1
    \pre max_angle >= 0 && max_angle <= 90
  */
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap,
  typename GeomTraits>
  OutIterator orthogonal_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 1);
    using Orthogonal_groups_2 = internal::Orthogonal_groups_2<
      GeomTraits, InputRange, SegmentMap>;

    Orthogonal_groups_2 grouping(
      input_range, np, segment_map, traits);
    return grouping.groups(groups);
  }

  /// \cond SKIP_IN_MANUAL
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap = CGAL::Identity_property_map<
  typename std::iterator_traits< typename InputRange::const_iterator >::value_type > >
  OutIterator orthogonal_groups(
    const InputRange& input_range,
    OutIterator groups,
    const NamedParameters& np,
    const SegmentMap segment_map = SegmentMap()) {

    CGAL_precondition(input_range.size() >= 1);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    return orthogonal_groups(
      input_range, groups, np, segment_map, traits);
  }

  template<
  typename InputRange,
  typename OutIterator>
  OutIterator orthogonal_groups(
    const InputRange& input_range,
    OutIterator groups) {

    CGAL_precondition(input_range.size() >= 1);
    return orthogonal_groups(
      input_range, groups, CGAL::parameters::all_default());
  }
  /// \endcond

  /*!
    \ingroup PkgShapeRegularizationRefSegments

    \brief substitutes groups of 2D collinear segments by average segments.

    This function first calls `Segments::collinear_groups()`
    and then substitutes each group of collinear segments by an average segment.
    The number of returned segments is the number of detected collinear groups.

    This function does not regularize input segments, but only groups and then simplifies them.

    \tparam InputRange
    a model of `ConstRange` whose iterator type is `RandomAccessIterator`

    \tparam OutIterator
    a model of `OutputIterator` that accepts segments of type `GeomTraits::Segment_2`

    \tparam NamedParameters
    a sequence of \ref bgl_namedparameters "Named Parameters"

    \tparam SegmentMap
    a model of `ReadablePropertyMap` whose key type is the value type of the input
    range and value type is `GeomTraits::Segment_2`

    \tparam GeomTraits
    a model of `Kernel`

    \param input_range
    a const range of input segments

    \param segments
    an output iterator with the simplified segments

    \param np
    an optional sequence of \ref bgl_namedparameters "Named Parameters"
    among the ones listed below; this parameter can be omitted,
    the default values are then used

    \param segment_map
    an instance of `SegmentMap`; this parameter can be omitted,
    the identity map `CGAL::Identity_property_map` is then used

    \param traits
    an instance of `GeomTraits`; this parameter can be omitted if the traits class
    can be deduced from the input value type

    \cgalNamedParamsBegin
      \cgalParamNBegin{max_offset}
        \cgalParamDescription{maximum allowed orthogonal distance between two parallel segments
          such that they are considered to be collinear}
        \cgalParamType{`GeomTraits::FT`}
        \cgalParamDefault{0.2 unit length}
      \cgalParamNEnd
      \cgalParamNBegin{preserve_order}
        \cgalParamDescription{indicates whether the order of input segments should be
          preserved or not}
        \cgalParamType{boolean}
        \cgalParamDefault{false}
      \cgalParamNEnd
    \cgalNamedParamsEnd

    \return an output iterator to the element in the destination range,
    one past the last segment stored

    \pre input_range.size() >= 1
    \pre max_offset >= 0
  */
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap,
  typename GeomTraits>
  OutIterator unique_segments(
    const InputRange& input_range,
    OutIterator segments,
    const NamedParameters& np,
    const SegmentMap segment_map,
    const GeomTraits& traits) {

    CGAL_precondition(input_range.size() >= 1);
    using Unique_segments_2 = internal::Unique_segments_2<
      GeomTraits, InputRange, SegmentMap>;

    const Unique_segments_2 unique(
      input_range, np, segment_map, traits);
    return unique.segments(segments);
  }

  /// \cond SKIP_IN_MANUAL
  template<
  typename InputRange,
  typename OutIterator,
  typename NamedParameters,
  typename SegmentMap = CGAL::Identity_property_map<
  typename std::iterator_traits< typename InputRange::const_iterator >::value_type > >
  OutIterator unique_segments(
    const InputRange& input_range,
    OutIterator segments,
    const NamedParameters& np,
    const SegmentMap segment_map = SegmentMap()) {

    CGAL_precondition(input_range.size() >= 1);
    using Iterator_type = typename InputRange::const_iterator;
    using Segment_2 = typename std::iterator_traits<Iterator_type>::value_type;
    using GeomTraits = typename Kernel_traits<Segment_2>::Kernel;
    GeomTraits traits;

    return unique_segments(
      input_range, segments, np, segment_map, traits);
  }

  template<
  typename InputRange,
  typename OutIterator>
  OutIterator unique_segments(
    const InputRange& input_range,
    OutIterator segments) {

    CGAL_precondition(input_range.size() >= 1);
    return unique_segments(
      input_range, segments, CGAL::parameters::all_default());
  }
  /// \endcond

} // namespace Segments
} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_REGULARIZE_SEGMENTS_H
