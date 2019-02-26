/*!
\ingroup PkgShapeDetectionRGConcepts
\cgalConcept

A concept that describes the set of methods used by the `CGAL::Shape_detection::Region_growing` 
to access connected items in a given set.

\cgalHasModel 
`CGAL::Shape_detection::Points_fuzzy_sphere_connectivity`, 
`CGAL::Shape_detection::Points_k_nearest_neighbors_connectivity`, 
`CGAL::Shape_detection::Polygon_mesh_adjacent_faces_connectivity`
*/

class NeighborQuery {

public:

  /*!  
    Fills the vector `neighbors` with indices of all items, which are connected to an item with the index `query_index`.

    This function is called each time when a new query item is chosen.
  */
  void operator()(
    const std::size_t query_index, 
    std::vector<std::size_t>& neighbors) {
        
  }
};
