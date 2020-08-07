// Copyright (c) 2020 GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
//
// Author(s)     : Dmitry Anisimov
//

#ifndef CGAL_GENERALIZED_UNIFORM_REGION_WEIGHT_H
#define CGAL_GENERALIZED_UNIFORM_REGION_WEIGHT_H

// #include <CGAL/license/Weight_interface.h>

// Internal includes.
#include <CGAL/Weight_interface/internal/utils.h>

namespace CGAL {
namespace Generalized_weights {

  /*!
    \ingroup PkgWeightInterfaceRefRegions

    \brief Uniform region weight.

    This weight always returns 1.

    \tparam GeomTraits
    must be a model of `AnalyticTraits`.
  */
  template<typename GeomTraits>
  class Uniform_region_weight {

  public:

    /// \name Types
    /// @{

    /// \cond SKIP_IN_MANUAL
    using GT = GeomTraits;
    /// \endcond

    /// Number type.
    typedef typename GeomTraits::FT FT;

    /// 2D point type.
    typedef typename GeomTraits::Point_2 Point_2;

    /// 3D point type.
    typedef typename GeomTraits::Point_3 Point_3;

    /// @}

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \param traits
      An instance of `GeomTraits`. The default initialization is provided.
    */
    Uniform_region_weight(
      const GeomTraits traits = GeomTraits()) :
    m_traits(traits)
    { }

    /// @}

    /// \name Access
    /// @{

    /*!
      \brief computes 2D uniform area weight.
    */
    const FT operator()(
      const Point_2&,
      const Point_2&,
      const Point_2&) const {

      return weight();
    }

    /*!
      \brief computes 2D uniform area weight.
    */
    const FT operator()(
      const Point_3&,
      const Point_3&,
      const Point_3&) const {

      return weight();
    }

    /// @}

  private:
    const GeomTraits m_traits;

    const FT weight() const {
      return FT(1);
    }
  };

  template<typename Point_2>
  decltype(auto) uniform_area_2(
    const Point_2& p, const Point_2& q, const Point_2& r) {

    using Traits = typename Kernel_traits<Point_2>::Kernel;
    Uniform_region_weight<Traits> uniform_area;
    return uniform_area(p, q, r);
  }

  template<typename Point_3>
  decltype(auto) uniform_area_3(
    const Point_3& p, const Point_3& q, const Point_3& r) {

    using Traits = typename Kernel_traits<Point_3>::Kernel;
    Uniform_region_weight<Traits> uniform_area;
    return uniform_area(p, q, r);
  }

} // namespace Generalized_weights
} // namespace CGAL

#endif // CGAL_GENERALIZED_UNIFORM_REGION_WEIGHT_H
