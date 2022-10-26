#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"
#include <limits>
bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
	double t;
	hit_t = 0;
	hit_f = -1;
	hit_t = std::numeric_limits<double>::infinity();
	for (int f = 0; f < F.rows(); f++) {

		// the ray does not intersect the triangle
		if (! ray_intersect_triangle(ray, V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)), min_t, max_t, t)){
			continue;
		}

		if (t < hit_t) {
			hit_t = t;
			hit_f = f;
		}
	}

	if (hit_f != -1) {
		return true;
	}
	else {
		return false;
	}
  
	
  ////////////////////////////////////////////////////////////////////////////
}
