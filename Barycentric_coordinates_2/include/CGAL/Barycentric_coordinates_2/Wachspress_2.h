// Copyright (c) 2014 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is a part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s) : Dmitry Anisimov, David Bommes, Kai Hormann, and Pierre Alliez.

/*!
  \file Wachspress_2.h
*/

#ifndef CGAL_WACHSPRESS_2_H
#define CGAL_WACHSPRESS_2_H

#include <CGAL/license/Barycentric_coordinates_2.h>

#include <CGAL/disable_warnings.h>

// CGAL headers.
#include <CGAL/assertions.h>
#include <CGAL/Polygon_2_algorithms.h>

// Barycentric coordinates headers.
#include <CGAL/Barycentric_coordinates_2/barycentric_enum_2.h>

// Boost headers.
#include <boost/optional/optional.hpp>

// CGAL namespace.
namespace CGAL {

// Barycentric coordinates namespace.
namespace Barycentric_coordinates {

// Examples: see the User Manual here - https://doc.cgal.org/latest/Manual/index.html.
// [1] Reference: "M. S. Floater, K. Hormann, and G. Kos. A general construction of barycentric coordinates over convex polygons. Advances in Computational Mathematics, 24(1-4):311-331, 2006.".

/*!
 * \ingroup PkgBarycentric_coordinates_2
 * The class `Wachspress_2` implements 2D Wachspress coordinates ( \cite cgal:bc:fhk-gcbcocp-06, \cite cgal:bc:mlbd-gbcip-02, \cite cgal:bc:w-rfeb-75 ).
 * This class is parameterized by a traits class `Traits`, and it is used as a coordinate class to complete the class `Generalized_barycentric_coordinates_2`.
 * For a polygon with three vertices it is better to use the class `Triangle_coordinates_2`.
 * Wachspress coordinates can be computed exactly, and they are always positive in the closure of a strictly convex polygon.

\tparam Traits must be a model of the concepts `BarycentricTraits_2` and `PolygonTraits_2`.

\cgalModels `BarycentricCoordinates_2`

\pre The provided polygon is strictly convex.

*/

template<class Traits>
    class Wachspress_2
{

public:

    /// \name Types
    /// @{

    /// Number type.
    typedef typename Traits::FT      FT;

    /// Point type.
    typedef typename Traits::Point_2 Point_2;

    /// @}

    // \name Creation

    // Creates the class `Wachspress_2` that implements the behaviour of Wachspress coordinates for any query point that does not belong to the polygon's boundary.
    // The polygon is given by a range of vertices of the type `Traits::Point_2` stored in a container of the type <a href="http://en.cppreference.com/w/cpp/container/vector">`std::vector`</a>.
    Wachspress_2(const std::vector<typename Traits::Point_2> &vertices, const Traits &barycentric_traits) :
        m_vertex(vertices),
        m_barycentric_traits(barycentric_traits),
        area_2(m_barycentric_traits.compute_area_2_object()),
        collinear_2(m_barycentric_traits.collinear_2_object())
    {
        // Resize all the internal containers.
        const size_t number_of_vertices = m_vertex.size();
        A.resize(number_of_vertices);
        C.resize(number_of_vertices);

        weight.resize(number_of_vertices);
    }

    // Computation of Wachspress Weight Functions

    // This function computes Wachspress weights (unnormalized coordinates) for a chosen query point.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> weights(const Point_2 &query_point, OutputIterator &output)
    {
        return weights_2(query_point, output);
    }

    // Computation of Wachspress Basis Functions

    // This function computes Wachspress barycentric coordinates for a chosen query point on the bounded side of a strictly convex polygon.
    // \pre The provided polygon is strictly convex.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> coordinates_on_bounded_side(const Point_2 &query_point, OutputIterator &output, const Type_of_algorithm type_of_algorithm)
    {
        switch(type_of_algorithm)
        {
            case PRECISE:
            return coordinates_on_bounded_side_precise_2(query_point, output);

            case FAST:
            return coordinates_on_bounded_side_fast_2(query_point, output);

            default:
            return boost::optional<OutputIterator>();
        }

    }

    // This function computes Wachspress barycentric coordinates for a chosen query point on the unbounded side of a strictly convex polygon.
    // \pre The provided polygon is strictly convex.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> coordinates_on_unbounded_side(const Point_2 &query_point, OutputIterator &output, const Type_of_algorithm type_of_algorithm, const bool warning_tag = true)
    {
        switch(type_of_algorithm)
        {
            case PRECISE:
            return coordinates_on_unbounded_side_precise_2(query_point, output, warning_tag);

            case FAST:
            return coordinates_on_unbounded_side_fast_2(query_point, output, warning_tag);

            default:
            return boost::optional<OutputIterator>();
        }

    }


private:

    // Some convenient typedefs.
    typedef typename std::vector<FT>      FT_vector;
    typedef typename std::vector<Point_2> Point_vector;

    // Internal global variables.
    const Point_vector &m_vertex;

    const Traits &m_barycentric_traits;

    FT_vector A, C, weight;

    FT wp_denominator, inverted_wp_denominator;

    typename Traits::Compute_area_2 area_2;
    typename Traits::Collinear_2 collinear_2;

    // WEIGHTS.

    // Compute Wachspress weights without normalization.
    template<class OutputIterator>
        boost::optional<OutputIterator> weights_2(const Point_2 &query_point, OutputIterator &output)
    {
        // Get the number of vertices in the polygon.
        const size_t n = m_vertex.size();

        // Compute areas A and C following the area notation from [1]. Split the loop to make this computation faster.
        A[0] = area_2(m_vertex[0]  , m_vertex[1], query_point);
        C[0] = area_2(m_vertex[n-1], m_vertex[0], m_vertex[1]  );

        for(size_t i = 1; i < n-1; ++i) {
            A[i] = area_2(m_vertex[i]  , m_vertex[i+1], query_point);
            C[i] = area_2(m_vertex[i-1], m_vertex[i]  , m_vertex[i+1]);
        }

        A[n-1] = area_2(m_vertex[n-1], m_vertex[0]  , query_point);
        C[n-1] = area_2(m_vertex[n-2], m_vertex[n-1], m_vertex[0]  );

        // Compute unnormalized weights following the formula (28) from [1].
        CGAL_precondition( A[n-1] != FT(0) && A[0] != FT(0) );
        *output = C[0] / (A[n-1] * A[0]);
        ++output;

        for(size_t i = 1; i < n-1; ++i) {
            CGAL_precondition( A[i-1] != FT(0) && A[i] != FT(0) );
            *output = C[i] / (A[i-1] * A[i]);
            ++output;
        }

        CGAL_precondition( A[n-2] != FT(0) && A[n-1] != FT(0) );
        *output = C[n-1] / (A[n-2] * A[n-1]);

        // Return weights.
        return boost::optional<OutputIterator>(output);
    }

    // COORDINATES ON BOUNDED SIDE.

    // Compute Wachspress coordinates on the bounded side of the polygon with the slow O(n^2) but precise algorithm.
    // Here, n - is the number of the polygon's vertices.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_bounded_side_precise_2(const Point_2 &query_point, OutputIterator &output)
    {
        CGAL_precondition( type_of_polygon() == STRICTLY_CONVEX );

        // Get the number of vertices in the polygon.
        const size_t n = m_vertex.size();

        // Compute areas A following the area notation from [1]. Split the loop to make this computation faster.
        A[0] = area_2(m_vertex[0], m_vertex[1], query_point);
        for(size_t i = 1; i < n-1; ++i) A[i] = area_2(m_vertex[i], m_vertex[i+1], query_point);
        A[n-1] = area_2(m_vertex[n-1], m_vertex[0], query_point);

        // Initialize weights with areas C following the area notation from [1].
        // Then we multiply them by areas A as in the formula (5) from [1]. We also split the loop.
        weight[0] = area_2(m_vertex[n-1], m_vertex[0], m_vertex[1]);
        for(size_t j = 1; j < n-1; ++j) weight[0] *= A[j];

        for(size_t i = 1; i < n-1; ++i) {
            weight[i] = area_2(m_vertex[i-1], m_vertex[i], m_vertex[i+1]);
            for(size_t j = 0; j < i-1; ++j) weight[i] *= A[j];
            for(size_t j = i+1; j < n; ++j) weight[i] *= A[j];
        }

        weight[n-1] = area_2(m_vertex[n-2], m_vertex[n-1], m_vertex[0]);
        for(size_t j = 0; j < n-2; ++j) weight[n-1] *= A[j];

        // Compute the sum of all weights - denominator of Wachspress coordinates.
        wp_denominator = weight[0];
        for(size_t i = 1; i < n; ++i) wp_denominator += weight[i];

        // Invert this denominator.
        CGAL_precondition( wp_denominator != FT(0) );
        inverted_wp_denominator = FT(1) / wp_denominator;

        // Normalize weights and save them as resulting Wachspress coordinates.
        for(size_t i = 0; i < n-1; ++i) {
            *output = weight[i] * inverted_wp_denominator;
            ++output;
        }
        *output = weight[n-1] * inverted_wp_denominator;

        // Return coordinates.
        return boost::optional<OutputIterator>(output);
    }

    // Compute Wachspress coordinates on the bounded side of the polygon with the fast O(n) but less precise algorithm.
    // Here, n - is the number of the polygon's vertices. Precision is lost near the boundary (~ 1.0e-10 and closer).
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_bounded_side_fast_2(const Point_2 &query_point, OutputIterator &output)
    {
        CGAL_precondition( type_of_polygon() == STRICTLY_CONVEX );

        // Get the number of vertices in the polygon.
        const size_t n = m_vertex.size();

        // Compute areas A and C following the area notation from [1]. Split the loop to make this computation faster.
        A[0] = area_2(m_vertex[0]  , m_vertex[1], query_point);
        C[0] = area_2(m_vertex[n-1], m_vertex[0], m_vertex[1]  );

        for(size_t i = 1; i < n-1; ++i) {
            A[i] = area_2(m_vertex[i]  , m_vertex[i+1], query_point);
            C[i] = area_2(m_vertex[i-1], m_vertex[i]  , m_vertex[i+1]);
        }

        A[n-1] = area_2(m_vertex[n-1], m_vertex[0]  , query_point);
        C[n-1] = area_2(m_vertex[n-2], m_vertex[n-1], m_vertex[0]  );

        // Compute the unnormalized weights following the formula (28) from [1].
        CGAL_precondition( A[n-1] != FT(0) && A[0] != FT(0) );
        weight[0] = C[0] / (A[n-1] * A[0]);

        for(size_t i = 1; i < n-1; ++i) {
            CGAL_precondition( A[i-1] != FT(0) && A[i] != FT(0) );
            weight[i] = C[i] / (A[i-1] * A[i]);
        }

        CGAL_precondition( A[n-2] != FT(0) && A[n-1] != FT(0) );
        weight[n-1] = C[n-1] / (A[n-2] * A[n-1]);

        // Compute the sum of all weights - denominator of Wachspress coordinates.
        wp_denominator = weight[0];
        for(size_t i = 1; i < n; ++i) wp_denominator += weight[i];

        // Invert this denominator.
        CGAL_precondition( wp_denominator != FT(0) );
        inverted_wp_denominator = FT(1) / wp_denominator;

        // Normalize weights and save them as resulting Wachspress coordinates.
        for(size_t i = 0; i < n-1; ++i) {
            *output = weight[i] * inverted_wp_denominator;
            ++output;
        }
        *output = weight[n-1] * inverted_wp_denominator;

        // Return coordinates.
        return boost::optional<OutputIterator>(output);
    }

    // COORDINATES ON UNBOUNDED SIDE.

    // Compute Wachspress coordinates on the unbounded side of the polygon with the slow O(n^2) but precise algorithm.
    // Here, n - is the number of the polygon's vertices.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_unbounded_side_precise_2(const Point_2 &query_point, OutputIterator &output, bool warning_tag)
    {
        if(warning_tag)
            std::cout << std::endl << "ATTENTION: Wachspress coordinates might be not well-defined outside the polygon!" << std::endl;

        // Use the same formulas as for the bounded side since they are also valid on the unbounded side.
        return coordinates_on_bounded_side_precise_2(query_point, output);
    }

    // Compute Wachspress coordinates on the unbounded side of the polygon with the fast O(n) but less precise algorithm.
    // Here, n - is the number of the polygon's vertices. Precision is lost near the boundary (~ 1.0e-10 and closer).
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_unbounded_side_fast_2(const Point_2 &query_point, OutputIterator &output, bool warning_tag)
    {
        if(warning_tag)
            std::cout << std::endl << "ATTENTION: Wachspress coordinates might be not well-defined outside the polygon!" << std::endl;

        // Use the same formulas as for the bounded side since they are also valid on the unbounded side.
        return coordinates_on_bounded_side_fast_2(query_point, output);
    }



    // Check the type of the provided polygon - CONVEX, STRICTLY_CONVEX, or CONCAVE.
    Type_of_polygon type_of_polygon() const
    {
        // First, test the polygon on convexity.
        if(CGAL::is_convex_2(m_vertex.begin(), m_vertex.end(), m_barycentric_traits)) {

            // Index of the last polygon's m_vertex.
            const size_t last = m_vertex.size() - 1;

            // Test all the consequent triplets of the polygon's vertices on collinearity.
            // In case we find at least one, return WEAKLY_CONVEX polygon.
            if(collinear_2(m_vertex[last], m_vertex[0], m_vertex[1]))
                return WEAKLY_CONVEX;

            for(size_t i = 1; i < last; ++i) {
                if(collinear_2(m_vertex[i-1], m_vertex[i], m_vertex[i+1]))
                    return WEAKLY_CONVEX;
            }

            if(collinear_2(m_vertex[last-1], m_vertex[last], m_vertex[0]))
                return WEAKLY_CONVEX;

            // Otherwise, return STRICTLY_CONVEX polygon.
            return STRICTLY_CONVEX;
        }

        // Otherwise, return CONCAVE polygon.
        return CONCAVE;
    }
};

} // namespace Barycentric_coordinates

} // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif // CGAL_WACHSPRESS_2_H
