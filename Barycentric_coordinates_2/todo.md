* Comment the code.
* Add 3D coordinates.
* Spell check all files.
* Is it ok license-wise that now this package depends on Mesh_2?
* I should merge this package with the natural neighbor coordinates package.
* Why the 2D mesher does not generate boundary points exactly on the polygon boundary but only with the precision 10^-5?
* The Project_traits classes are missing the construct_centroid_2 and compute_determinant_2 objects.
* Finally, should we remove `in_tuple` and `in_pair` or should we leave them?
* What about adding a demo with visualization of the coordinate functions?
* CGAL polygon is missing end and begin, which are documented.
* I should move internal weights to the weight interface.
* Check memory leaking.

Small extra fixes:
* I should refactor harmonic coordinates and make its solver a parameter. In this case, I should also update the solver interface package.
