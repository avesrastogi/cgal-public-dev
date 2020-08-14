#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights/mean_value_weights.h>

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
  std::cout << "2D mean value: " <<
    CGAL::Generalized_weights::mean_value_weight_2(q2, t2, r2, p2) << std::endl;
  std::cout << "3D mean value: " <<
    CGAL::Generalized_weights::mean_value_weight_3(q3, t3, r3, p3) << std::endl;
  std::cout << "----------------" << std::endl;

  // Compute weights on a polygon.
  const std::vector<Point_2> polygon = {t2, r2, p2, Point_2(0, 1)};
  std::vector<double> weights;
  weights.reserve(polygon.size());
  CGAL::Generalized_weights::mean_value_weights_2(
    polygon, q2, std::back_inserter(weights));
  std::cout << "2D mean value: ";
  for (const double weight : weights)
    std::cout << weight << " ";
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
