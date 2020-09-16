#include <CGAL/Simple_cartesian.h>
#include <CGAL/Barycentric_coordinates_2/Delaunay_domain_2.h>
#include <CGAL/Barycentric_coordinates_2/Harmonic_coordinates_2.h>

// Typedefs.
using Kernel      = CGAL::Simple_cartesian<double>;
using FT          = Kernel::FT;
using Point_2     = Kernel::Point_2;
using Point_range = std::vector<Point_2>;

using Domain =
  CGAL::Barycentric_coordinates::Delaunay_domain_2<Point_range, Kernel>;
using Harmonic_coordinates_2 =
  CGAL::Barycentric_coordinates::Harmonic_coordinates_2<Point_range, Domain, Kernel>;

int main() {

  // Construct a source shape.
  const std::vector<Point_2> source_shape = {
    Point_2(0, 0), Point_2(1, 0), Point_2(1, 1), Point_2(0, 1)
  };

  // Construct a target shape.
  const std::vector<Point_2> target_shape = {
    Point_2(0, 0), Point_2(2, 0), Point_2(2, 2), Point_2(0, 2)
  };

  // Construct a Delaunay domain for the source shape.
  std::list<Point_2> list_of_seeds;
  list_of_seeds.push_back(Point_2(0.5, 0.5));

  Domain source_domain(source_shape);
  source_domain.create(0.01, list_of_seeds);

  // Compute harmonic coordinates at the vertices of the source domain.
  Harmonic_coordinates_2 harmonic_coordinates_2(source_shape, source_domain);
  harmonic_coordinates_2.compute();

  // Use it to store coordinates.
  std::vector<double> coordinates;
  coordinates.reserve(source_shape.size());
  assert(target_shape.size() == source_shape.size());

  // Deform the source domain into the target domain.
  // We output only the first 20 results.
  for (std::size_t k = 0; k < 20; ++k) {
    coordinates.clear();
    harmonic_coordinates_2(k, std::back_inserter(coordinates));
    FT x = FT(0), y = FT(0);
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
      x += coordinates[i] * target_shape[i].x();
      y += coordinates[i] * target_shape[i].y();
    }
    std::cout << "deformed domain vertex: (" << x << ", " << y << ")" << std::endl;
  }

  return EXIT_SUCCESS;
}
