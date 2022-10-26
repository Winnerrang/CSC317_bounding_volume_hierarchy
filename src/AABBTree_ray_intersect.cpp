#include "AABBTree.h"
#include "ray_intersect_box.h"
#include <iostream>
#include <assert.h>
#include <MeshTriangle.h>
#include <limits>
static void assertWithMessage(std::string s, bool condition);


// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const 
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:


	//check if it intersect the bounding box
	if (!ray_intersect_box(ray, this->box, min_t, max_t)) return false;


	//check if the ray intersect the children
	double left_t, right_t;
	std::shared_ptr<Object> leftDescendant, rightDescendant;
	bool leftFound, rightFound;
	assertWithMessage("both children are nullptr", this->left != nullptr || this->right != nullptr);

	
	//left tree
	leftFound = this->left != nullptr && ray_intersect_box(ray, this->left->box, min_t, max_t);
	

	leftFound = leftFound && this->left->ray_intersect(ray, min_t, max_t, left_t, leftDescendant);
	if (leftFound && std::dynamic_pointer_cast<MeshTriangle>(this->left) != nullptr) {
		leftDescendant = this->left;
	}
	
	
	
	//right tree
	rightFound = this->right != nullptr && ray_intersect_box(ray, this->right->box, min_t, max_t);
	if (leftFound) {
		Ray newRay(ray.origin + left_t * ray.direction, -ray.direction);

		//check if the reversed ray starting from the hit point hit the right bounding box
		rightFound = rightFound && ray_intersect_box(newRay, this->right->box, 0, std::numeric_limits<double>::infinity());

	}
	
	rightFound = rightFound && this->right->ray_intersect(ray, min_t, max_t, right_t, rightDescendant);
	
	if (rightFound && std::dynamic_pointer_cast<MeshTriangle>(this->right) != nullptr) {
		rightDescendant = this->right;
	}




	if (leftFound && rightFound) {
		if (left_t < right_t) {
			t = left_t;
			descendant = leftDescendant;
			return true;
		}
		else {
			t = right_t;
			descendant = rightDescendant;
			return true;
		}
	}
	else if (leftFound) {
		t = left_t;
		descendant = leftDescendant;
		return true;
	}
	else if (rightFound) {
		t = right_t;
		descendant = rightDescendant;
		return true;
	}
	else {
		t = 0;
		return false;
	}

	
	
	/**
	leftFound = this->left != nullptr && this->left->ray_intersect(ray, min_t, max_t, left_t, leftDescendant);

	if (std::dynamic_pointer_cast<MeshTriangle>(this->left) != nullptr) {
		leftDescendant = this->left;
	}

	rightFound = this->right != nullptr && this->right->ray_intersect(ray, min_t, max_t, right_t, rightDescendant);
	
	if (std::dynamic_pointer_cast<MeshTriangle>(this->right) != nullptr) {
		rightDescendant = this->right;
	}

	if (leftFound && rightFound) {
		if (left_t < right_t) {
			t = left_t;
			descendant = leftDescendant;
			return true;
		}
		else {
			t = right_t;
			descendant = rightDescendant;
			return true;
		}
	}
	else if (leftFound) {
		t = left_t;
		descendant = leftDescendant;
		return true;
	}
	else if (rightFound) {
		t = right_t;
		descendant = rightDescendant;
		return true;
	}
	else {
		t = 0;
		return false;
	}*/

  ////////////////////////////////////////////////////////////////////////////
}

//this helper function assume it will hit this object bounding box

static void assertWithMessage(std::string s, bool condition) {

	if (!condition) {
		std::cout << s << std::endl;
		assert(false);
	}
}

