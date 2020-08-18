To discuss:
* When computing weights one by one, we recompute certain areas/distances multiple times and so using them e.g. in barycentric coordinates is not efficient. What should we do about that?
* Why do we have in the orbifold_Tutte_parameterizer "+tangent_weight" while in the MVC_post_processor it is "-tangent_weight"? I have fixed that.
* Do I need to cut-zero the dh weights in the orbifold_Tutte_parameterizer? (line 837) I do cut it for the moment. Otherwise, the results are not equal to the original version.
* Skeletonization uses the weird secure version for the cotangent weights.
* In skeletonization, the final example results are not determenistic.
* Should I remove the positive area from the Tangent_weight and substitute it by computing tan(alpha/2)? In this case, I will keep the correct sign in any configuration.
* Should I use reference in the traits?

Do it now:
* I should try to combine mvc and dhc in the orbifold parameterization. Not sure if this is necessary.
* Some packages require traits, which do not have a Point_2. What should we do in that case? See e.g. heat_method_3_concept. Almost fixed.
* Change to
  using GeomTraits = typename CGAL::Kernel_traits<
      typename boost::property_traits<VertexPointMap>::value_type>::type; Not sure if it is still necessary!

Docs:
* Make the user manual short.
* Clean up the reference manual.
* Mention that Tangent_weight_3 uses positive areas (no distortions) and can be used only for PMP, while MV_weight_2/3 e.g. can have different signs/distortions for 2D and 3D versions due to the flattenning of the 3D region.
* Mention that some functions do not use all objects from the traits class but only a subset.

Later:
* Cleanup tests.
* Comment the code.
* Add a concept test as in the heat_method.
* Check ordering of the points in the tools file.
* Improve the weighted Laplacian example.