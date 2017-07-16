#include "../../../catch.hpp"
#include <AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.hpp>

using namespace pomar;

SCENARIO("Image2DAdjacencyRelationByTranslating initialize correctly") {
  GIVEN("An adjacency relation defined by the translating of the two points: { (-1, 0), (1,0) } and a image size of (3,1)") {
    int width = 3, height = 1;
    std::vector<int> dx{-1, 1};
    std::vector<int> dy{0, 0};
    Image2DAdjacencyRelationByTranslating adj(width, height, dx, dy);

    WHEN("compute adjacent points of (1,0)") {
      size_t p = 1;
      std::vector<int> ap = adj.getAdjacentElements(p);
      THEN("the adjacent points should be { (0,0), (2,0) }") {
	REQUIRE(ap[0] == 0);
	REQUIRE(ap[1] == 2);
      }
      THEN("the adjacent points set should contains 2 elements") {
	REQUIRE(ap.size() == 2);
      }
    }
    WHEN("compute adjacent points of (2,0)") {
      size_t p = 2;
      std::vector<int> ap = adj.getAdjacentElements(p);
      THEN("the adjacent points should be { (1,0), NoAdjacentIndex }") {
	REQUIRE(ap[0] == 1);
	REQUIRE(ap[1] == Image2DAdjacencyRelationByTranslating::NoAdjacentIndex);
      }
      THEN("the adjacent point set should contains 2 elements") {
	REQUIRE(ap.size() == 2);
      }
    }
  }
}
  
  /*
SCENARIO("Image2DAdjacencyRelationByTranslating initialize 8-,4-connectivitily correctly") {
}
  */
