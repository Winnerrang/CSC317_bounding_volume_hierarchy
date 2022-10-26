#include "nearest_neighbor_brute_force.h"
#include <limits>// std::numeric_limits<double>::infinity();

void nearest_neighbor_brute_force(
  const Eigen::MatrixXd & points,
  const Eigen::RowVector3d & query,
  int & I,
  double & sqrD)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
	
	I = -1;
	sqrD = std::numeric_limits<double>::infinity();
	for (int i = 0; i < points.rows(); i++) {
		const Eigen::RowVector3d dis = points.row(i) - query;
		double newSqrD = dis.x() * dis.x() + dis.y() * dis.y() + dis.z() * dis.z();

		if (newSqrD < sqrD) {
			sqrD = newSqrD;
			I = i;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////
}
