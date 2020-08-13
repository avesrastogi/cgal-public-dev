#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights/tangent_weight.h>

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
  std::cout << "2D tangent: " <<
    CGAL::Generalized_weights::tangent_weight_2(q2, t2, r2, p2) << std::endl;
  std::cout << "3D tangent: " <<
    CGAL::Generalized_weights::tangent_weight_3(q3, t3, r3, p3) << std::endl;
  std::cout << "-------------" << std::endl;

  // // Construct a 2D weight.
  // const FT w2 =
  //   tw(tw.distance(r2, q2), tw.distance(t2, q2), tw.area(r2, q2, t2), tw.scalar_product(r2, q2, t2)) +
  //   tw(tw.distance(r2, q2), tw.distance(p2, q2), tw.area(p2, q2, r2), tw.scalar_product(p2, q2, r2));
  // std::cout << "2D tangent: " << w2 << std::endl;

  // // Construct a 3D weight.
  // const FT w3 =
  //   tw(tw.distance(r3, q3), tw.distance(t3, q3), tw.area(r3, q3, t3), tw.scalar_product(r3, q3, t3)) +
  //   tw(tw.distance(r3, q3), tw.distance(p3, q3), tw.area(p3, q3, r3), tw.scalar_product(p3, q3, r3));
  // std::cout << "3D tangent: " << w3 << std::endl;

  return EXIT_SUCCESS;
}
