* Add 3D coordinates.
* Spell check all files.
* Is it ok license-wise that now this package depends on Mesh_2?
* I should merge this package with the natural neighbor coordinates package.
* I should use weights interface in order to compute discrete harmonic weights inside the harmonic solver.
* I should refactor harmonic coordinates and make its solver a parameter. In this case, I should also update the solver interface package.
* Why the 2D mesher does not generate boundary points exactly on the polygon boundary but only with the precision 10^-5?
* What about adding a demo with visualization of the coordinate functions?
* Finally, should we remove `in_tuple` and `in_pair` or should we leave them?
* I should add more construct_objects from the kernel instead of constructors.
* Add a test with traits_concept that checks if all necessary traits objects are provided.
* I should move internal weights to the weight interface.
* Comment the code.

Small extra fixes:
* check preconditions;
* fixed return statements;
* take geometric traits by const&;
* fix OutputIterator concept and params;
* no capital letter in the \param \tparam descriptions (everywhere);
* const Point_2 p2 = Point_2 and others, you can just write const Point_2 p2;
* add more objects from the traits class e.g. the sqrt object and/or centroid/midpoint;
* no end dot in \param \tparam descriptions if it's not a sentence (everywhere);
* fix traits concept if any by using function operator and spaces;
* flexible api - do I write it here.