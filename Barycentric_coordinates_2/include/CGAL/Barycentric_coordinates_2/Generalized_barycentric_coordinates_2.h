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
  \file Generalized_barycentric_coordinates_2.h
*/

#ifndef CGAL_GENERALIZED_BARYCENTRIC_COORDINATES_2_H
#define CGAL_GENERALIZED_BARYCENTRIC_COORDINATES_2_H

#include <CGAL/license/Barycentric_coordinates_2.h>

#include <CGAL/disable_warnings.h>

// CGAL headers.
#include <CGAL/Polygon_2_algorithms.h>

// Barycentric coordinates headers.
#include <CGAL/Barycentric_coordinates_2/Segment_coordinates_2.h>
#include <CGAL/Barycentric_coordinates_2/barycentric_enum_2.h>

// Boost headers.
#include <boost/optional.hpp>

// Memory headers.
#include <memory>

// Property map headers.
#include <CGAL/property_map.h>

// CGAL namespace.
namespace CGAL {

// Barycentric coordinates namespace.
namespace Barycentric_coordinates {

// Examples: see the User Manual here - https://doc.cgal.org/latest/Manual/index.html.

/*!
 * \ingroup PkgBarycentric_coordinates_2
 * The class `Generalized_barycentric_coordinates_2` implements generalized barycentric coordinates along the polygon's boundary and provides a common interface for all coordinate classes.
 * This class is parameterized by a coordinate class `Coordinate_2`, a element container class `Elements`, a corresponding point map class `Point_map` and a traits class `Traits`.

\tparam Coordinate_2 must be a model of the concept `BarycentricCoordinates_2`.

\tparam Elements could be a model of any data containers e.g. `std::vector<Pair>` where `Pair` is typedef of `std::pair<Point_2, bool>`.

\tparam Point_map must be a model of corresponding property maps of Elements e.g. CGAL::First_of_pair_property_map().

\tparam Traits must be a model of the concepts `BarycentricTraits_2` and `PolygonTraits_2`.

*/

template<class Coordinate_2,  class Elements, class Point_map, class Traits >
    class Generalized_barycentric_coordinates_2
{

public:

    /// \name Types
    /// @{

    /// Number type.
    typedef typename Traits::FT      FT;

    /// Point type.
    typedef typename Traits::Point_2 Point_2;


    #ifdef DOXYGEN_RUNNING
        /// Elements container.
        /// This type allows users pass an arbitrary container with corresponding point maps.
        typedef unspecified_type Element_iterator;
    #else
        typedef typename Elements::const_iterator Element_iterator;
    #endif

    /// @}

    /// \name Creation
    /// @{

    /// Creates the class `Generalized_barycentric_coordinates_2` that implements generalized barycentric coordinates along the polygon's boundary given by a range of elements `elements` and corresponding point maps `point_map`.
    /// \pre Number of the polygon's vertices > 2.
    /// \pre The provided polygon is simple.
    Generalized_barycentric_coordinates_2(const Elements &elements, const Point_map &point_map, const Traits &barycentric_traits = Traits()) :
        m_elements(elements),
        m_point_map(point_map),
        m_barycentric_traits(barycentric_traits),
        equal_2(m_barycentric_traits.equal_2_object()),
        collinear_2(m_barycentric_traits.collinear_2_object()),
        collinear_are_ordered_along_line_2(m_barycentric_traits.collinear_are_ordered_along_line_2_object())
    {
        CGAL_precondition( m_elements.size() > 2 );

        for(Element_iterator element_it = m_elements.begin(); element_it != m_elements.end(); ++element_it)
        {
            const Point_2 &point = get(m_point_map, *element_it);
            m_vertex.push_back(point);
        }

        m_coordinate = std::make_shared<Coordinate_2>(m_vertex, barycentric_traits);

        CGAL_precondition( CGAL::is_simple_2(m_vertex.begin(), m_vertex.end(), barycentric_traits) );
    }

    /// @}

    /// \name Computation
    /// @{

    /// Computes generalized barycentric coordinates for any query point in the plane with respect to all the vertices of the polygon.
    /// Computed coordinates are stored in the output iterator `output`.
    ///
    /// Different choices of the parameter `query_point_location` are possible:
    /// `CGAL::Barycentric_coordinates::UNSPECIFIED_LOCATION` - default constant with automatic check for a location,
    /// `CGAL::Barycentric_coordinates::ON_BOUNDED_SIDE` - for a strictly interior query point,
    /// `CGAL::Barycentric_coordinates::ON_BOUNDARY` - for a query point on the boundary of the polygon,
    /// `CGAL::Barycentric_coordinates::ON_VERTEX` - for a query point at one of the polygon's vertices, and
    /// `CGAL::Barycentric_coordinates::ON_UNBOUNDED_SIDE` - for a strictly exterior query point.
    ///
    /// Another parameter is `type_of_algorithm` with the following possible constants:
    /// `CGAL::Barycentric_coordinates::PRECISE` - default slow algorithm, which is as precise as possible and
    /// `CGAL::Barycentric_coordinates::FAST` - fast algorithm, which is less precise but much faster.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> compute(const Point_2 &query_point, OutputIterator output, Query_point_location query_point_location = UNSPECIFIED_LOCATION, Type_of_algorithm type_of_algorithm = PRECISE)
    {
        return coordinates_2(query_point, output, query_point_location, type_of_algorithm);
    }

    /// Computes generalized barycentric coordinates for a query point on the polygon's boundary with known index of the edge to which this point belongs.
    /// Computed coordinates are stored in the output iterator `output`, and it is a set of zeros with two possibly non zero values.
    ///
    /// \pre The provided query point belongs to the polygon's boundary.
    /// \pre (0 <= index) && (index < number of the polygon's vertices).
    template<class OutputIterator>
        inline boost::optional<OutputIterator> compute_on_edge(const Point_2 &query_point, const int index, OutputIterator output) const
    {
        return coordinates_on_boundary_2(query_point, index, output);
    }

    /// Computes generalized barycentric coordinates for a query point, which coincides with one of the polygon's vertices, with known index.
    /// Computed coordinates are stored in the output iterator `output`, and it is a set of zeros with value of one at the vertex of interest.
    ///
    /// \pre (0 <= index) && (index < number of the polygon's vertices).
    template<class OutputIterator>
        inline boost::optional<OutputIterator> compute_on_vertex(const int index, OutputIterator output) const
    {
        return coordinates_on_vertex_2(index, output);
    }

    /// Computes generalized barycentric weights (unnormalized coordinates) for any strictly interior query point with respect to all the vertices of the polygon.
    /// Computed weights are stored in the output iterator `output`.
    ///
    /// \pre The provided query point belongs to the polygon's interior, excluding the boundary.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> compute_weights(const Point_2 &query_point, OutputIterator output)
    {
        return weights_2(query_point, output);
    }

    /// @}

    /// \name Endpoint Accessors
    /// @{

    /// Returns all the vertices of the polygon.
    //inline const Vertex_range& vertices() const
    //{
    //    return vertex;
    //}

    /// Returns the first vertex of the polygon.
    inline const Point_2& first_vertex() const
    {
        return m_vertex.begin();
        //return get(m_point_map, *(m_elements.begin()));
    }

    /// Returns the last vertex of the polygon.
    inline const Point_2& last_vertex() const
    {
        return m_vertex.end();
        //return get(m_point_map, *(m_elements.end()));
    }

    /// @}

    // Computes generalized barycentric coordinates for any query point in the plane with respect to all the vertices of the polygon.
    // This function accepts a container of the type <a href="http://en.cppreference.com/w/cpp/container/vector">`std::vector`</a>
    // and returns an iterator of the type <a href="http://en.cppreference.com/w/cpp/iterator/back_insert_iterator">`std::back_insert_iterator`</a>
    // that is placed past-the-end of the resulting sequence of coordinate values.
    inline boost::optional<std::back_insert_iterator<std::vector<FT> > > compute(const Point_2 &query_point, std::vector<FT> &output_vector, Query_point_location query_point_location = UNSPECIFIED_LOCATION, Type_of_algorithm type_of_algorithm = PRECISE)
    {
        const size_t number_of_vertices = m_vertex.size();
        output_vector.reserve(output_vector.size() + number_of_vertices);
        typedef typename std::back_insert_iterator<std::vector<FT> > OutputIterator;
        OutputIterator output = std::back_inserter(output_vector);
        return coordinates_2(query_point, output, query_point_location, type_of_algorithm);
    }

    // Computes generalized barycentric coordinates for a query point on the polygon's boundary with known index of the edge to which this point belongs.
    // This function accepts a container of the type <a href="http://en.cppreference.com/w/cpp/container/vector">`std::vector`</a>
    // and returns an iterator of the type <a href="http://en.cppreference.com/w/cpp/iterator/back_insert_iterator">`std::back_insert_iterator`</a>
    // that is placed past-the-end of the resulting sequence of coordinate values.
    //
    // \pre The provided query point belongs to the polygon's boundary.
    // \pre (0 <= index) && (index < number of the polygon's vertices).
    inline boost::optional<std::back_insert_iterator<std::vector<FT> > > compute_on_edge(const Point_2 &query_point, const int index, std::vector<FT> &output_vector) const
    {
        const size_t number_of_vertices = m_vertex.size();
        output_vector.reserve(output_vector.size() + number_of_vertices);
        typedef typename std::back_insert_iterator<std::vector<FT> > OutputIterator;
        OutputIterator output = std::back_inserter(output_vector);
        return coordinates_on_boundary_2(query_point, index, output);
    }

    // Computes generalized barycentric coordinates for a query point, which coincides with one of the polygon's vertices, with known index.
    // This function accepts a container of the type <a href="http://en.cppreference.com/w/cpp/container/vector">`std::vector`</a>
    // and returns an iterator of the type <a href="http://en.cppreference.com/w/cpp/iterator/back_insert_iterator">`std::back_insert_iterator`</a>
    // that is placed past-the-end of the resulting sequence of coordinate values.
    //
    // \pre (0 <= index) && (index < number of the polygon's vertices).
    inline boost::optional<std::back_insert_iterator<std::vector<FT> > > compute_on_vertex(const int index, std::vector<FT> &output_vector) const
    {
        const size_t number_of_vertices = m_vertex.size();
        output_vector.reserve(output_vector.size() + number_of_vertices);
        typedef typename std::back_insert_iterator<std::vector<FT> > OutputIterator;
        OutputIterator output = std::back_inserter(output_vector);
        return coordinates_on_vertex_2(index, output);
    }

    // Computes generalized barycentric weights (unnormalized coordinates) for any strictly interior query point with respect to all the vertices of the polygon.
    // This function accepts a container of the type <a href="http://en.cppreference.com/w/cpp/container/vector">`std::vector`</a>
    // and returns an iterator of the type <a href="http://en.cppreference.com/w/cpp/iterator/back_insert_iterator">`std::back_insert_iterator`</a>
    // that is placed past-the-end of the resulting sequence of weight values.
    //
    // \pre The provided query point belongs to the polygon's interior, excluding the boundary.
    inline boost::optional<std::back_insert_iterator<std::vector<FT> > > compute_weights(const Point_2 &query_point, std::vector<FT> &output_vector)
    {
        const size_t number_of_vertices = m_vertex.size();
        output_vector.reserve(output_vector.size() + number_of_vertices);
        typedef typename std::back_insert_iterator<std::vector<FT> > OutputIterator;
        OutputIterator output = std::back_inserter(output_vector);
        return weights_2(query_point, output);
    }

    // Information Functions

    // This function prints some information about the used polygon and coordinate functions.
    void print_information(std::ostream &output_stream = std::cout) const
    {
        output_stream << std::endl << "INFORMATION: " << std::endl;

        output_stream << std::endl << "DATA STRUCTURE: " << std::endl << std::endl;
        output_stream << "The internal data structure is polygon." << std::endl;

        output_stream << std::endl << "NUMBER OF VERTICES: " << std::endl << std::endl;
        const size_t number_of_vertices = m_vertex.size();
        if(int(number_of_vertices < 3) ) {
            output_stream << "This polygon has " << number_of_vertices << " vertices." << std::endl;
            output_stream << "Since number of vertices is less than 3, generalized barycentric coordinates cannot be computed!" << std::endl;
            output_stream << "Please use the class CGAL::Barycentric_coordinates::Segment_coordinates_2!" << std::endl;
        } else {
            if(int(number_of_vertices) == 3) {
                 output_stream << "This polygon has " << number_of_vertices << " vertices." << std::endl;
                 output_stream << "For triangles it is better to use the class CGAL::Barycentric_coordinates::Triangle_coordinates_2!" << std::endl;
            }
            else output_stream << "This polygon has " << number_of_vertices << " vertices." << std::endl;
        }

        output_stream << std::endl << "SIMPLICITY: " << std::endl << std::endl;
        if(CGAL::is_simple_2(m_vertex.begin(), m_vertex.end(), m_barycentric_traits)) output_stream << "This polygon is simple." << std::endl;
        else output_stream << "This polygon is not simple. The correInputIterator &first_vertexct computation is not expected!" << std::endl;

    }

private:

    // Internal global variables.
    const Elements m_elements;

    const Point_map m_point_map;

    typedef typename std::vector<Point_2> Vertex_range;

    Vertex_range m_vertex;

    const Traits &m_barycentric_traits;

    std::shared_ptr<Coordinate_2> m_coordinate;

    typename Traits::Equal_2 equal_2;
    typename Traits::Collinear_2 collinear_2;
    typename Traits::Collinear_are_ordered_along_line_2 collinear_are_ordered_along_line_2;

    // WEIGHTS.

    // Compute weights on the bounded side of the polygon - see precondition.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> weights_2(const Point_2 &query_point, OutputIterator &output)
    {
        // This is the only global precondition on the computation of weights.
        CGAL_precondition( CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits) == CGAL::ON_BOUNDED_SIDE );

        return m_coordinate->weights(query_point, output);
    }

    // COORDINATES EVERYWHERE.

    // Compute coordinates at any point in the plane.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_2(const Point_2 &query_point, OutputIterator &output, const Query_point_location query_point_location, const Type_of_algorithm type_of_algorithm)
    {
        // Determine a location of the current query point provided by the user.
        switch(query_point_location)
        {
        case UNSPECIFIED_LOCATION:
            return coordinates_unspecified_2(query_point, output, type_of_algorithm);

        case ON_BOUNDED_SIDE:
            return coordinates_on_bounded_side_2(query_point, output, type_of_algorithm);

        case ON_BOUNDARY:
            return coordinates_on_boundary_2(query_point, output);

        case ON_VERTEX:
            return coordinates_on_vertex_2(query_point, output);

        case ON_UNBOUNDED_SIDE:
            return coordinates_on_unbounded_side_2(query_point, output, type_of_algorithm);

        default:
            return boost::optional<OutputIterator>();
        }

    }

    // Compute coordinates at any point in the plane with unspecified location.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_unspecified_2(const Point_2 &query_point, OutputIterator &output, const Type_of_algorithm type_of_algorithm)
    {
        // Determine a global location of the current query point.
        switch(CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits))
        {
        case CGAL::ON_BOUNDED_SIDE:
            return coordinates_on_bounded_side_2(query_point, output, type_of_algorithm);

        case CGAL::ON_BOUNDARY:
        {
            int index = -1;
            if(is_query_point_at_vertex(query_point, index)) return coordinates_on_vertex_2(index, output);
            else return coordinates_on_boundary_2(query_point, output);
        }

        case CGAL::ON_UNBOUNDED_SIDE:
            return coordinates_on_unbounded_side_2(query_point, output, type_of_algorithm);

        default:
            return boost::optional<OutputIterator>();
        }

    }

    // COORDINATES ON BOUNDED SIDE.

    // Compute coordinates on the bounded side of the polygon - precise or fast.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> coordinates_on_bounded_side_2(const Point_2 &query_point, OutputIterator &output, const Type_of_algorithm type_of_algorithm)
    {
        CGAL_precondition( CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits) == CGAL::ON_BOUNDED_SIDE );

        return m_coordinate->coordinates_on_bounded_side(query_point, output, type_of_algorithm);
    }

    // COORDINATES ON BOUNDARY.

    // Compute coordinates along the boundary of the polygon with beforehand known index of the edge to which the query point belongs.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_boundary_2(const Point_2 &query_point, const int index, OutputIterator &output) const
    {
        CGAL_precondition( CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits) == CGAL::ON_BOUNDARY );
        CGAL_precondition( (0 <= index) && (index < m_vertex.size() ) );

        // Index of the last polygon's vertex.
        const size_t last = m_vertex.size() - 1;

        // If the query point is on the last edge, call the function coordinates_on_last_edge_2().
        if(index == last) return coordinates_on_last_edge_2(query_point, last, output);
        else {
            // Otherwise, all the coordinates are zero apart from those for the chosen edge with the index = `index`.
            for(size_t i = 0; i < index; ++i) {
                *output = FT(0);
                ++output;
            }

            // Compute segment coordinates along the chosen edge with the index = `index`.
            Segment_coordinates_2<Traits> segment_coordinates(m_vertex[index], m_vertex[index+1]);
            boost::optional<OutputIterator> success = segment_coordinates.compute(query_point, output);
            ++output;

            for(size_t i = index + 1; i < last; ++i) {
                *output = FT(0);
                ++output;
            }

            // Return coordinates.
            if(success) return boost::optional<OutputIterator>(output);
            else return boost::optional<OutputIterator>();
        }

    }

    // Compute coordinates along the boundary of the polygon without beforehand known index of the edge to which the query point belongs.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_boundary_2(const Point_2 &query_point, OutputIterator &output) const
    {
        CGAL_precondition( CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits) == CGAL::ON_BOUNDARY );

        // Index of the last polygon's vertex.
        const size_t last = m_vertex.size() - 1;

        // If the query point is on the last edge, call the function coordinates_on_last_edge_2().
        if(collinear_2(m_vertex[last], m_vertex[0], query_point) && collinear_are_ordered_along_line_2(m_vertex[last], query_point, m_vertex[0])) {
            return coordinates_on_last_edge_2(query_point, last, output);
        }
        else {
            // Otherwise, all the coordinates are zeros apart from those for the edge with the query point.
            int index;
            bool status = false;
            for(index = 0; index < last; ++index) {
                if(collinear_2(m_vertex[index], m_vertex[index+1], query_point) && collinear_are_ordered_along_line_2(m_vertex[index], query_point, m_vertex[index+1])) {

                    // Compute segment coordinates along the edge with the query point.
                    Segment_coordinates_2<Traits> segment_coordinates(m_vertex[index], m_vertex[index+1]);
                    boost::optional<OutputIterator> success = segment_coordinates.compute(query_point, output);
                    if(success) status = true;
                    ++output;
                    break;

                } else {
                    *output = FT(0);
                    ++output;
                }
            }

            for(size_t i = index + 1; i < last; ++i) {
                *output = FT(0);
                ++output;
            }

            // Return coordinates.
            if(status == true) return boost::optional<OutputIterator>(output);
            else return boost::optional<OutputIterator>();
        }

    }

    // Compute coordinates for a query point lying on the last edge of the polygon.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_last_edge_2(const Point_2 &query_point, const int last, OutputIterator &output) const
    {
        // Some convenient typedefs.
        typedef std::vector<FT> Coordinate_vector;

        typedef std::back_insert_iterator<Coordinate_vector> Vector_insert_iterator;

        // Create a coordinate vector.
        Coordinate_vector coordinate;
        coordinate.reserve(2);

        // Compute segment coordinates along the last edge of the polygon.
        Segment_coordinates_2<Traits> segment_coordinates(m_vertex[last], m_vertex[0]);
        boost::optional<Vector_insert_iterator> success = segment_coordinates.compute(query_point, std::back_inserter(coordinate));

        // Store all the coordinate values.
        // All the values are zeros apart from those corresponding to the first and the last vertices of the polygon.
        *output = coordinate[1];
        ++output;
        for(size_t i = 1; i < last; ++i) {
            *output = FT(0);
            ++output;
        }
        *output = coordinate[0];

        // Return computed coordinates.
        if(success) return boost::optional<OutputIterator>(output);
        else return boost::optional<OutputIterator>();
    }

    // COORDINATES AT VERTEX.

    // Compute coordinates for a query point lying at one of the polygon's vertices with beforehand known vertex's index.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_vertex_2(const int index, OutputIterator &output) const
    {
        CGAL_precondition( (0 <= index) && (index < m_vertex.size() ) );

        // All the coordinate values are zeros apart from that one with the index = `index`, which is one.
        for(size_t i = 0; i < index; ++i) {
            *output = FT(0);
            ++output;
        }

        *output = FT(1);
        ++output;

        const size_t last = m_vertex.size();
        for(size_t i = index + 1; i < last; ++i) {
            *output = FT(0);
            ++output;
        }

        // Return coordinates.
        return boost::optional<OutputIterator>(output);
    }

    // Compute coordinates for a query point lying at one of the polygon's vertices without beforehand known vertex's index.
    template<class OutputIterator>
        boost::optional<OutputIterator> coordinates_on_vertex_2(const Point_2 &query_point, OutputIterator &output) const
    {
        int index = -1;
        CGAL_precondition( is_query_point_at_vertex(query_point, index) );

        // All the coordinate values are zeros apart from that one corresponding to the vertex coinciding with the query point, which is one.
        bool coordinates_on_vertex_failure = true;
        const size_t last = m_vertex.size();
        for(index = 0; index < last; ++index) {
            if(is_query_point_equal_to_vertex(query_point, index)) {

                // We have just found vertex coinciding with the query point.
                *output = FT(1);
                ++output;
                coordinates_on_vertex_failure = false;
                break;

            }
            else {
                *output = FT(0);
                ++output;
            }
        }

        for(size_t i = index + 1; i < last; ++i) {
            *output = FT(0);
            ++output;
        }

        // Return coordinates.
        if(!coordinates_on_vertex_failure) return boost::optional<OutputIterator>(output);
        else return boost::optional<OutputIterator>();
    }

    // COORDINATES ON UNBOUNDED SIDE.

    // Compute coordinates on the unbounded side of the polygon - precise or fast.
    template<class OutputIterator>
        inline boost::optional<OutputIterator> coordinates_on_unbounded_side_2(const Point_2 &query_point, OutputIterator &output, const Type_of_algorithm type_of_algorithm)
    {
        CGAL_precondition( CGAL::bounded_side_2(m_vertex.begin(), m_vertex.end(), query_point, m_barycentric_traits) == CGAL::ON_UNBOUNDED_SIDE );

        return m_coordinate->coordinates_on_unbounded_side(query_point, output, type_of_algorithm);
    }

    // HELP FUNCTIONS.

    // Test if the query point is at vertex or not.
    inline bool is_query_point_at_vertex(const Point_2 &query_point, int &index) const
    {
        const size_t last = m_vertex.size();
        for(index = 0; index < last; ++index)
            if(is_query_point_equal_to_vertex(query_point, index)) return true;

        return false;
    }

    // Test if the query point is equal to the vertex with the index = `index` or not.
    inline bool is_query_point_equal_to_vertex(const Point_2 &query_point, const int index) const
    {
        if(equal_2(query_point, m_vertex[index])) return true;
        return false;
    }
};

} // namespace Barycentric_coordinates

} // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif // CGAL_GENERALIZED_BARYCENTRIC_COORDINATES_2_H
