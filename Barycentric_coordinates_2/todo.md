* Is it ok license-wise that now this package depends on Mesh_2?
* I should refactor harmonic coordinates and make its solver a parameter. In this case, I should also update the solver interface package.
* I should merge this package with the natural neighbor coordinates package.
* I should use weights interface in order to compute discrete harmonic weights inside the harmonic solver.
* Why the 2D mesher does not generate boundary points exactly on the polygon boundary but only with precision 10^-5?
* What about adding a demo with visualization of the coordinate functions?
* Add 3D coordinates.
* Add a table that compares properties of all available coordinates.