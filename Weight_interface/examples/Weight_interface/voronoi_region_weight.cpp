#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Weighting_regions/Voronoi_region_weight.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using FT      = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;
using Point_3 = typename Kernel::Point_3;

using VRW = CGAL::Generalized_weights::Voronoi_region_weight<Kernel>;

int main() {

  // 2D configuration.
  const Point_2 p2 = Point_2(-1, 0);
  const Point_2 q2 = Point_2( 0, 0);
  const Point_2 r2 = Point_2( 0, 1);

  // 3D configuration.
  const Point_3 p3 = Point_3(-1, 0, 1);
  const Point_3 q3 = Point_3( 0, 0, 1);
  const Point_3 r3 = Point_3( 0, 1, 1);

  // Compute weights.
  VRW voronoi;
  std::cout << "2D voronoi: " << voronoi(p2, q2, r2) << std::endl;
  std::cout << "3D voronoi: " << voronoi(p3, q3, r3) << std::endl;
  std::cout << "----------" << std::endl;

  // Using free functions.
  std::cout << "2D voronoi: " <<
    CGAL::Generalized_weights::voronoi_area_2(p2, q2, r2) << std::endl;
  std::cout << "3D voronoi: " <<
    CGAL::Generalized_weights::voronoi_area_3(p3, q3, r3) << std::endl;

  return EXIT_SUCCESS;
}
