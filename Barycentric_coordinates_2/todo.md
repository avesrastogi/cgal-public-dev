* Comment the code.
* Check memory leaking.
* Spell check all files.

* Is it ok license-wise that now this package depends on Mesh_2?
* Why the 2D mesher does not generate boundary points exactly on the polygon boundary but only with the precision 10^-5?
* The Project_traits classes are missing the construct_centroid_2 and compute_determinant_2 objects.
* Finally, should we remove `in_tuple` and `in_pair` or should we leave them?
* CGAL polygon is missing end and begin, which are documented.

* Add 3D coordinates.
* I should merge this package with the natural neighbor coordinates package.
* Improve the solver interface and make it a parameter for the harmonic coordinates class.
* What about adding a demo with visualization of the coordinate functions?
* I should move internal weights to the weight interface.
