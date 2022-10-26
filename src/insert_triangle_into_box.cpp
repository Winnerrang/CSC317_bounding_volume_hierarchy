#include "insert_triangle_into_box.h"
#include <vector>;

void findLargestAndMinimum(const double x, const double y, const double z, double& min, double& max);
void insert_triangle_into_box(
  const Eigen::RowVector3d & a,
  const Eigen::RowVector3d & b,
  const Eigen::RowVector3d & c,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  // 
	findLargestAndMinimum(a.x(), b.x(), c.x(), B.min_corner.x(), B.max_corner.x());
	findLargestAndMinimum(a.y(), b.y(), c.y(), B.min_corner.y(), B.max_corner.y());
	findLargestAndMinimum(a.z(), b.z(), c.z(), B.min_corner.z(), B.max_corner.z());
  ////////////////////////////////////////////////////////////////////////////
}

void findLargestAndMinimum(const double x, const double y, const double z, double& min, double& max) {

	min = std::min(x, std::min(y, z));
	max = std::max(x, std::max(y, z));
}
