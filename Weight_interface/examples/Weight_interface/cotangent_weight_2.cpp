#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights_2/Cotangent_weight_2.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using FT      = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;
using Point_3 = typename Kernel::Point_3;

using CW2 = CGAL::Generalized_weights::Cotangent_weight_2<Kernel>;

int main() {

  // 2D configuration.
  const Point_2 query2 = Point_2( 0, 0);
  const Point_2 vm2    = Point_2( 1, 0);
  const Point_2 vj2    = Point_2( 0, 1);
  const Point_2 vp2    = Point_2(-1, 0);

  // 3D configuration.
  const Point_3 query3 = Point_3( 0, 0, 1);
  const Point_3 vm3    = Point_3( 1, 0, 1);
  const Point_3 vj3    = Point_3( 0, 1, 1);
  const Point_3 vp3    = Point_3(-1, 0, 1);

  // Compute weights.
  CW2 cotangent;
  std::cout << "2D cot: " << cotangent(query2, vm2, vj2, vp2) << std::endl;
  std::cout << "3D cot: " << cotangent(query3, vm3, vj3, vp3) << std::endl;
  std::cout << "---------" << std::endl;

  // Construct a 2D weight.
  const FT w2 = cotangent(query2, vm2, vj2) + cotangent(query2, vj2, vp2);
  std::cout << "2D cot: " << w2 << std::endl;

  // Construct a 3D weight.
  const FT w3 = cotangent(query3, vm3, vj3) + cotangent(query3, vj3, vp3);
  std::cout << "3D cot: " << w3 << std::endl;

  return EXIT_SUCCESS;
}
