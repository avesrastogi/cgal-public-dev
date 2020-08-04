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
// Author(s)     : Jean-Philippe Bauchet, Florent Lafarge, Gennadii Sytov, Dmitry Anisimov
//

#ifndef CGAL_SHAPE_REGULARIZATION_QP_REGULARIZATION_H
#define CGAL_SHAPE_REGULARIZATION_QP_REGULARIZATION_H

// #include <CGAL/license/Shape_regularization.h>

// Internal includes.
#include <CGAL/Shape_regularization/internal/utils.h>

// Reference papers:
// [1] J. P. Bauchet and F. Lafarge, KIPPI: KInetic Polygonal Partitioning of Images, Section 3, 2018.

namespace CGAL {
namespace Shape_regularization {

  /*!
    \ingroup PkgShapeRegularizationRef

    \brief Shape regularization algorithm based on the quadratic programming
    global optimization.

    Given a quadratic programming solver via the class `QuadraticProgramTraits`, this version of the
    shape regularization algorithm enables to regularize a set of user-defined
    geometric objects provided a way
    - to access neighbors of each object via the `NeighborQuery` class;
    - to obtain a max bound on a regularization characteristic (angle-orientation/
    distance-offset/etc.) with respect to which an object is being regularized
    via the `RegularizationType` class;
    - to obtain a target value for each pair of neighbor objects via
    the `RegularizationType` class.

    This class is generic and forms a framework for different types of user-defined
    regularizations. Please address the user manual in order to learn how to use it.
    In particular, section \ref QP_Regularization_Segments "Regularizing 2D Segments"
    shows an example.

    \tparam GeomTraits
    must be a model of `Kernel`.

    \tparam InputRange
    must be a model of `ConstRange`.

    \tparam NeighborQuery
    must be a model of `NeighborQuery`.

    \tparam RegularizationType
    must be a model of `RegularizationType`.

    \tparam QuadraticProgramTraits
    must be a model of `QuadraticProgramTraits`.
  */
  template<
  typename GeomTraits,
  typename InputRange,
  typename NeighborQuery,
  typename RegularizationType,
  typename QuadraticProgramTraits>
  class QP_regularization {

  private:
    class Parameters {
    public:
      using FT = typename GeomTraits::FT;

      const FT weight,  lambda;
      const FT neg_inf, pos_inf;
      const FT val_neg, val_pos;

      Parameters():
      weight(FT(100000)),
      lambda(FT(4) / FT(5)),
      neg_inf(-internal::max_value<FT>()),
      pos_inf(+internal::max_value<FT>()),
      val_pos(FT(+2) * lambda),
      val_neg(FT(-2) * lambda)
      { }
    };

  public:
    /// \cond SKIP_IN_MANUAL
    using Traits = GeomTraits;
    using Input_range = InputRange;
    using Neighbor_query = NeighborQuery;
    using Regularization_type = RegularizationType;
    using Quadratic_program = QuadraticProgramTraits;

    using FT = typename Traits::FT;
    using Indices = std::vector<std::size_t>;
    using Size_pair = std::pair<std::size_t, std::size_t>;
    /// \endcond

    /// \name Initialization
    /// @{

    /*!
      \brief initializes all internal data structures.

      \param input_range
      a const range of input objects for shape regularization

      \param neighbor_query
      an instance of `NeighborQuery` that is used internally to
      access object neighbors

      \param regularization_type
      an instance of `RegularizationType` that is used internally to
      obtain object bounds and target values

      \param quadratic_program
      an instance of `QuadraticProgramTraits` to solve the quadratic programming problem

      \param traits
      an instance of `GeomTraits`, if not provided, the default is used

      \pre input_range.size() >= 2
    */
    QP_regularization(
      const InputRange& input_range,
      NeighborQuery& neighbor_query,
      RegularizationType& regularization_type,
      QuadraticProgramTraits& quadratic_program,
      const GeomTraits traits = GeomTraits()) :
    m_input_range(input_range),
    m_neighbor_query(neighbor_query),
    m_regularization_type(regularization_type),
    m_quadratic_program(quadratic_program),
    m_traits(traits),
    m_parameters(Parameters()) {

      CGAL_precondition(input_range.size() >= 2);
      clear();
    }

    /// @}

    /// \name Regularization
    /// @{

    /*!
      \brief runs the shape regularization algorithm.
    */
    void regularize() {
      if (m_input_range.size() < 2) return;

      // Graph = edges connecting neighbor segments.
      build_graph_of_neighbors();
      if (m_graph.size() == 0) {
        std::cerr << "Error: the number of edges in the graph is zero!" << std::endl;
        clear(); return;
      }

      // Bounds = number of input segments.
      obtain_bounds();
      if (m_bounds.size() == 0) {
        std::cerr << "Error: the number of bound values is zero!" << std::endl;
        clear(); return;
      }

      if (m_bounds.size() != m_input_range.size()) {
        std::cerr << "Error: the number of bounds is not equal to the number " <<
        "of input segments!" << std::endl;
        clear(); return;
      }

      // Targets = number of graph edges.
      obtain_targets();
      if (m_targets.size() == 0) {
        // std::cerr << "Warning: the number of target values is zero!" << std::endl;
        clear(); return;
      }

      // QP data.
      set_qp_data(
        m_quadratic_program);

      // Solve.
      std::vector<FT> solution;
      solve_quadratic_program(
        m_quadratic_program, solution);
      if (solution.size() != m_input_range.size() + m_targets.size()) {
        std::cerr << "Error: the number of solution values is not equal to the " <<
        "number of input segments + the number of edges in the graph!" << std::endl;
        clear(); return;
      }

      // Update.
      m_regularization_type.update(solution);
    }

    /// @}

    /// \name Memory Management
    /// @{

    /*!
      \brief clears all internal data structures.
    */
    void clear() {
      m_graph.clear();
      m_bounds.clear();
      m_targets.clear();
      m_max_bound = -FT(1);
    }

    /*!
      \brief releases all memory that is used internally.
    */
    void release_memory() {
      clear();
      m_bounds.shrink_to_fit();
    }

    /// @}

  private:
    const Input_range& m_input_range;
    Neighbor_query& m_neighbor_query;
    Regularization_type& m_regularization_type;
    Quadratic_program& m_quadratic_program;
    const Traits m_traits;
    const Parameters m_parameters;

    FT m_max_bound;
    std::set<Size_pair> m_graph;
    std::vector<FT> m_bounds;
    std::map<Size_pair, FT> m_targets;

    void build_graph_of_neighbors() {

      Size_pair pair;
      Indices neighbors;
      m_graph.clear();

      for (std::size_t i = 0; i < m_input_range.size(); ++i) {
        m_neighbor_query(i, neighbors);
        for (const std::size_t neighbor : neighbors) {
          i < neighbor ?
          pair = std::make_pair(i, neighbor) :
          pair = std::make_pair(neighbor, i);
          m_graph.insert(pair);
        }
      }
      CGAL_assertion(m_graph.size() > 0);
    }

    void obtain_bounds() {

      m_bounds.clear();
      m_bounds.reserve(m_input_range.size());
      m_max_bound = -FT(1);

      for (std::size_t i = 0; i < m_input_range.size(); ++i) {
        const FT bound = m_regularization_type.bound(i);
        CGAL_assertion(bound >= FT(0));
        m_bounds.push_back(bound);
        m_max_bound = (CGAL::max)(bound, m_max_bound);
      }

      CGAL_assertion(m_max_bound >= FT(0));
      CGAL_assertion(m_bounds.size() == m_input_range.size());
    }

    void obtain_targets() {

      m_targets.clear();
      for (const auto& pair : m_graph) {
        const std::size_t i = pair.first;
        const std::size_t j = pair.second;

        const FT target = m_regularization_type.target(i, j);
        if (CGAL::abs(target) <
          m_regularization_type.bound(i) + m_regularization_type.bound(j)) {
          m_targets[pair] = target;
        }
      }
      CGAL_assertion(m_targets.size() >= 0);
    }

    void set_qp_data(
      Quadratic_program& qp) const {

      const std::size_t k = m_input_range.size(); // k segments
      const std::size_t e = m_targets.size(); // e graph edges
      const std::size_t n = k + e; // number of variables
      const std::size_t m = 2 * e + n; // number of constraints

      set_quadratic_term(n, k, qp);
      set_linear_term(n, k, qp);
      set_constant_term(qp);
      set_constraint_matrix(n, k, qp);
      set_constraint_bounds(m, k, e, qp);
    }

    void set_quadratic_term(
      const std::size_t n,
      const std::size_t k,
      Quadratic_program& qp) const {

      qp.reserve_P(n);
      for (std::size_t i = 0; i < n; ++i) {
        FT value = FT(0);
        if (i < k) {
          value = FT(2) * m_parameters.weight * (FT(1) - m_parameters.lambda) /
            (m_bounds[i] * m_bounds[i] * FT(k));
        }
        qp.set_P(i, i, value);
      }
      // CGAL_assertion(qp.P_size() == n);
    }

    void set_linear_term(
      const std::size_t n,
      const std::size_t k,
      Quadratic_program& qp) const {

      qp.reserve_q(n);
      for (std::size_t i = 0; i < n; ++i) {
        FT value = FT(0);
        if (i >= k) {
          value = m_parameters.lambda * m_parameters.weight /
            (FT(4) * m_max_bound * FT(n - k));
        }
        qp.set_q(i, value);
      }
      // CGAL_assertion(qp.q_size() == n);
    }

    void set_constant_term(
      Quadratic_program& qp) const {

      const FT value = FT(0);
      qp.set_r(value);
    }

    void set_constraint_matrix(
      const std::size_t n,
      const std::size_t k,
      Quadratic_program& qp) const {

      CGAL_assertion(n >= k);
      std::size_t it = 0;
      std::size_t ij = k;

      const std::size_t A_nnz = m_targets.size() * 6 + n;
      qp.reserve_A(A_nnz);
      for (const auto& target : m_targets) {
        const std::size_t i = target.first.first;
        const std::size_t j = target.first.second;

        qp.set_A(it, i, m_parameters.val_neg);
        qp.set_A(it, j, m_parameters.val_pos);
        qp.set_A(it, ij, -FT(1));
        ++it;

        qp.set_A(it, i, m_parameters.val_pos);
        qp.set_A(it, j, m_parameters.val_neg);
        qp.set_A(it, ij, -FT(1));
        ++it; ++ij;
      }

      // Used to set bounds for each variable li <= xi <= ui.
      const std::size_t s = m_targets.size() * 2;
      for (std::size_t i = 0; i < n; ++i)
        qp.set_A(s + i, i, FT(1));

      // CGAL_assertion(qp.A_size() == A_nnz);
    }

    void set_constraint_bounds(
      const std::size_t m,
      const std::size_t k,
      const std::size_t e,
      Quadratic_program& qp) const {

      qp.reserve_l(m);
      qp.reserve_u(m);

      auto tit = m_targets.begin();
      for (std::size_t i = 0; i < m; ++i) {
        if (i < 2 * e) { // first 2 * e constraints
          const FT value = tit->second;
          if (i % 2 == 0) {
            qp.set_l(i, m_parameters.neg_inf);
            qp.set_u(i, m_parameters.val_neg * value);
          } else {
            qp.set_l(i, m_parameters.neg_inf);
            qp.set_u(i, m_parameters.val_pos * value); ++tit;
          }
        } else if (i < 2 * e + k) { // next k constraints
          qp.set_l(i, -FT(1) * m_max_bound);
          qp.set_u(i, +FT(1) * m_max_bound);
        } else { // last e constraints
          qp.set_l(i, m_parameters.neg_inf);
          qp.set_u(i, m_parameters.pos_inf);
        }
      }

      // CGAL_assertion(qp.l_size() == m); // all together m constraints
      // CGAL_assertion(qp.u_size() == m);
    }

    void solve_quadratic_program(
      Quadratic_program& qp,
      std::vector<FT>& solution) {

      const std::size_t n =
        m_input_range.size() + m_targets.size();

      solution.clear();
      solution.reserve(n);

      const auto success = qp.solve(
        std::back_inserter(solution));
      if (!success)
        std::cerr << "WARNING: The solver has not converged!" << std::endl;
      CGAL_assertion(solution.size() == n);
    }
  };

} // namespace Shape_regularization
} // namespace CGAL

#endif // CGAL_SHAPE_REGULARIZATION_QP_REGULARIZATION_H
