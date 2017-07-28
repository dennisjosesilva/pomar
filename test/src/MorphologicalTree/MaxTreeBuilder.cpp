#include "../../catch.hpp"
#include <MorphologicalTree/MorphologicalTreeBuilder.hpp>
#include <AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.hpp>
#include <memory>
#include <iostream>

using namespace pomar;

SCENARIO("Max tree should be built correctly.") {
  GIVEN("An image of size 3x3 with the following values 2,0,3,2,1,3,7,0,3,a 4-connected adjacency relation and a max-tree builder.") {
    std::vector<unsigned char> vertices {
	2,0,3,
        2,1,3,
	7,0,3
    };
    auto width = 3;
    auto height = 3;
    auto adj = Image2DAdjacencyRelationByTranslating::createFourConnectedAdjacency(width, height);
    MorphologicalTreeBuilder builder;
	
    WHEN ("The builder builds") {
      auto tree = builder.build<unsigned char>(vertices, std::move(adj), MorphologicalTreeBuilder::TreeType::MaxTree);

      THEN("The built tree should have 5 nodes") {
	REQUIRE(tree.getNumberofNodes() == 5);
      }
      THEN("it should navigate the nodes in the following (level, number of CNPs)  order: (7,1), (3,3), (2,2), (1,1), (0,2)") {
	std::vector<int> levels {7,3,2,1,0};
	std::vector<int> nCNPs  {1,3,2,1,2}; 
	int i = 0;

	tree.transverseFromLeavesToRoot([&levels, &nCNPs, &i] (const MTNode<unsigned char>& node) {
	    REQUIRE(levels[i] == node.level());
	    REQUIRE(nCNPs[i++] == node.vertexIndexes().size());
	});
      }
      THEN("It should return parent equals to (-1,0,1,1,2) for the nodes (0,1,2,3,4)") {
	REQUIRE(tree.getNodeParent(0) == -1);
	REQUIRE(tree.getNodeParent(1) == 0);
	REQUIRE(tree.getNodeParent(2) == 1);
	REQUIRE(tree.getNodeParent(3) == 1);
	REQUIRE(tree.getNodeParent(4) == 2);
      }
      THEN("It should return children equals to ({1},{2,3},{},{4},{}) for the nodes (0,1,2,3,4)") {
	REQUIRE(tree.getNodeChildren(0) == std::vector<int>({1}));
	REQUIRE(tree.getNodeChildren(1) == std::vector<int>({2,3}));
	REQUIRE(tree.getNodeChildren(2) == std::vector<int>({4}));
	REQUIRE(tree.getNodeChildren(3) == std::vector<int>());
	REQUIRE(tree.getNodeChildren(4) == std::vector<int>());		
      }
    }
  }
}
