#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
namespace kdtree_homemade {
	class kd_Node {
	public:
		kd_Node(glm::dvec3& pos, glm::dvec3& flu) : position(pos), flux(flu) { left_node = nullptr; right_node = nullptr; depth = 0; }

		kd_Node() : position(glm::dvec3(0)),flux(glm::dvec3(0)),depth(0),left_node(nullptr),right_node(nullptr) {

		}
	
		glm::dvec3 position;
		glm::dvec3 flux;
		int depth;
		kd_Node* left_node;
		kd_Node* right_node;
	};

	class kdNodes_homemade{

		
	public:

		void push_back(glm::dvec3& pos, glm::dvec3& flu) {
			
			Nodes.push_back(kd_Node(pos,flu));
		}
		void push_back(kd_Node& Node){ 
			Nodes.push_back(Node);
		}
		std::vector<kd_Node> Nodes;
		
	};
	class kdTree_homemade
	{
	public:
		kd_Node* root;
		/*kdTree_homemade(kdNodes_homemade& nodes) {
			std::sort(nodes.Nodes.begin(), nodes.Nodes.end(), [](const kd_Node& a, const kd_Node& b) {
				return a.position.x < b.position.x;
				});
			size_t mid = nodes.Nodes.size()/2;
			glm::dvec3 median = nodes.Nodes[mid].position;
			
			

		}*/

		kd_Node* buildTree(std::vector<kd_Node*>& nodes, int depth) {
			if (nodes.empty()) {
				return nullptr; //basecase
			}
			
			int axis = depth % 3;

			std::sort(nodes.begin(), nodes.end(), [axis](const kd_Node* a, const kd_Node* b) {
				
				return a->position[axis] < b->position[axis];
				});

			size_t medianIndex = nodes.size() / 2;
			kd_Node* medianNode = nodes[medianIndex];

			std::vector<kd_Node*> leftNodes(nodes.begin(), nodes.begin() + medianIndex);
			std::vector<kd_Node*> rightNodes(nodes.begin() + medianIndex + 1, nodes.end());

			medianNode->left_node = buildTree(leftNodes, depth + 1);
			medianNode->right_node = buildTree(rightNodes, depth + 1);

			return medianNode;
		}
		void deleteTree(kd_Node* node) {
			if (node != nullptr) {
				
				deleteTree(node->left_node);
				deleteTree(node->right_node);
				delete node;
				node = nullptr;
			}
		}
		
		kdTree_homemade(std::vector<kd_Node*>& nodes) : root(nullptr) {
			root = buildTree(nodes, 0);
		}

		~kdTree_homemade() {
			deleteTree(root);
		}

		kd_Node* getRoot() const {
			return root;
		}

		void find_photons(std::vector<kd_Node*>& close_photons,kd_Node* root,const glm::dvec3& v_0,const double& r)  const {
			
			if (root == nullptr) {
				
				return;
			}
			
			int axis = root->depth % 3;
			if (v_0[axis] - r <= root->position[axis] && v_0[axis] + r >= root->position[axis]) {
				int axis_next = (root->depth + 1) % 3;
				int axis_nextnext = (root->depth + 2) % 3;

				if (v_0[axis_next] - r <= root->position[axis_next] && v_0[axis_next] + r >= root->position[axis_next] && v_0[axis_nextnext] - r <= root->position[axis_nextnext] && v_0[axis_nextnext] + r >= root->position[axis_nextnext]) {
					close_photons.push_back(root);
				}
				find_photons(close_photons, root->left_node, v_0, r);
				find_photons(close_photons, root->right_node, v_0, r);
			}
			//axis is larger than min value but also larger than max value, exlude values on right in tree.
			else if (v_0[axis] - r <= root->position[axis]) {
				
				find_photons(close_photons, root->left_node, v_0, r);
				
			}
			//axis is smaller than max value but also smaller than min value, exlude values on left in tree.
			else if (v_0[axis] + r >= root->position[axis]) {
				
				find_photons(close_photons, root->right_node, v_0, r);
			}
			else {
				
			}
		}
	};
}


