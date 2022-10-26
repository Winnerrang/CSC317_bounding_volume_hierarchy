#include "ray_intersect_triangle.h"
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <assert.h>
#include <iostream>
bool ray_intersect_triangle_2D(const Ray& ray,
	const Eigen::RowVector3d& A,
	const Eigen::RowVector3d& B,
	const Eigen::RowVector3d& C,
	const double min_t,
	const double max_t,
	double& t);

bool ray_intersect_triangle(
  const Ray & ray,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  const double min_t,
  const double max_t,
  double & t)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
	
	Eigen::Vector3d AB = B.transpose() - A.transpose();
	Eigen::Vector3d AC = C.transpose() - A.transpose();
	
	Eigen::Vector3d normal = (AB).cross(AC);
	
	normal.normalize();
	
	double t_result;
	Eigen::Matrix3d M;
	M << AB, AC, -1 * ray.direction;
	
	
	if (std::fabs(M.determinant()) < 1e-8) {
		// when ray is coplaner with the triangle
		if (std::abs(normal.dot(ray.direction)) <= 1e-8) {
			return ray_intersect_triangle_2D(ray, A, B, C, min_t, max_t, t);
		}
		else {
			return false;
		}
	}


	Eigen::Vector3d result = M.inverse() * (ray.origin - A.transpose());
	double a = result.x();
	double b = result.y();
	t_result = result.z();

	if (a + b <= 1 && a >= 0 && b >= 0 && t_result >= min_t && t_result <= max_t) {
		t = t_result;
		return true;
	}
	else {
		return false;
	}
  ////////////////////////////////////////////////////////////////////////////
}

bool ray_intersect_triangle_2D(const Ray& ray,
	const Eigen::RowVector3d& A,
	const Eigen::RowVector3d& B,
	const Eigen::RowVector3d& C,
	const double min_t,
	const double max_t,
	double& t) {
	std::cout << "Ray and triangle are coplanar" << std::endl;
	assert(false);
	t = 0;
	return false;
}
