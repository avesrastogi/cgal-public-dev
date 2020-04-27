#include <CGAL/Timer.h>
#include <CGAL/property_map.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Shape_regularization.h>

#include "include/Saver.h"

// Typedefs.
using Kernel = CGAL::Simple_cartesian<double>;

using FT        = typename Kernel::FT;
using Point_2   = typename Kernel::Point_2;
using Segment_2 = typename Kernel::Segment_2;
using Indices   = std::vector<std::size_t>;

using Input_range = std::vector<Segment_2>;
using Parallel_groups = 
  CGAL::Shape_regularization::Segments::Parallel_groups_2<Kernel, Input_range>;

using Neighbor_query = 
  CGAL::Shape_regularization::Segments::Delaunay_neighbor_query_2<Kernel, Input_range>;
using Offset_regularization = 
  CGAL::Shape_regularization::Segments::Offset_regularization_2<Kernel, Input_range>;
using Quadratic_program = 
  CGAL::Shape_regularization::OSQP_quadratic_program<FT>;
using QP_offset_regularizer = 
  CGAL::Shape_regularization::QP_regularization<Kernel, Input_range, Neighbor_query, Offset_regularization, Quadratic_program>;

using Saver = 
  CGAL::Shape_regularization::Examples::Saver<Kernel>;

double get_coef_value(
  const double theta, double& iterator) {
  
  if (
    theta == 0.0 || 
    theta == CGAL_PI / 2.0 || 
    theta == CGAL_PI || 
    theta == 3.0 * CGAL_PI / 2.0) {
    
    iterator = 0.0;
  } else if (
    theta == CGAL_PI / 4.0 || 
    theta == 3.0 * CGAL_PI / 4.0 || 
    theta == 5.0 * CGAL_PI / 4.0 || 
    theta == 7.0 * CGAL_PI / 4.0) {
    
    iterator = 0.22;
  } else if (
    (theta > 0.0 && theta < CGAL_PI / 4.0) || 
    (theta > CGAL_PI / 2.0 && theta < 3.0 * CGAL_PI / 4.0) || 
    (theta > CGAL_PI && theta < 5.0 * CGAL_PI / 4.0) || 
    (theta > 3.0 * CGAL_PI / 2.0 && theta < 7.0 * CGAL_PI / 4.0)) {
    
    iterator += 0.02;
  } else
    iterator -= 0.02;

  if (theta < CGAL_PI) return -1.0 * iterator;
  return iterator;
}

int main(int argc, char *argv[]) {

  std::cout << std::endl << 
    "regularize 100 segments offsets example started" 
  << std::endl << std::endl;

  // If we want to save the result in a file, we save it in a path.
  std::string path = "";
  if (argc > 1) path = argv[1];

  // Initialize a timer.
  CGAL::Timer timer;

  // Initialize input range.
  Input_range input_range;
  input_range.reserve(100);

  double theta = 0.0;
  double coef = 0.0;
  double iterator = 0.0;
  double theta_step = CGAL_PI / 25.0;

  while (theta < 2 * CGAL_PI) {
    const double st = std::sin(theta);
    const double ct = std::cos(theta);
    const Point_2 a = Point_2(0.0, 0.0);
    const Point_2 b = Point_2(ct, st);

    coef = get_coef_value(theta, iterator);
    const Point_2 c = Point_2(ct, st + coef);
    const Point_2 d = Point_2(2 * ct, 2 * st + coef);
    theta += theta_step;

    input_range.push_back(Segment_2(a, b));
    input_range.push_back(Segment_2(c, d));
  }

  // Save input segments.
  if (path != "") {
    Saver saver;
    const std::string full_path = path + "regularize_100_segments_offsets_before";
    saver.save_segments_2(input_range, full_path);
  }

  // Regularize.
  timer.start();

  // Create parallel groups.
  const FT max_angle_2 = FT(1);
  Parallel_groups grouping(input_range, 
  CGAL::parameters::max_angle(max_angle_2));

  std::vector<Indices> parallel_groups;
  grouping.groups(
    std::back_inserter(parallel_groups));

  std::cout << 
    "* number of parallel groups = " << parallel_groups.size() 
  << std::endl;

  // Create a solver.
  Quadratic_program qp_offsets;

  // Create a neighbor query.
  Neighbor_query neighbor_query(input_range);

  // Offset regularization.
  const FT max_offset_2 = FT(25) / FT(100);
  Offset_regularization offset_regularization(
    input_range, CGAL::parameters::max_offset(max_offset_2));

  for (const auto& group : parallel_groups) {
    if (group.size() < 2) continue;
    neighbor_query.add_group(group);
    offset_regularization.add_group(group);
  }

  QP_offset_regularizer qp_offset_regularizer(
    input_range, neighbor_query, offset_regularization, qp_offsets);
  qp_offset_regularizer.regularize();

  timer.stop();
  std::cout << 
    "* number of modified segments = " << 
    offset_regularization.number_of_modified_segments() << 
    " in time = " << timer.time() << " sec." 
  << std::endl;

  // Save regularized segments.
  if (path != "") {
    Saver saver;
    const std::string full_path = path + "regularize_100_segments_offsets_after";
    saver.save_segments_2(input_range, full_path);
  }

  std::cout << std::endl << 
    "regularize 100 segments offsets example finished" 
  << std::endl << std::endl;
}
