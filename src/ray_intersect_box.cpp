#include "ray_intersect_box.h"
#include <iostream>
#include <vector>

#define DIMENSION 3
bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
	//take care of ray is inside the box, ray backward
	std::vector<double> minimum(3);
	std::vector<double> maximum(3);

	for (int i = 0; i < DIMENSION; i++){
		double inverse = 1 / ray.direction(i);
		if (inverse >= 0) {
			minimum[i] = (box.min_corner(i) - ray.origin(i)) * inverse;
			maximum[i] = (box.max_corner(i) - ray.origin(i)) * inverse;
		}
		else {
			maximum[i] = (box.min_corner(i) - ray.origin(i)) * inverse;
			minimum[i] = (box.max_corner(i) - ray.origin(i)) * inverse;
		}
		
	}

	double exit = *std::min_element(maximum.begin(), maximum.end());
	double enter = *std::max_element(minimum.begin(), minimum.end());

	if (enter < exit && exit >= min_t && enter <= max_t) {
		return true;
	}
	else {
		return false;
	}

  ////////////////////////////////////////////////////////////////////////////
}
