namespace CGAL {
namespace Barycentric_coordinates {

/*!
\ingroup PkgBarycentricCoordinates2RefConcepts
\cgalConcept

A concept that describes the set of methods that should be defined for all
discretized domains obtained by meshing the interior part of a simple polygon.

After meshing, the interior part of the polygon is split into multiple finite
elements, which share common edges and vertices. These finite elements are then
used to approximate certain types of generalized barycentric coordinate functions.
The domain is bounded by the polygon.

\cgalHasModel
- `Delaunay_domain_2`
*/
class DiscretizedDomain_2 {

public:

  /*!
    returns the number of vertices after meshing the domain.
  */
  const std::size_t number_of_vertices() const {

  }

  /*!
    returns a const reference of type `Point_2` to the vertex with
    the index `query_index`.
  */
  const Point_2& vertex(
    const std::size_t query_index) const {

  }

  /*!
    controls if the vertex with the index `query_index` is on the
    boundary of the domain.
  */
  const bool is_on_boundary(
    const std::size_t query_index) const {

  }

  /*!
    fills `neighbors` with the indices of the vertices, which from the one-ring
    neighborhood of the vertex with the index `query_index`, the neighbors have to
    be in the counterclockwise order and form a simple polygon.
  */
  void operator()(
    const std::size_t query_index,
    std::vector<std::size_t>& neighbors) {

  }

  /*!
    fills `indices` with the indices of the vertices, which form a finite element
    of the domain, that contains a `query` point, if no indices are found, the
    `query` point does not belong to the domain.
  */
  void locate(
    const Point_2& query,
    std::vector<std::size_t>& indices) {

  }
};

} // namespace Barycentric_coordinates
} // namespace CGAL
