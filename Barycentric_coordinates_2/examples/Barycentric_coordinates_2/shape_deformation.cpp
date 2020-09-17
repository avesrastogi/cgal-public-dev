#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Barycentric_coordinates_2.h>

// Typedefs.
using Kernel  = CGAL::Exact_predicates_inexact_constructions_kernel;
using FT      = Kernel::FT;
using Point_2 = Kernel::Point_2;

int main() {

  // Construct the source and target shapes.
  // The number of vertices in both shapes must be equal.
  const std::vector<Point_2> source_shape = {
    Point_2(0, 0), Point_2(1, 0), Point_2(1, 1), Point_2(0, 1)
  };
  const std::vector<Point_2> target_shape = {
    Point_2(0, 0), Point_2(2, 0), Point_2(2, 2), Point_2(0, 2)
  };
  assert(target_shape.size() == source_shape.size());

  // Use seeds to mark the interior part of the source shape.
  const std::vector<Point_2> seeds = {
    Point_2(FT(1) / FT(2), FT(1) / FT(2))
  };

  // Use it to store coordinates.
  std::vector< std::vector<FT> > coordinates;

  // Compute harmonic coordinates at the vertices of the
  // discretized interior domain of the source shape.
  CGAL::Barycentric_coordinates::harmonic_coordinates_2(
    source_shape, seeds, std::back_inserter(coordinates));

  // Deform the source domain into the target domain.
  // We output only the first 20 results.
  for (std::size_t k = 0; k < 20; ++k) {
    FT x = FT(0), y = FT(0);
    for (std::size_t i = 0; i < coordinates[k].size(); ++i) {
      x += coordinates[k][i] * target_shape[i].x();
      y += coordinates[k][i] * target_shape[i].y();
    }
    std::cout << "deformed domain vertex: (" << x << ", " << y << ")" << std::endl;
  }

  return EXIT_SUCCESS;
}
