* Comment the code.
* Spell check all files.
* Remove all unnecessary functions or put them in experimental.
* Cleanup Solver_interface one more time. Ask Simon to have a look.
* Check memory leaking.

* Install OSQP on all testing platforms.
* Add a 3D version of the QP regularization algorithm.
* Add an opencv tutorial with the global segments regularization over the Chicago map.
* Should we carry out a rigorous performance analysis?
* Do we actually care about the clockwise/counterclockwise order in the contours? Yes if it is an open contour. For closed contours, the visual result is identical however the order of the vertices is reversed. For open contours, the visual result is not identical.
