#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights/wachspress_weights.h>
#include <CGAL/Weight_interface/internal/Projection_traits_3.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using Point_2 = typename Kernel::Point_2;
using Point_3 = typename Kernel::Point_3;

int main() {

  // 2D configuration.
  const Point_2 q2 = Point_2( 0,  0);
  const Point_2 t2 = Point_2(-1,  0);
  const Point_2 r2 = Point_2( 0, -1);
  const Point_2 p2 = Point_2( 1,  0);

  // 3D configuration.
  const Point_3 q3 = Point_3( 0,  0, 1);
  const Point_3 t3 = Point_3(-1,  0, 1);
  const Point_3 r3 = Point_3( 0, -1, 1);
  const Point_3 p3 = Point_3( 1,  0, 1);

  // Compute weights.
  std::cout << "2D wachspress: " <<
    CGAL::Generalized_weights::wachspress_weight_2(q2, t2, r2, p2) << std::endl;
  std::cout << "3D wachspress: " <<
    CGAL::Generalized_weights::wachspress_weight_3(q3, t3, r3, p3) << std::endl;

  // 2D configuration.
  const std::vector<Point_2> polygon2 = {t2, r2, p2, Point_2(0, 1)};

  std::vector<double> weights2;
  weights2.reserve(polygon2.size());
  CGAL::Generalized_weights::wachspress_weights_2(
    polygon2, q2, std::back_inserter(weights2));

  std::cout << "2D wachspress (polygon): ";
  for (const double weight2 : weights2)
    std::cout << weight2 << " ";
  std::cout << std::endl;

  // 3D configuration.
  CGAL::Generalized_weights::internal::Projection_traits_3<Kernel> ptraits(
    typename Kernel::Vector_3(0, 0, 1));

  const std::vector<Point_3> polygon3 = {t3, r3, p3, Point_3(0, 1, 1)};

  std::vector<double> weights3;
  weights3.reserve(polygon3.size());
  CGAL::Generalized_weights::wachspress_weights_2(
    polygon3, q3, std::back_inserter(weights3), ptraits);

  std::cout << "3D wachspress (polygon): ";
  for (const double weight3 : weights3)
    std::cout << weight3 << " ";
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
