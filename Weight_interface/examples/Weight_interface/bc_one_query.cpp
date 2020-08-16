#include <CGAL/Simple_cartesian.h>
#include <CGAL/Weight_interface/Generalized_weights.h>

// Typedefs.
using Kernel  = CGAL::Simple_cartesian<double>;
using FT      = typename Kernel::FT;
using Point_2 = typename Kernel::Point_2;

// #define WP_WEIGHT
#define MV_WEIGHT
// #define DH_WEIGHT

int main() {

  const Point_2 query = Point_2(FT(1) / FT(2), FT(1) / FT(2));
  const std::vector<Point_2> polygon =
    { Point_2(0, 0), Point_2(1, 0), Point_2(1, 1), Point_2(0, 1) };

  std::vector<FT> weights;
  weights.reserve(polygon.size());

  std::vector<FT> coordinates;
  coordinates.reserve(polygon.size());

  // Compute barycentric weights.
  #if defined(WP_WEIGHT)
    CGAL::Generalized_weights::wachspress_weights_2(
      polygon, query, std::back_inserter(weights));
  #elif defined(MV_WEIGHT)
    CGAL::Generalized_weights::mean_value_weights_2(
      polygon, query, std::back_inserter(weights));
  #elif defined(DH_WEIGHT)
    CGAL::Generalized_weights::discrete_harmonic_weights_2(
      polygon, query, std::back_inserter(weights));
  #endif

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
