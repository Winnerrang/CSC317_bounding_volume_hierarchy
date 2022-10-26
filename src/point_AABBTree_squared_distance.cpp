#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include <limits>
#include <iostream>
struct compareDistance {
    bool operator()(std::pair<double, std::shared_ptr<Object>>& x, std::pair<double, std::shared_ptr<Object>>& y) {
        return x.first > y.first;
    }
};

bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here

    if (root == nullptr) return false;
    sqrd = std::numeric_limits<double>::infinity();
    std::priority_queue<std::pair<double, std::shared_ptr<Object>>, std::vector<std::pair<double, std::shared_ptr<Object>>>, compareDistance> Q;
    descendant = nullptr;
    Q.push(std::pair<double, std::shared_ptr<Object>>(point_box_squared_distance(query, root->box), root));

    while (!Q.empty()) {
       auto node = Q.top();
       Q.pop();
        

       auto tree = std::dynamic_pointer_cast<AABBTree>(node.second);

       // it is a point
       if (tree == nullptr) {
           if (node.first < sqrd && node.first <= max_sqrd && node.first >= min_sqrd) {
               sqrd = node.first;
               descendant = node.second;
           }
       }
       // it is an AABB tree
       else {
           if (tree->left != nullptr) {
               double dis = point_box_squared_distance(query, tree->left->box);

               if (dis < sqrd && dis < max_sqrd) {
                   Q.push(std::pair<double, std::shared_ptr<Object>>(dis, tree->left));
               }

               
           }

           if (tree->right != nullptr) {
               double dis = point_box_squared_distance(query, tree->right->box);

               if (dis < sqrd && dis < max_sqrd) {
                   Q.push(std::pair<double, std::shared_ptr<Object>>(dis, tree->right));
               }

               
           }
       }
    }

    if (descendant != nullptr) {
        return true;
    }
    else {
        sqrd = 0;
        return false;
    }
  ////////////////////////////////////////////////////////////////////////////
}
