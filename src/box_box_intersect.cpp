#include "box_box_intersect.h"
bool box_box_intersect(
	const BoundingBox& A,
	const BoundingBox& B)
{
	////////////////////////////////////////////////////////////////////////////
	// Replace with your code here:

	bool x_overlap, y_overlap, z_overlap;
	x_overlap = A.max_corner.x() >= B.min_corner.x() && A.min_corner.x() <= B.max_corner.x();
	y_overlap = A.max_corner.y() >= B.min_corner.y() && A.min_corner.y() <= B.max_corner.y();
	z_overlap = A.max_corner.z() >= B.min_corner.z() && A.min_corner.z() <= B.max_corner.z();
	return x_overlap && y_overlap && z_overlap;
	////////////////////////////////////////////////////////////////////////////
}
