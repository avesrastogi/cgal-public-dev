#ifndef CGAL_LEVEL_OF_DETAIL_MYLOG_H
#define CGAL_LEVEL_OF_DETAIL_MYLOG_H

#if defined(WIN32) || defined(_WIN32) 
#define _SR_ "\\"
#define _NL_ "\r\n"
#else 
#define _SR_ "/" 
#define _NL_ "\n"
#endif

// STL includes.
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// CGAL includes.
#include <CGAL/Random.h>
#include <CGAL/IO/Color.h>

namespace CGAL {

	namespace Level_of_detail {

		class Mylog {

		private:
			using Colour = CGAL::Color;
			using Random = CGAL::Random;

			Random m_rand;

		public:
			Mylog() {
				out.precision(20);
			}

			void clear() {
				out.str(std::string());
			}

            template<class Elements, class Point_map>
            void save_points(const Elements &elements, const Point_map &point_map, const std::string &file_name) {

				clear();
                using Const_elements_iterator = typename Elements::const_iterator;

				for (Const_elements_iterator ce_it = elements.begin(); ce_it != elements.end(); ++ce_it) out << get(point_map, *ce_it) << std::endl;
				save(file_name, ".xyz");
            }

			template<class Faces_range, class Point_map>
			void save_faces(const Faces_range &faces_range, const Point_map &point_map, const std::string &file_name) {

				clear();
				using Const_faces_iterator = typename Faces_range::const_iterator;

				size_t num_faces    = 0;
				size_t num_vertices = 0;

				for (Const_faces_iterator cf_it = faces_range.begin(); cf_it != faces_range.end(); ++cf_it) {
					
					num_faces    += 1;
					num_vertices += (*cf_it).size();
				}

				out << 
				"ply" 				   +  std::string(_NL_) + ""               			  << 
				"format ascii 1.0"     +  std::string(_NL_) + ""     			          << 
				"element vertex "      << num_vertices     << "" + std::string(_NL_) + "" << 
				"property double x"    +  std::string(_NL_) + ""    			          << 
				"property double y"    +  std::string(_NL_) + ""    			          << 
				"property double z"    +  std::string(_NL_) + "" 				          <<
				"element face "        << num_faces        << "" + std::string(_NL_) + "" << 
				"property list uchar int vertex_indices"         + std::string(_NL_) + "" <<
				"property uchar red"   +  std::string(_NL_) + "" 				          <<
				"property uchar green" +  std::string(_NL_) + "" 				          <<
				"property uchar blue"  +  std::string(_NL_) + "" 				          <<
				"end_header"           +  std::string(_NL_) + "";

				for (Const_faces_iterator cf_it = faces_range.begin(); cf_it != faces_range.end(); ++cf_it) {
					const auto &vertices = *cf_it;

					for (auto cv_it = vertices.begin(); cv_it != vertices.end(); ++cv_it)
						out << get(point_map, *cv_it) << std::endl;
				}

				for (Const_faces_iterator cf_it = faces_range.begin(); cf_it != faces_range.end(); ++cf_it) {
					const auto &vertices = *cf_it;
					
					const size_t num_vertices = (*cf_it).size();
					out << num_vertices << " ";

					for (size_t i = 0; i < num_vertices; ++i) out << i << " ";
					out << generate_random_colour() << std::endl;
				}

				save(file_name, ".ply");
			}

			template<class Regions_range, class Point_map>
			void save_regions(const Regions_range &regions_range, const Point_map &point_map, const std::string &file_name) {

				clear();
				using Const_regions_iterator = typename Regions_range::const_iterator;

				size_t num_vertices = 0;
				for (Const_regions_iterator cr_it = regions_range.begin(); cr_it != regions_range.end(); ++cr_it) num_vertices += (*cr_it).size();

				out << 
				"ply" 				   +  std::string(_NL_) + ""               			  << 
				"format ascii 1.0"     +  std::string(_NL_) + ""     			          << 
				"element vertex "      << num_vertices     << "" + std::string(_NL_) + "" << 
				"property double x"    +  std::string(_NL_) + ""    			          << 
				"property double y"    +  std::string(_NL_) + ""    			          << 
				"property double z"    +  std::string(_NL_) + "" 				          <<
				"property uchar red"   +  std::string(_NL_) + "" 				          <<
				"property uchar green" +  std::string(_NL_) + "" 				          <<
				"property uchar blue"  +  std::string(_NL_) + "" 				          <<
				"end_header"           +  std::string(_NL_) + "";

				for (Const_regions_iterator cr_it = regions_range.begin(); cr_it != regions_range.end(); ++cr_it) {
					
					const auto &points = *cr_it;
					const Colour colour = generate_random_colour();
					
					for (auto cp_it = points.begin(); cp_it != points.end(); ++cp_it) 
						out << get(point_map, *cp_it) << " " << colour << std::endl;
				}
				
				save(file_name, ".ply");
			}

			template<class Elements, class Segment_map_2>
			void save_segments(const Elements &elements, const Segment_map_2 &segment_map_2, const std::string &file_name) {
				
				clear();
				using Const_elements_iterator = typename Elements::const_iterator;

				size_t size = 0;
				for (Const_elements_iterator ce_it = elements.begin(); ce_it != elements.end(); ++ce_it, ++size) {
					const auto &segment = get(segment_map_2, *ce_it);

					out << "v " << segment.source() << " " << 0 << std::endl;
					out << "v " << segment.target() << " " << 0 << std::endl;
					out << "v " << segment.target() << " " << 0 << std::endl;
				}

				for (size_t i = 0; i < size * 3; i += 3)
					out << "f " << i + 1 << " " << i + 2 << " " << i + 3 << std::endl;
				
				save(file_name, ".obj");
			}

        private:
            std::stringstream out;

			inline std::string data() const {
				return out.str();
			}

			void save(const std::string &file_name, const std::string &extension = ".log") const {

				const std::string final_path = file_name + extension;
				std::ofstream file(final_path.c_str(), std::ios_base::out);

				if (!file) std::cerr << std::endl << "ERROR: Error saving log file with the name " << file_name << std::endl << std::endl;

				file << data() << std::endl;
				file.close();
			}

			Colour generate_random_colour() {

				const int r = m_rand.get_int(0, 256);
				const int g = m_rand.get_int(0, 256);
				const int b = m_rand.get_int(0, 256);

				return Colour(r, g, b);
			}
        };

    } // Level_of_detail

} // CGAL

#endif // CGAL_LEVEL_OF_DETAIL_MYLOG_H