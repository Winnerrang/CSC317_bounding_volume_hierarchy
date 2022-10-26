#include "point_box_squared_distance.h"

#define DIMENSION 3

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here
	Eigen::Vector3d dis = Eigen::RowVector3d::Constant(1, 3, std::numeric_limits<double>::infinity());

	for (int i = 0; i < DIMENSION; i++) {
		if (query(i) > box.max_corner(i)) {
			dis(i) = query(i) - box.max_corner(i);
		}
		else if (query(i) < box.min_corner(i)) {
			dis(i) = box.min_corner(i) - query(i);
		}
		else {
			dis(i) = 0;
		}
	}


	return dis.x() * dis.x() + dis.y() * dis.y() + dis.z() * dis.z();
  ////////////////////////////////////////////////////////////////////////////
}
