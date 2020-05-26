#include "include/utils.h"
#include "include/Saver.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Shape_regularization/Segments/Delaunay_neighbor_query_2.h>

namespace SR = CGAL::Shape_regularization;

template<class Traits>
void test_neighbor_query() {

  using FT        = typename Traits::FT;
  using Point_2   = typename Traits::Point_2;
  using Segment_2 = typename Traits::Segment_2;
  using Indices   = std::vector<std::size_t>;
  using Saver     = SR::Tests::Saver<Traits>;

  using Segments = std::vector<Segment_2>;
  using NQ = SR::Segments::Delaunay_neighbor_query_2<Traits, Segments>;

  Saver saver;
  const Segments segments = {
    Segment_2(Point_2(0, 0), Point_2(4, 0)), // external square
    Segment_2(Point_2(4, 0), Point_2(4, 4)),
    Segment_2(Point_2(4, 4), Point_2(0, 4)),
    Segment_2(Point_2(0, 4), Point_2(0, 0)),

    Segment_2(Point_2(1, 1), Point_2(3, 1)), // internal square
    Segment_2(Point_2(3, 1), Point_2(3, 3)),
    Segment_2(Point_2(3, 3), Point_2(1, 3)),
    Segment_2(Point_2(1, 3), Point_2(1, 1))
  };
  // saver.export_polylines(segments,
  //   "/Users/monet/Documents/gsoc/ggr/logs/nq_input");

  std::vector<Indices> groups(2);
  groups[0] = {0, 1, 2, 3}; // external square
  groups[1] = {4, 5, 6, 7}; // internal square

  NQ neighbor_query(segments);
  neighbor_query.create_unique_group();

  // Check unique group.
  Segments edges;
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph0");
  assert(edges.size() == 17);
  assert(neighbor_query.number_of_groups() == 1);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);

  // Check clear.
  neighbor_query.clear();
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph1");
  assert(edges.size() == 0);
  assert(neighbor_query.number_of_groups() == 0);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);

  // Add first group.
  neighbor_query.clear();
  neighbor_query.add_group(groups[0]);
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph2");
  assert(edges.size() == 5);
  assert(neighbor_query.number_of_groups() == 1);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);

  // Add second group.
  neighbor_query.clear();
  neighbor_query.add_group(groups[1]);
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph3");
  assert(edges.size() == 5);
  assert(neighbor_query.number_of_groups() == 1);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);

  // Add groups consequently.
  neighbor_query.clear();
  neighbor_query.add_group(groups[0]);
  neighbor_query.add_group(groups[1]);
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph4");
  assert(edges.size() == 10);
  assert(neighbor_query.number_of_groups() == 2);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);

  // Check list with minimum 2 items.
  neighbor_query.clear();
  const std::list<std::size_t> mini = {0, 1};
  neighbor_query.add_group(mini);
  neighbor_query.get_edges(edges);
  // saver.export_polylines(edges, "/Users/monet/Documents/gsoc/ggr/logs/nq_graph5");
  assert(edges.size() == 1);
  assert(neighbor_query.number_of_groups() == 1);
  assert(neighbor_query.number_of_neighbors() == edges.size() * 2);
}

int main() {
  test_neighbor_query< CGAL::Simple_cartesian<double> >();
  test_neighbor_query< CGAL::Exact_predicates_inexact_constructions_kernel >();
  test_neighbor_query< CGAL::Exact_predicates_exact_constructions_kernel >();
  std::cout << "test_neighbor_query: SUCCESS" << std::endl;
  return EXIT_SUCCESS;
}
