To discuss:
* When computing weights one by one, we recompute certain areas/distances multiple times and so using them e.g. in barycentric coordinates is not efficient. What should we do about that?
* Why do we have in the orbifold_Tutte_parameterizer "+tangent_weight" while in the MVC_post_processor it is "-tangent_weight"? I have fixed that.
* Do I need to cut-zero the dh weights in the orbifold_Tutte_parameterizer? (line 837) I do cut it for the moment. Otherwise, the results are not equal to the original version.
* Skeletonization uses the weird secure version for the cotangent weights.
* In skeletonization, the final example results are not determenistic.
* Should I remove the positive area from the Tangent_weight and substitute it by computing tan(alpha/2)? In this case, I will keep the correct sign in any configuration.
* Should I use reference in the traits?

Do it now:
* I should try to combine mvc and dhc in the orbifold parameterization.
* Some packages require traits, which do not have a Point_2. What should we do in that case? See e.g. heat_method_3_concept.
* Change to
  using GeomTraits = typename CGAL::Kernel_traits<
      typename boost::property_traits<VertexPointMap>::value_type>::type;
* Add a concept test as in the heat_method.
* Remove my current examples from the user manual and use them only in the reference manual. While in the user manual add more complete examples:
  one that chooses a weight from the list, one that shows how to set up a Laplacian, one that shows how to compute barycentric coordinates, one that shows how to define your own traits class, one that shows how to normalize weights with weighting regions. Make the user manual short.
* Clean up the reference manual.
* Add tests.
* Mention that Tangent_weight_3 uses positive areas (no distortions) and can be used only for PMP, while MV_weight_2/3 e.g. can have different signs/distortions for 2D and 3D versions due to the flattenning of the 3D region.
* Or better use an arbitrary direction projection traits from the triangulation package for the polygonal weights.
* Comment the code.
* Add WP polygon example to the wachspress_weights.cpp file.
* Mention that some functions do not use all objects from the traits class but only a subset.