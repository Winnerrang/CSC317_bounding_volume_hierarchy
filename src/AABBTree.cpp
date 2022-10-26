#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <limits>
#include <assert.h>
#include <iostream>

#define DIMENSION 3
#define X 0
#define Y 1
#define Z 2
void quickSelect(std::vector<std::shared_ptr<Object>>& objects, int target, int axis, int left, int right);
static void assertWithMessage(std::string s, bool condition);

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

	//corner cases
	switch (objects.size()) {
		case 0:
			this->left = nullptr;
			this->right = nullptr;
			return;
		case 1:
			this->box = objects[0]->box;
			this->left = objects[0];
			this->right = nullptr;
			return;
		case 2:
			this->left = objects[0];
			this->right = objects[1];
			insert_box_into_box(this->left->box, this->box);
			insert_box_into_box(this->right->box, this->box);
			return;
	}

	Eigen::Vector3d min, max, range;
	int longestAxis;
	min = Eigen::Vector3d::Constant(3, 1, std::numeric_limits<double>::infinity());
	max = Eigen::Vector3d::Constant(3, 1, -std::numeric_limits<double>::infinity());

	// loop through every single triangle, add each triangle to the box
	for (auto ptr : objects) {
		for (int d = 0; d < DIMENSION; d++) {
			if (ptr->box.center()(d) < min(d)) {
				min(d) = ptr->box.center()(d);
			}

			if (ptr->box.center()(d) > max(d)) {
				max(d) = ptr->box.center()(d);
			}
		}
	}
	range = max - min;

	if (range.x() > range.y() && range.x() > range.z()) {
		longestAxis = X;
	}
	else if (range.y() > range.x() && range.y() > range.z()) {
		longestAxis = Y;
	}
	else {
		longestAxis = Z;
	}

	auto tempObjects(objects);

	int middle = (int)tempObjects.size() >> 1;
	quickSelect(tempObjects, middle, longestAxis, 0, (int) tempObjects.size() - 1);
	
	// make two new array split the original array into half
	std::vector<std::shared_ptr<Object>> left(tempObjects.begin(), tempObjects.begin() + middle);
	std::vector<std::shared_ptr<Object>> right(tempObjects.begin() + middle, tempObjects.end());

	assertWithMessage("left has size 0", (int)left.size() != 0);
	assertWithMessage("right has size 0", (int)right.size() != 0);
	/*
	for (auto obj : left) {
		assert(obj->box.center()(longestAxis) <= tempObjects[middle]->box.center()(longestAxis));
	}

	for (auto obj : right) {
		assert(obj->box.center()(longestAxis) >= tempObjects[middle]->box.center()(longestAxis));
	}*/

	if (left.size() > 1) {
		this -> left = std::make_shared<AABBTree>(left, depth + 1);	
	}
	else {
		assert(left.size() == 1);
		this->left = left[0];
	}
	
	if (right.size() > 1) {
		this->right = std::make_shared<AABBTree>(right, depth + 1);
	}
	else {
		assert(right.size() == 1);
		this->right = right[0];
	}

	insert_box_into_box(this->left->box, this->box);
	insert_box_into_box(this->right->box, this->box);

	
  ////////////////////////////////////////////////////////////////////////////
}

void quickSelect(std::vector<std::shared_ptr<Object>>& objects, int target, int axis, int left, int right) {
	assertWithMessage("left is bigger than right", left <= right);
	assertWithMessage("left is smaller than 0", left >= 0);
	assertWithMessage("right is too large", right < (int)objects.size());
	//std::cout << target << " " << left << " " << right << std::endl;
	if (left == right) {
		assertWithMessage("The quick select algrithm is wrong",left == target);
	}

	auto pivot = right;
	auto pivot_value = objects[pivot]->box.center()(axis);
	int i = left - 1;

	for (int j = left; j <= right - 1; j++) {
		if (objects[j]->box.center()(axis) < pivot_value) {
			i++;
			auto temp = objects[i];
			objects[i] = objects[j];
			objects[j] = temp;
		}

	}
	i++;
	assertWithMessage("i is out of bound", i>=0 && i<objects.size());
	assertWithMessage("pivot is out of bound", pivot >= 0 && pivot < objects.size());
	auto temp = objects[i];
	//std::cout << i << " " << pivot << std::endl;
	objects[i] = objects[pivot];
	objects[pivot] = temp;

	if (i == target) return;
	else if (i < target) quickSelect(objects, target, axis, i + 1, right);
	else quickSelect(objects, target, axis, left, i - 1);
}

static void assertWithMessage(std::string s, bool condition) {
	
	if (!condition) {
		std::cout << s << std::endl;
		assert(false);
	}
}