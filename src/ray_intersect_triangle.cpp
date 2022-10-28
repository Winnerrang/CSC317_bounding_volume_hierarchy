#include "ray_intersect_triangle.h"
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <assert.h>
#include <iostream>
#include <vector>
#include <limits>

#define X 0
#define Y 1
#define Z 2

bool ray_intersect_triangle_2D(const Ray& ray,
	const Eigen::RowVector3d& A,
	const Eigen::RowVector3d& B,
	const Eigen::RowVector3d& C,
	const double min_t,
	const double max_t,
	double& t);

bool rayRayIntersection2D(const Eigen::Vector2d o1,
	const Eigen::Vector2d d1,
	const double min_t_1,
	const double max_t_1,
	double& t1,
	const Eigen::Vector2d o2,
	const Eigen::Vector2d d2,
	const double min_t_2,
	const double max_t_2,
	double & t2);

bool rayRayIntersection1D(const Eigen::Vector2d o1,
	const Eigen::Vector2d d1,
	const double min_t_1,
	const double max_t_1,
	double& t1,
	const Eigen::Vector2d o2,
	const Eigen::Vector2d d2,
	const double min_t_2,
	const double max_t_2,
	double& t2);

static double crossProduct2D(const Eigen::Vector2d d1, const Eigen::Vector2d d2);

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
	
	auto n = ((B - A).cross(C - A)).normalized();
	double nx, ny, nz;
	nx = n.x();
	ny = n.y();
	nz = n.z();
	
	int drop, u, v;
	if (nx > ny && nx > nz) drop = X;
	else if (ny > nx && ny > nz) drop = Y;
	else drop = Z;

	u = (drop + 1) % 3;
	v = (drop + 2) % 3;

	std::vector<Eigen::Vector2d> triangle;

	triangle[0] = Eigen::Vector2d(A(u), A(v));
	triangle[1] = Eigen::Vector2d(B(u), B(v));
	triangle[2] = Eigen::Vector2d(C(u), C(v));

	auto o = Eigen::Vector2d(ray.origin(u), ray.origin(v));
	auto d = Eigen::Vector2d(ray.direction(u), ray.direction(v));

	bool found = false;
	t = std::numeric_limits<double>::infinity();
	for (int i = 0; i < 3; i++) {
		double t1, t2;
		if (rayRayIntersection2D(o, d, min_t, max_t, t1, triangle[i], triangle[(i + 1) % 3] - triangle[i], 0, 1, t2) && t < t1) {
			found = true;
			t = t1;
		}
	}

	return found;
	
}

bool rayRayIntersection2D(const Eigen::Vector2d o1,
	const Eigen::Vector2d d1,
	const double min_t_1,
	const double max_t_1,
	double& t1,
	const Eigen::Vector2d o2,
	const Eigen::Vector2d d2,
	const double min_t_2,
	const double max_t_2,
	double& t2) {
	
	auto denominator = crossProduct2D(d1, d2);
	auto p = o2 - o1;

	if (std::abs(denominator) < 1e-5) {
		rayRayIntersection1D(o1, d1, min_t_1, max_t_1, t1, o2, d2, min_t_2, max_t_2, t2);
	}

	t1 = crossProduct2D(p, d2) / denominator;

	if (t1 < min_t_1 || t1 > max_t_1) return false;

	t2 = crossProduct2D(p, d1) / denominator;

	if (t2 < min_t_2 || t2 > max_t_2) return false;

	return true;
}

static double crossProduct2D(const Eigen::Vector2d d1, const Eigen::Vector2d d2) {
	return d1[0] * d2[1] - d1[1] * d1[0];
}

bool rayRayIntersection1D(const Eigen::Vector2d o1,
	const Eigen::Vector2d d1,
	const double min_t_1,
	const double max_t_1,
	double& t1,
	const Eigen::Vector2d o2,
	const Eigen::Vector2d d2,
	const double min_t_2,
	const double max_t_2,
	double& t2) {
	if (std::abs(crossProduct2D(o2 - o1, d1)) < 1e-5) return false;
	
	int p;

	if (d1.x() > d1.y()) p = 0;
	else p = 1;

	double left, right;

	if (d1(p) * d2(p) > 0) {
		left = (o2(p) + d2(p) * min_t_2 - o1(p)) / d1(p);
		right = (o2(p) + d2(p) * max_t_2 - o1(p)) / d1(p);
	}
	else {
		right = (o2(p) + d2(p) * min_t_2 - o1(p)) / d1(p);
		left = (o2(p) + d2(p) * max_t_2 - o1(p)) / d1(p);
	}

	left = std::max(left, min_t_1);
	right = std::min(right, max_t_1);

	if (left > right) return false;

	t1 = left;

	t2 = (o1(p) + t1 * d1(p) - o2(p)) / d2(p);

	return true;
}