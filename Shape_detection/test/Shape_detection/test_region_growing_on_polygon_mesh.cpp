// STL includes.
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>

// CGAL includes.
#include <CGAL/assertions.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_polygon_mesh.h>

namespace SD = CGAL::Shape_detection;

template<class Kernel>
bool test_region_growing_on_polygon_mesh(int argc, char *argv[]) {

  using FT      = typename Kernel::FT;
  using Point_3 = typename Kernel::Point_3;

  using Surface_mesh = CGAL::Surface_mesh<Point_3>;
  using Face_range   = typename Surface_mesh::Face_range;

  using Neighbor_query = SD::Polygon_mesh::One_ring_neighbor_query<Surface_mesh>;
  using Region_type    = SD::Polygon_mesh::Least_squares_plane_fit_region<Kernel, Surface_mesh>;
  using Region_growing = SD::Region_growing<Face_range, Neighbor_query, Region_type>;

  // Default parameter values for the data file polygon_mesh.off.
  const FT          distance_threshold = FT(1);
  const FT          angle_threshold    = FT(45);
  const std::size_t min_region_size    = 5;

  // Load data.
  std::ifstream in(argc > 1 ? argv[1] : "../data/polygon_mesh.off");
  CGAL::set_ascii_mode(in);

  Surface_mesh surface_mesh;
  in >> surface_mesh;
    
  in.close();
  const Face_range face_range = CGAL::faces(surface_mesh);

  CGAL_assertion(face_range.size() == 32245);
  if (face_range.size() != 32245) 
    return false;

  // Create parameter classes.
  Neighbor_query neighbor_query(surface_mesh);

  using Vertex_to_point_map = typename Region_type::Vertex_to_point_map;
  const Vertex_to_point_map vertex_to_point_map(
    get(CGAL::vertex_point, surface_mesh));

  Region_type region_type(
    surface_mesh, 
    distance_threshold, angle_threshold, min_region_size, 
    vertex_to_point_map);

  // Run region growing.
  Region_growing region_growing(
    face_range, neighbor_query, region_type);
  
  std::vector< std::vector<std::size_t> > regions;
  region_growing.detect(std::back_inserter(regions));

  // Test data.
  CGAL_assertion(regions.size() >= 328 && regions.size() <= 332);
  if (regions.size() < 328 || regions.size() > 332) 
    return false;

  for (const auto& region : regions)
    if (!region_type.is_valid_region(region)) 
      return false;

  std::vector<std::size_t> unassigned_faces;
  region_growing.unassigned_items(std::back_inserter(unassigned_faces));

  CGAL_assertion(unassigned_faces.size() >= 869 && unassigned_faces.size() <= 889);
  if (unassigned_faces.size() < 869 || unassigned_faces.size() > 889) 
    return false;

  return true;
}

int main(int argc, char *argv[]) {

  // ------>

  bool cartesian_double_test_success = true;
  if (!test_region_growing_on_polygon_mesh< CGAL::Simple_cartesian<double> >(argc, argv)) 
    cartesian_double_test_success = false;
    
  std::cout << "cartesian_double_test_success: " << cartesian_double_test_success << std::endl;
  CGAL_assertion(cartesian_double_test_success);

  // ------>

  bool exact_inexact_test_success = true;
  if (!test_region_growing_on_polygon_mesh<CGAL::Exact_predicates_inexact_constructions_kernel>(argc, argv)) 
    exact_inexact_test_success = false;
    
  std::cout << "exact_inexact_test_success: " << exact_inexact_test_success << std::endl;
  CGAL_assertion(exact_inexact_test_success);
}
