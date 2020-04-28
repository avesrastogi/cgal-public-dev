#include "include/utils.h"
#include "include/Saver.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Shape_regularization/Segments/Parallel_groups_2.h>

namespace SR = CGAL::Shape_regularization;

template<class Traits>
void test_parallel_groups() { 

  using FT        = typename Traits::FT;
  using Point_2   = typename Traits::Point_2;
  using Segment_2 = typename Traits::Segment_2;
  using Indices   = std::vector<std::size_t>;
  using Saver     = SR::Tests::Saver<Traits>;

  using Segments = std::vector<Segment_2>;
  using PG = SR::Segments::Parallel_groups_2<Traits, Segments>;

  Saver saver;
  const Segments segments = {
    Segment_2(Point_2(1, 1), Point_2(4, 1)), // bottom group
    Segment_2(Point_2(1, 2), Point_2(4, 2)),
    Segment_2(Point_2(1, 3), Point_2(FT(399) / FT(100), FT(319) / FT(100))),

    Segment_2(Point_2(1, 4), Point_2(1, 6)), // top left group
    Segment_2(Point_2(2, 5), Point_2(2, 8)),

    Segment_2(Point_2(3, 5), Point_2(6, 7)), // top right group
    Segment_2(Point_2(7, 6), Point_2(4, 4))
  };
  // saver.export_polylines(segments, 
  //   "/Users/monet/Documents/gsoc/ggr/logs/pg_input");
  
  const PG grouping(
    segments, CGAL::parameters::all_default());
  std::vector<Indices> groups;
  grouping.groups(
    std::back_inserter(groups));
  assert(groups.size() == 3);

  // saver.export_group(segments, groups[0], "pg_group0");
  assert(groups[0].size() == 3);
  // saver.export_group(segments, groups[1], "pg_group1");
  assert(groups[1].size() == 2);
  // saver.export_group(segments, groups[2], "pg_group2");
  assert(groups[2].size() == 2);
}

int main() {
  test_parallel_groups< CGAL::Simple_cartesian<double> >();
  test_parallel_groups< CGAL::Exact_predicates_inexact_constructions_kernel >();
  test_parallel_groups< CGAL::Exact_predicates_exact_constructions_kernel >();
  std::cout << "test_parallel_groups: SUCCESS" << std::endl;
  return EXIT_SUCCESS;
}
