#include "../../catch.hpp"
#include <pomar/ComponentTree/CTSorter.hpp>
#include <pomar/ComponentTree/CTBuilder.hpp>
#include <pomar/AdjacencyRelation/AdjacencyByTranslating.hpp>
#include <memory>
#include <algorithm>

using namespace pomar;

SCENARIO("Max-tree sorter should sort any given elements set.") {
  GIVEN("An unsigned char element set with the following values (2,5,3,0,2,3,0,3)") {    
    std::vector<unsigned char> v = {2,5,3,0,2,3,0,3};
    WHEN("Max-tree sorter sorts the array") {
      auto sortedIndices = maxTreeSort(v);
      THEN("the result should be (3,6,0,4,2,5,7,1)") {
        std::vector<int> expected = {3,6,0,4,2,5,7,1};
        REQUIRE (sortedIndices == expected);
      }
    }
  }
  GIVEN("An long element set with the following values (2,5,3,0,2,3,0,3)") {    
    std::vector<long> v = {2,5,3,0,2,3,0,3};
    WHEN("Max-tree sorter sorts the array") {
      auto sortedIndices = maxTreeSort(v);
      THEN("the result should be (3,6,0,4,2,5,7,1)") {
        std::vector<int> expected = {3,6,0,4,2,5,7,1};
        REQUIRE (sortedIndices == expected);
      }
    }
  }
}

SCENARIO("Min-tree sorter should sort any given elements set") {
  GIVEN("An unsigned char element set with the following values (2,5,3,0,2,3,0,3)") {    
    std::vector<unsigned char> v = {2,5,3,0,2,3,0,3};
    WHEN("Min-tree sorter sorts the array") {
      auto sortedIndices = minTreeSort(v);
      THEN("The result should be (1,2,5,7,0,4,3,6)") {
        std::vector<int> expected = {1,2,5,7,0,4,3,6};
        REQUIRE(sortedIndices == expected);
      }
    }
  }
  GIVEN("An long element set with the following values (2,5,3,0,2,3,0,3)") {    
    std::vector<long> v = {2,5,3,0,2,3,0,3};
    WHEN("Min-tree sorter sorts the array") {
      auto sortedIndices = minTreeSort(v);
      THEN("The result should be (1,2,5,7,0,4,3,6)") {
        std::vector<int> expected = {1,2,5,7,0,4,3,6};
        REQUIRE(sortedIndices == expected);
      }
    }
  }
}

SCENARIO("Component tree builder should be built correctly.") {
  GIVEN("An image of size 3x3 with the following values 2,0,3,2,1,3,7,0,3,a 4-connected adjacency relation and a component tree builder.") {
    std::vector<unsigned char> elements {
      	2,0,3,
        2,1,3,
      	7,0,3
    };
    auto width = 3;
    auto height = 3;
    std::unique_ptr<Adjacency> adj = AdjacencyByTranslating2D::createAdjacency4(width, height);
    CTBuilder builder;
    auto meta = std::make_shared<CTMetaImage2D>(width, height, 1);

    WHEN ("The builder builds a max-tree") {
      auto tree = builder.build(meta, elements, std::move(adj),CTBuilder::TreeType::MaxTree);

      THEN("The built tree should have 5 nodes") {
	      REQUIRE(tree.numberOfNodes() == 5);
      }
      THEN("it should navigate the nodes in the following (level, number of CNPs)  order: (7,1), (3,3), (2,2), (1,1), (0,2)") {
      	std::vector<int> levels {7,3,2,1,0};
      	std::vector<int> nCNPs  {1,3,2,1,2};
      	int i = 0;

      	tree.transverse([&levels, &nCNPs, &i] (const CTNode<unsigned char>& node) {
      	    REQUIRE(levels[i] == node.level());
      	    REQUIRE(nCNPs[i++] == node.elementIndices().size());
      	});
      }
      THEN("It should return parent equals to (-1,0,1,1,2) for the nodes (0,1,2,3,4)") {
      	REQUIRE(tree.nodeParent(0) == -1);
      	REQUIRE(tree.nodeParent(1) == 0);
      	REQUIRE(tree.nodeParent(2) == 1);
      	REQUIRE(tree.nodeParent(3) == 1);
      	REQUIRE(tree.nodeParent(4) == 2);
      }
      THEN("It should return children equals to ({1},{2,3},{},{4},{}) for the nodes (0,1,2,3,4)") {
      	REQUIRE(tree.nodeChildren(0) == std::vector<int>({1}));
      	REQUIRE(tree.nodeChildren(1) == std::vector<int>({2,3}));
      	REQUIRE(tree.nodeChildren(2) == std::vector<int>({4}));
      	REQUIRE(tree.nodeChildren(3) == std::vector<int>());
      	REQUIRE(tree.nodeChildren(4) == std::vector<int>());
      }
      THEN("It should reconstruct a node with id = 1 returning the following: (0,2,3,4,5,6,8)") {
      	auto rnode = tree.reconstructNode(1);
      	std::sort(rnode.begin(), rnode.end());
      	REQUIRE(rnode == std::vector<int>({0,2,3,4,5,6,8}));
      }
    }

    WHEN ("The builder builds a min-tree") {
      auto tree = builder.build(meta, elements, std::move(adj), CTBuilder::TreeType::MinTree);

      THEN("The built tree should have 6 nodes") {
        REQUIRE(tree.numberOfNodes() == 6);
      }
      THEN("It should navigate the nodes in the following (level, number of CNPs), order: (0,1), (0,1), (1,1), (2,2), (3,3), (7,1)") {
        std::vector<int> levels = {0,0,1,2,3,7};
        std::vector<int> nCNPs = {1,1,1,2,3,1};
        int i = 0;

        tree.transverse([&levels, &nCNPs, &i](const CTNode<unsigned char> &node) {
          REQUIRE(levels[i] == node.level());
          REQUIRE(nCNPs[i++] == node.elementIndices().size());
        });
      }
      THEN("It should return parent equals to (-1,0,1,2,3,3) for the nodes (0,1,2,3,4,5)") {
        REQUIRE(tree.nodeParent(0) == -1);
        REQUIRE(tree.nodeParent(1) ==  0);
        REQUIRE(tree.nodeParent(2) ==  1);
        REQUIRE(tree.nodeParent(3) ==  2);
        REQUIRE(tree.nodeParent(4) ==  3);
        REQUIRE(tree.nodeParent(5) ==  3);
      }
      THEN("It should return children equals to ({1}, {2}, {3}, {4,5}, {}, {})") {
        REQUIRE(tree.nodeChildren(0) == std::vector<int>({1}));
        REQUIRE(tree.nodeChildren(1) == std::vector<int>({2}));
        REQUIRE(tree.nodeChildren(2) == std::vector<int>({3}));
        REQUIRE(tree.nodeChildren(3) == std::vector<int>({4,5}));
        REQUIRE(tree.nodeChildren(4) == std::vector<int>());
        REQUIRE(tree.nodeChildren(5) == std::vector<int>());
      }
      THEN ("It should reconstruct a node with id = 1 returning the following: (0,1,2,3,4,5,7,8)") {
        auto rnode = tree.reconstructNode(1);
        std::sort(rnode.begin(), rnode.end());
        REQUIRE(rnode == std::vector<int>({0,1,2,3,4,5,7,8}));
      }
    }
  }
}
