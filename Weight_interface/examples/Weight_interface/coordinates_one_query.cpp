#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/discrete_harmonic_weights.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using FT      = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;

int main() {

  // Create a polygon and a query point.
  const std::vector<Point_2> polygon =
    { Point_2(0, 0), Point_2(1, 0), Point_2(1, 1), Point_2(0, 1) };
  const Point_2 query = Point_2(FT(1) / FT(2), FT(1) / FT(2));

  // Allocate memory for weights and coordinates.
  std::vector<FT> weights;
  weights.reserve(polygon.size());
  std::vector<FT> coordinates;
  coordinates.reserve(polygon.size());

  // Compute barycentric weights.
  CGAL::Weights::discrete_harmonic_weights_2(
    polygon, query, std::back_inserter(weights));

  std::cout << "2D weights: ";
  for (const FT weight : weights)
    std::cout << weight << " ";
  std::cout << std::endl;

  // Normalize weights in order to get barycentric coordinates.
  FT sum = FT(0);
  for (const FT weight : weights)
    sum += weight;
  for (const FT weight : weights)
    coordinates.push_back(weight / sum);

  std::cout << "2D coordinates: ";
  for (const FT coordinate : coordinates)
    std::cout << coordinate << " ";
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
