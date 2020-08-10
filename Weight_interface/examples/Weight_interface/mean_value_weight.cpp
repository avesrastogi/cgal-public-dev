#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights/Mean_value_weight.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using FT      = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;
using Point_3 = typename Kernel::Point_3;

using MV = CGAL::Generalized_weights::Mean_value_weight<Kernel>;

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
  const MV mv;
  std::cout << "2D mean value: " << mv(q2, t2, r2, p2) << std::endl;
  std::cout << "3D mean value: " << mv(q3, t3, r3, p3) << std::endl;
  std::cout << "----------------" << std::endl;

  // Using free functions.
  std::cout << "2D mean value: " <<
    CGAL::Generalized_weights::mean_value_weight_2(q2, t2, r2, p2) << std::endl;
  std::cout << "3D mean value: " <<
    CGAL::Generalized_weights::mean_value_weight_3(q3, t3, r3, p3) << std::endl;

  return EXIT_SUCCESS;
}
