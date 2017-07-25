#include "../../catch.hpp"
#include <MorphologicalTree/MorphologicalTreeBuilder.hpp>

#include <memory>

using namespace pomar;

SCENARIO("Max tree should be built correctly.") {
  GIVEN("An image of size 3x3 with the following values 2,0,3,2,1,3,7,0,3,a 4-connected adjacency relation and a max-tree builder.") {
    std::vector<unsigned char> vertices {
      3,0,3,
      2,1,3,
      7,0,3
    };
    auto width = 3;
    auto height = 3;
    auto adj = Image2DAdjacencyRelationByTranslating::createFourConnectedAdjacency(width, height);
    MaxTreeBuilder builder;
    
    WHEN ("The builder builds") {
      MorphologicalTree<unsigned char> tree = builder.build(vertices, *adj);

      THEN("The built tree should have 5 nodes") {
	REQUIRE(tree.getNumberofNodes() == 5);
      }
    }
  }
}
