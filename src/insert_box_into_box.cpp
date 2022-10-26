#include "insert_box_into_box.h"

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
	if (A.min_corner.x() < B.min_corner.x()) {
		B.min_corner.x() = A.min_corner.x();
	}

	if (A.min_corner.y() < B.min_corner.y()) {
		B.min_corner.y() = A.min_corner.y();
	}

	if (A.min_corner.z() < B.min_corner.z()) {
		B.min_corner.z() = A.min_corner.z();
	}

	if (A.max_corner.x() > B.max_corner.x()) {
		B.max_corner.x() = A.max_corner.x();
	}

	if (A.max_corner.y() > B.max_corner.y()) {
		B.max_corner.y() = A.max_corner.y();
	}

	if (A.max_corner.z() > B.max_corner.z()) {
		B.max_corner.z() = A.max_corner.z();
	}
  ////////////////////////////////////////////////////////////////////////////
}

