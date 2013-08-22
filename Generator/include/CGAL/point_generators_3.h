// Copyright (c) 1997  
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved. 
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Lutz Kettner  <kettner@inf.ethz.ch>

#ifndef CGAL_POINT_GENERATORS_3_H
#define CGAL_POINT_GENERATORS_3_H 1
#include <CGAL/generators.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/number_type_basic.h>
#include <vector>
#include <algorithm>
#include <CGAL/internal/Finite_support_distribution.h>
#include <CGAL/internal/Weighted_random_generator.h>

#include <CGAL/Timer.h>

namespace CGAL {

template < class P, class Creator = 
                   Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_sphere_3 : public Random_generator_base<P> {
    void generate_point();
public:
    typedef P result_type;
    typedef Random_points_in_sphere_3<P,Creator> This;
    Random_points_in_sphere_3( double r = 1, Random& rnd = default_random)
        // g is an input iterator creating points of type `P' uniformly
        // distributed in the open sphere with radius r, i.e. |`*g'| < r .
        // Three random numbers are needed from `rnd' for each point
    : Random_generator_base<P>( r, rnd) { generate_point(); }
    This& operator++()    {
        generate_point();
        return *this;
    }
    This  operator++(int) {
        This tmp = *this;
        ++(*this);
        return tmp;
    }
};

template < class P, class Creator >
void
Random_points_in_sphere_3<P,Creator>::
generate_point() {
  // A strip between z and z+dz has an area independant of z
    typedef typename Creator::argument_type T;
    double alpha = this->_rnd.get_double() * 2.0 * CGAL_PI;
    double z     = 2 * this->_rnd.get_double() - 1.0;
    double r     = std::sqrt( 1 - z * z);
    r *= std::pow( this->_rnd.get_double() , 1.0/3.0 );  
    Creator creator;
    this->d_item = creator( T(this->d_range * r * std::cos(alpha)),
                            T(this->d_range * r * std::sin(alpha)),
                            T(this->d_range * z));
}


template < class P, class Creator = 
                   Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_on_sphere_3 : public Random_generator_base<P> {
    void generate_point();
public:
    typedef P result_type;
    typedef Random_points_on_sphere_3<P,Creator> This;
    Random_points_on_sphere_3( double r = 1, Random& rnd = default_random)
        // g is an input iterator creating points of type `P' uniformly
        // distributed on the sphere with radius r, i.e. |`*g'| == r . A
        // two random numbers are needed from `rnd' for each point.
    : Random_generator_base<P>( r, rnd) { generate_point(); }
    This& operator++()    {
        generate_point();
        return *this;
    }
    This  operator++(int) {
        This tmp = *this;
        ++(*this);
        return tmp;
    }
};

template < class P, class Creator >
void
Random_points_on_sphere_3<P,Creator>::
generate_point() {
  // A strip between z and z+dz has an area independant of z
    typedef typename Creator::argument_type T;
    double alpha = this->_rnd.get_double() * 2.0 * CGAL_PI;
    double z     = 2 * this->_rnd.get_double() - 1.0;
    double r     = std::sqrt( 1 - z * z);
    Creator creator;
    this->d_item = creator( T(this->d_range * r * std::cos(alpha)),
                            T(this->d_range * r * std::sin(alpha)),
                            T(this->d_range * z));
}


template < class P, class Creator = 
                   Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_cube_3 : public Random_generator_base<P>{
    void generate_point();
public:
    typedef P result_type;
    typedef Random_points_in_cube_3<P,Creator> This;
    Random_points_in_cube_3( double a = 1, Random& rnd = default_random)
    : Random_generator_base<P>( a, rnd) { generate_point(); }
    This& operator++()    {
        generate_point();
        return *this;
    }
    This  operator++(int) {
        This tmp = *this;
        ++(*this);
        return tmp;
    }
};

template < class P, class Creator >
void
Random_points_in_cube_3<P,Creator>::
generate_point() {
    typedef typename Creator::argument_type T;
    Creator creator;
    this->d_item =
	     creator( T(this->d_range * ( 2 * this->_rnd.get_double() - 1.0)),
                      T(this->d_range * ( 2 * this->_rnd.get_double() - 1.0)),
                      T(this->d_range * ( 2 * this->_rnd.get_double() - 1.0)));
}


template <class OutputIterator, class Creator>
OutputIterator
points_on_cube_grid_3( double a, std::size_t n, 
                         OutputIterator o, Creator creator)
{
    if  (n == 0)
        return o;

    int m = int(std::ceil(
                  std::sqrt(std::sqrt(static_cast<double>(n)))));

    while (m*m*m < int(n)) m++;

    double base = -a;  // Left and bottom boundary.
    double step = 2*a/(m-1);
    int j = 0;
    int k = 0;
    double px = base;
    double py = base;
    double pz = base;
    *o++ = creator( px, py, pz);
    for (std::size_t i = 1; i < n; i++) {
        j++;
        if ( j == m) {
           k++;
           if ( k == m) {
              py = base;
              px = base;
              pz = pz + step;
              k = 0;
           }
           else {
              px = base;
              py = py + step;
           }
           j = 0;
        } else {
           px = px + step;
        }
        *o++ = creator( px, py, pz);
    }
    return o;
}

template <class OutputIterator>
OutputIterator
points_on_cube_grid_3( double a, std::size_t n, OutputIterator o)
{
    typedef std::iterator_traits<OutputIterator> ITraits;
    typedef typename ITraits::value_type         P;
    return points_on_square_grid_3(a, n, o, 
                 Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P>());
}

template < class P, class Creator = 
Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_triangle_3 : public Random_generator_base<P> {
	P _p,_q,_r;
	void generate_point();
public:
	typedef P result_type;
	typedef Random_points_in_triangle_3<P> This;
	typedef typename Kernel_traits<P>::Kernel::Triangle_3 Triangle_3;
	Random_points_in_triangle_3() {}
	Random_points_in_triangle_3( const This& x,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd ),_p(x._p),_q(x._q),_r(x._r) {
		generate_point();
	}
	Random_points_in_triangle_3( const P& p, const P& q, const P& r, Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd ),_p(p),_q(q),_r(r) {
		generate_point();
	}
	Random_points_in_triangle_3( const Triangle_3& triangle,Random& rnd = default_random)
	: Random_generator_base<P>( 1,
			rnd),_p(triangle[0]),_q(triangle[1]),_r(triangle[2]) {
		generate_point();
	}
	This operator=(This x) {
		_p = x._p;
		_q = x._q;
		_r = x._r;
		return *this;
	}
	This& operator++() {
		generate_point();
		return *this;
	}
	This operator++(int) {
		This tmp = *this;
		++(*this);
		return tmp;
	}
};


template<class P, class Creator >
void Random_points_in_triangle_3<P, Creator>::generate_point() {
	typedef typename Creator::argument_type T;
	Creator creator;
	double a1 = this->_rnd.get_double(0,1);
	double a2 = this->_rnd.get_double(0,1);
	if(a1>a2) std::swap(a1,a2);
	double b1 = a1;
	double b2 = a2-a1;
	double b3 = 1.0-a2;
	T ret[3];
	for(int i = 0; i < 3; ++i) {
	    ret[i] = T(to_double(_p[i])*b1+to_double(_q[i])*b2+to_double(_r[i])*b3);
	}
	this->d_item = creator(ret[0],ret[1],ret[2]);
}

template < class P, class Creator = 
Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_tetrahedron_3 : public Random_generator_base<P> {
	P _p,_q,_r,_s;
	void generate_point();
public:
	typedef P result_type;
	typedef Random_points_in_tetrahedron_3<P> This;
	typedef typename Kernel_traits<P>::Kernel::Tetrahedron_3 Tetrahedron_3;
	Random_points_in_tetrahedron_3() {}
	Random_points_in_tetrahedron_3( const This& x,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd ),_p(x._p),_q(x._q),_r(x._r),_s(x._s) {
		generate_point();
	}
	Random_points_in_tetrahedron_3( const P& p, const P& q, const P& r, const P& s,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd ),_p(p),_q(q),_r(r),_s(s) {
		generate_point();
	}
	Random_points_in_tetrahedron_3( const Tetrahedron_3& tetrahedron,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd),_p(tetrahedron[0]),_q(tetrahedron[1]),_r(tetrahedron[2]),_s(tetrahedron[3]) {
		generate_point();
	}
	This operator=(This x) {
		_p = x._p;
		_q = x._q;
		_r = x._r;
		_s = x._s;
		return *this;
	}
	This& operator++() {
		generate_point();
		return *this;
	}
	This operator++(int) {
		This tmp = *this;
		++(*this);
		return tmp;
	}
};

template<class P, class Creator >
void Random_points_in_tetrahedron_3<P, Creator>::generate_point() {
	typedef typename Creator::argument_type T;
	Creator creator;
	double a[3];
	for(int i = 0; i < 3; ++i) {
		a[i]=this->_rnd.get_double(0,1);
	}
	std::sort(a,a+3);
	double b[4];
	b[0]=a[0];
	b[1]=a[1]-a[0];
	b[2]=a[2]-a[1];
	b[3]=1.0-a[2];
	T ret[3];
	for(int i = 0; i < 3; ++i) {
	    ret[i] = T(to_double(_p[i])*b[0]+to_double(_q[i])*b[1]+to_double(_r[i])*b[2]+to_double(_s[i])*b[3]);
	}
	this->d_item = creator(ret[0],ret[1],ret[2]);
}

template<typename Tetrahedron_3>
class WeightFunctor_tetrahedron_3 {
	public:
		double operator() (Tetrahedron_3 &t) {
			return t.volume();
		}
};

template < class P, class C3t3, class Creator = 
Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_mesh_3 : public Random_generator_base<P> {
	CGAL::internal::Finite_support_distribution<CGAL::internal::Weighted_random_generator<Random_points_in_tetrahedron_3<P> > > _fsp_distrib;
	Random *_rand;
	void generate_point();
public:
	CGAL::Timer t;

	typedef P result_type;
	typedef Random_points_in_mesh_3<P, C3t3> This;
	typedef typename Kernel_traits<P>::Kernel::Tetrahedron_3 Tetrahedron_3;
	typedef CGAL::Random_points_in_tetrahedron_3<P> PointGen;
	typedef CGAL::internal::Weighted_random_generator<PointGen>
		GeneratorWithWeight;
	typedef CGAL::internal::Finite_support_distribution<CGAL::internal::Weighted_random_generator<Random_points_in_tetrahedron_3<P> > > FspDistrib;
	typedef typename C3t3::Triangulation Tr;
	Random_points_in_mesh_3() {}
	Random_points_in_mesh_3( const This& x,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd
			),_fsp_distrib(x._fsp_distrib),_rand(x._rand) {
		generate_point();
	}
	Random_points_in_mesh_3( const FspDistrib& fsp_distrib,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd
			),_fsp_distrib(fsp_distrib),_rand(&rnd) {
		generate_point();
	}
	Random_points_in_mesh_3( const C3t3 c3t3,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd),_rand(&rnd) {
		t.start();

		int Nr_cells_in_cplx = c3t3.number_of_cells_in_complex();
		WeightFunctor_tetrahedron_3<Tetrahedron_3> weightElem;
		Tr tr = c3t3.triangulation();
		GeneratorWithWeight* containing_structure;
		containing_structure = new GeneratorWithWeight[Nr_cells_in_cplx];
		typename Tr::Finite_cells_iterator iter = tr.finite_cells_begin();
		int i = 0;
		for (; iter != tr.finite_cells_end(); ++iter) {
			if (c3t3.is_in_complex(iter)) {
				Tetrahedron_3 aux = tr.tetrahedron(iter);
				double weight = weightElem(aux);
				PointGen randGen(aux);
				containing_structure[i] = GeneratorWithWeight (randGen, weight);
				i++;
			}
		}

		_fsp_distrib =
			CGAL::internal::Finite_support_distribution<GeneratorWithWeight>
			(containing_structure, i);
		generate_point();

		t.stop();
		std::cout << "Time elapsed for building the containing structure: "
			<< t.time() << std::endl;
		t.reset();
		delete[] containing_structure;
	}
	This operator=(This x) {
		_fsp_distrib = FspDistrib(x._fsp_distrib);
		_rand = x._rand;
		return *this;
	}
	This& operator++() {
		generate_point();
		return *this;
	}
	This operator++(int) {
		This tmp = *this;
		++(*this);
		return tmp;
	}
};

template<class P, class C3t3, class Creator >
void Random_points_in_mesh_3<P, C3t3, Creator>::generate_point() {
	P ret = _fsp_distrib.generate(*_rand);
	this->d_item = ret;
}

template <typename Triangle_3>
class WeightFunctor_triangle_3 {
	public:
		double operator() (Triangle_3 &t) {
			return sqrt(t.squared_area());
		}
};

template < class P, class C2t3, class Creator = 
Creator_uniform_3<typename Kernel_traits<P>::Kernel::RT,P> >
class Random_points_in_surface_mesh_3 : public Random_generator_base<P> {
	CGAL::internal::Finite_support_distribution<CGAL::internal::Weighted_random_generator<Random_points_in_triangle_3<P> > > _fsp_distrib;
	Random *_rand;
	void generate_point();
public:
	typedef P result_type;
	typedef Random_points_in_surface_mesh_3<P, C2t3> This;
	typedef typename Kernel_traits<P>::Kernel::Triangle_3 Triangle_3;
	typedef CGAL::Random_points_in_triangle_3<P> PointGen;
	typedef CGAL::internal::Weighted_random_generator<PointGen>
		GeneratorWithWeight;
	typedef
		CGAL::internal::Finite_support_distribution<CGAL::internal::Weighted_random_generator<Random_points_in_triangle_3<P> > > FspDistrib;
	typedef typename C2t3::Triangulation Tr;
	Random_points_in_surface_mesh_3() {}
	Random_points_in_surface_mesh_3( const This& x,Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd
			),_fsp_distrib(x._fsp_distrib),_rand(x._rand) {
		generate_point();
	}
	Random_points_in_surface_mesh_3( const FspDistrib& fsp_distrib, Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd
			),_fsp_distrib(fsp_distrib),_rand(&rnd) {
		generate_point();
	}
	Random_points_in_surface_mesh_3( const C2t3 c2t3, Random& rnd = default_random)
	: Random_generator_base<P>( 1, rnd),_rand(&rnd) {
		WeightFunctor_triangle_3<Triangle_3> weightElem;
		int Nr_cells = c2t3.number_of_facets();
		std::vector<GeneratorWithWeight> containing_structure;
		containing_structure.reserve(Nr_cells);
		Tr tr = c2t3.triangulation();
		typename Tr::Finite_facets_iterator iter = tr.finite_facets_begin();
		for (; iter != tr.finite_facets_end(); ++iter) {
			if (c2t3.is_in_complex(*iter)) {
				Triangle_3 aux = tr.triangle(*iter);
				double weight = weightElem(aux);
				PointGen randGen(aux);
				GeneratorWithWeight tmp = GeneratorWithWeight (randGen, weight);
				containing_structure.push_back(tmp);
			}
		}
		CGAL::internal::Finite_support_distribution<GeneratorWithWeight
			> aux(containing_structure);
		_fsp_distrib = aux;
		generate_point();
	}
	This operator=(This x) {
		_fsp_distrib(x._fsp_distrib);
		_rand = x._rand;
		return *this;
	}
	This& operator++() {
		generate_point();
		return *this;
	}
	This operator++(int) {
		This tmp = *this;
		++(*this);
		return tmp;
	}
};

template<class P, class C2t3, class Creator >
void Random_points_in_surface_mesh_3<P, C2t3, Creator>::generate_point() {
	typedef typename Creator::argument_type T;
	Creator creator;
	P ret = _fsp_distrib.generate(*_rand);
	this->d_item = ret;
}
} //namespace CGAL

#endif // CGAL_POINT_GENERATORS_3_H //
// EOF //
