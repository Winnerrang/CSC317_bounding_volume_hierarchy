#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"

// Hint: use a list as a queue
#include <list> 
typedef std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>> BoxPair;

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > & 
    leaf_pairs)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  
    std::list<BoxPair> q;
    std::shared_ptr<Object> nodeA, nodeB;
    if (rootA == nullptr || rootB == nullptr) return;

    
    // the tree don't have a nullptr most of the time unless it only
    // have one element in it. Therefore we need to use the one element
    // for comparison so that nullptr case will not exist in the while loop.
    // Tree has 0 elements but is not nullptr is handled for defensive
    // coding.
    if (rootA->left == nullptr && rootA->right == nullptr) return;
    else if (rootA->left == nullptr) nodeA = rootA->right;
    else if (rootA->right == nullptr) nodeA = rootA->left;
    else nodeA = rootA;

    if (rootB->left == nullptr && rootB->right == nullptr) return;
    else if (rootB->left == nullptr) nodeB = rootB->right;
    else if (rootB->right == nullptr) nodeB = rootB->left;
    else nodeB = rootB;

    if (!box_box_intersect(nodeA->box, nodeB->box)) return;
    q.push_back(BoxPair(nodeA, nodeB));

    while (!q.empty()) {
        auto node = q.front();
        q.pop_front();

        auto firstPtr = std::dynamic_pointer_cast<AABBTree>(node.first);
        auto secondPtr = std::dynamic_pointer_cast<AABBTree>(node.second);

        if (firstPtr == nullptr && secondPtr == nullptr) leaf_pairs.push_back(BoxPair(node.first, node.second));
        else if (firstPtr == nullptr) {
            if (box_box_intersect(node.first->box, secondPtr->left->box)) q.push_back(BoxPair(node.first, secondPtr->left));

            if (box_box_intersect(node.first->box, secondPtr->right->box)) q.push_back(BoxPair(node.first, secondPtr->right));

        }
        else if (secondPtr == nullptr) {
            if (box_box_intersect(firstPtr->left->box, node.second->box)) q.push_back(BoxPair(firstPtr->left, node.second));

            if (box_box_intersect(firstPtr->right->box, node.second->box)) q.push_back(BoxPair(firstPtr->right, node.second));
        }
        else {

            if (box_box_intersect(firstPtr->left->box, secondPtr->left->box)) q.push_back(BoxPair(firstPtr->left, secondPtr->left));

            if (box_box_intersect(firstPtr->left->box, secondPtr->right->box)) q.push_back(BoxPair(firstPtr->left, secondPtr->right));

            if (box_box_intersect(firstPtr->right->box, secondPtr->left->box)) q.push_back(BoxPair(firstPtr->right, secondPtr->left));

            if (box_box_intersect(firstPtr->right->box, secondPtr->right->box)) q.push_back(BoxPair(firstPtr->right, secondPtr->right));
        }
    }
  ////////////////////////////////////////////////////////////////////////////
}
