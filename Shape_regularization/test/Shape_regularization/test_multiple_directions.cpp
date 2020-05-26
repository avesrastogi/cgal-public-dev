#include "include/utils.h"
#include "include/Saver.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Shape_regularization/Contours/Multiple_directions_2.h>

namespace SR = CGAL::Shape_regularization;

template<class Traits>
void test_multiple_directions() {

  using FT      = typename Traits::FT;
  using Point_2 = typename Traits::Point_2;
  using Saver   = SR::Tests::Saver<Traits>;

  using Contour = std::vector<Point_2>;
  using Point_map = CGAL::Identity_property_map<Point_2>;
  using MD = SR::Contours::Multiple_directions_2<Traits, Contour, Point_map>;

  Saver saver;
  Point_map pmap;
  const Contour contour = {
    Point_2( 1, 1), Point_2(4, 1),
    Point_2( 4, 4), Point_2(7, 1),
    Point_2(10, 4), Point_2(7, 7),
    Point_2(1, 7)
  };
  assert(contour.size() == 7);
  // saver.export_closed_contour(contour,
  //   "/Users/monet/Documents/gsoc/ggr/logs/md_input");

  const FT min_length_2 = FT(2);
  const FT max_angle_2 = FT(10);

  const bool is_closed = true;
  MD closed_directions(
    contour,  is_closed,
    CGAL::parameters::min_length(min_length_2).max_angle(max_angle_2), pmap);
  MD open_directions(
    contour, !is_closed,
    CGAL::parameters::min_length(min_length_2).max_angle(max_angle_2), pmap);

  const std::size_t num_closed_directions =
    closed_directions.number_of_directions();
  const std::size_t num_open_directions =
    open_directions.number_of_directions();

  assert(num_closed_directions == 2);
  assert(num_closed_directions == num_open_directions);

  const auto& closed_dirs = closed_directions.get_directions();
  const auto& open_dirs = open_directions.get_directions();

  assert(closed_dirs[0] == open_dirs[0]);
  assert(closed_dirs[1] == open_dirs[1]);
}

int main() {
  test_multiple_directions< CGAL::Simple_cartesian<double> >();
  test_multiple_directions< CGAL::Exact_predicates_inexact_constructions_kernel >();
  test_multiple_directions< CGAL::Exact_predicates_exact_constructions_kernel >();
  std::cout << "test_multiple_directions: SUCCESS" << std::endl;
  return EXIT_SUCCESS;
}
