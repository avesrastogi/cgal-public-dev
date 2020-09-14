* Comment the code.
* Add 3D coordinates.
* Spell check all files.
* Is it ok license-wise that now this package depends on Mesh_2?
* I should merge this package with the natural neighbor coordinates package.
* Why the 2D mesher does not generate boundary points exactly on the polygon boundary but only with the precision 10^-5?
* What about adding a demo with visualization of the coordinate functions?
* Finally, should we remove `in_tuple` and `in_pair` or should we leave them?
* I should move internal weights to the weight interface.

Small extra fixes:
* new line after \param and \tparam.
* fix OutputIterator concept and params.
* change positions of traits and vertex_map.
* no capital letter in the \param \tparam descriptions (everywhere).
* const Point_2 p2 = Point_2 and others, you can just write const Point_2 p2.
* add more objects from the traits class e.g. the sqrt object and/or centroid/midpoint/construct_vector.
* i should use weight interface in order to compute discrete harmonic weights inside the harmonic solver.
* i should refactor harmonic coordinates and make its solver a parameter. In this case, I should also update the solver interface package.
* add a test with traits_concept that checks if all necessary traits objects are provided.
* If the traits concept is covered by one from the weight interface, just use that one.
* no end dot in \param \tparam descriptions if it's not a sentence (everywhere).
* remove GeomTraits and VertexMap template parameters from harmonic coordinates.
* refactor utils into utils and polygon_utils, use weight_interface.
* fix traits concept if any by using function operator and spaces.
* remove CGAL::Barycentric_coordinates:: namespace from the docs.
* remove ; inside all bullet lists which start from -.
* flexible api - do I write it here.
* add review from Simon (see SR).
* types Polygon -> VertexRange?
* types VertexMap -> PointMap?