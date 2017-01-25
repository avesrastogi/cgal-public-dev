// Copyright (c) 2008  INRIA Sophia-Antipolis (France), ETH Zurich (Switzerland).
// All rights reserved.
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
// Author(s)     : Camille Wormser, Jane Tournois, Pierre Alliez


#ifndef CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H
#define CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H

#include <CGAL/Sphere_3.h>
#include <CGAL/Bbox_3.h>

#include <CGAL/number_utils.h>


namespace CGAL {

namespace internal {

    template <class K>
    typename K::Boolean
    do_intersect(const typename K::Sphere_3& sphere,
        const CGAL::Bbox_3& bbox,
        const K&)
    {
        typedef typename K::FT FT;
        typedef typename K::Point_3 Point;
        FT d = FT(0);
        FT distance = FT(0);
		Point center = sphere.center();

		if(possibly(center.x() < (FT)bbox.xmin()))
		{
			d = (FT)bbox.xmin() - center.x();
			distance += square(d);
		}
		else if(possibly(center.x() > (FT)bbox.xmax()))
		{
			d = center.x() - (FT)bbox.xmax();
			distance += square(d);
		}

		if(possibly(center.y() < (FT)bbox.ymin()))
		{
			d = (FT)bbox.ymin() - center.y();
			distance += square(d);
		}
		else if(possibly(center.y() > (FT)bbox.ymax()))
		{
			d = center.y() - (FT)bbox.ymax();
			distance += square(d);
		}

		if(possibly(center.z() < (FT)bbox.zmin()))
		{
			d = (FT)bbox.zmin() - center.z();
			distance += square(d);
		}
		else if(possibly(center.z() > (FT)bbox.zmax()))
		{
			d = center.z() - (FT)bbox.zmax();
			distance += square(d);
		}

		// For unknown reason this causes a syntax error on VC2005
		// but compiles fine on Linux and MAC
		//int i;
		//for(i = 0; i < 3; ++i)
		//{
		//	if(center[i] < (FT)bbox.min(i))
		//	{
		//		d = (FT)bbox.min(i) - center[i];
		//		distance += square(d);
		//	}
		//	else if(center[i] > (FT)bbox.max(i))
		//	{
		//		d = center[i] - (FT)bbox.max(i);
		//		distance += square(d);
		//	}
		//}

		return distance <= sphere.squared_radius();
    }

    template <class K>
    typename K::Boolean
    do_intersect(const CGAL::Bbox_3& bbox,
                 const typename K::Sphere_3& sphere,
                 const K&)
    { return do_intersect(sphere, bbox, K()); }


} // namespace internal

template<typename K>
typename K::Boolean
do_intersect(const CGAL::Bbox_3& a,
             const Sphere_3<K>& b) {
  return K().do_intersect_3_object()(a, b);
}

template<typename K>
typename K::Boolean
do_intersect(const Sphere_3<K>& a,
             const CGAL::Bbox_3& b) {
  return K().do_intersect_3_object()(a, b);
}


} //namespace CGAL

#endif  // CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H

// Local Variables:
// tab-width: 4
// End:
