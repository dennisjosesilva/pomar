#include "../../catch.hpp"
#include <pomar/AdjacencyRelation/AdjacencyByTranslating.hpp>
#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/Math/Point.hpp>

using namespace pomar;

SCENARIO("AdjacencyByTranslating2D initialise correctly") {
  GIVEN("An adjacency relation defined by the translating of the two points: { (-1, 0), (1,0) } and a image size of (3,1)") {
    int width = 3, height = 1;
    std::vector<IPoint2D> dt{{-1,0}, {1,0}};
    AdjacencyByTranslating2D adj(width, height, dt);

    WHEN("compute adjacent points of (1,0)") {
      size_t p = 1;
      std::vector<int> ap = adj.neighbours(p);
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
      std::vector<int> ap = adj.neighbours(p);
      THEN("the adjacent points should be { (1,0), NoAdjacentIndex }") {
      	REQUIRE(ap[0] == 1);
      	REQUIRE(ap[1] == Adjacency::NoAdjacentIndex);
      }
      THEN("the adjacent point set should contains 2 elements") {
	       REQUIRE(ap.size() == 2);
      }
    }
  }
}


SCENARIO("AdjacencyByTranslating2D initialize 8-,4-connectivitily correctly") {

  GIVEN("An 8-connected adjacency relation for an image of size (3,3)") {
    int width = 3,  height = 3;
    std::unique_ptr<Adjacency> adj = AdjacencyByTranslating2D::createAdjacency8(width, height);

    WHEN("compute adjacent points of (1,1)") {
      size_t pc = 1*width + 1;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent points should be { (0,0), (1,0), (2,0), (2,1), (2,2), (1,2), (0,2), (0,1) }") {
      	REQUIRE(ap[0] == 0); REQUIRE(ap[1] == 1); REQUIRE(ap[2] == 2); REQUIRE(ap[3] == 5);
      	REQUIRE(ap[4] == 8); REQUIRE(ap[5] == 7); REQUIRE(ap[6] == 6); REQUIRE(ap[7] == 3);
      }
      THEN("the adjacent points set should contains 8 elements") {
	       REQUIRE(ap.size() == 8);
      }
    }
    WHEN("compute adjacent points with center at (0,0)") {
      size_t pc = 0;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent points should be { (1,0), (1,1), (0,3), NoIndex, NoIndex, NoIndex, NoIndex, NoIndex }") {
      	REQUIRE(ap[0] == 1); REQUIRE(ap[1] == 4); REQUIRE(ap[2] == 3);
      	REQUIRE(ap[3] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[4] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[5] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[6] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[7] == Adjacency::NoAdjacentIndex);
      }
      THEN("the adjacent points set should contains 8 elements") {
	      REQUIRE(ap.size() == 8);
      }
    }
    WHEN("compute adjacent points with center at (2,2)") {
      size_t pc = 2*width + 2;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent point should be { (1,1), (2,1), (1,2), NoIndex, NoIndex, NoIndex, NoIndex, NoIndex }") {
      	REQUIRE(ap[0] == 4); REQUIRE(ap[1] == 5); REQUIRE(ap[2] == 7);
      	REQUIRE(ap[3] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[4] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[5] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[6] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[7] == Adjacency::NoAdjacentIndex);
      }
      THEN("the adjacent points set should contains 8 elements") {
	      REQUIRE(ap.size() == 8);
      }
    }
  }

  GIVEN("An 4-connected adjacency relation for an image of size (3,3)") {
    int width = 3, height = 3;
    std::unique_ptr<Adjacency> adj = AdjacencyByTranslating2D::createAdjacency4(width, height);

    WHEN("compute adjacent points of (1,1)") {
      size_t pc = 1 * width + 1;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent points should be { (0,1), (1,1), (2,1), (1,2) }") {
	       REQUIRE(ap[0] == 3); REQUIRE(ap[1] == 1); REQUIRE(ap[2] == 5); REQUIRE(ap[3] == 7);
      }
      THEN("the adjacent points set should contains 4 elements") {
	       REQUIRE(ap.size() == 4);
      }
    }
    WHEN("compute adjacent points of (0,0)") {
      size_t pc = 0;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent points of { (1,0), (0,1), NoIndex, NoIndex }") {
      	REQUIRE(ap[0] == 1); REQUIRE(ap[1] == 3);
      	REQUIRE(ap[2] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[3] == Adjacency::NoAdjacentIndex);
      }
      THEN("the adjacent points set should contains 4 elements") {
	      REQUIRE(ap.size() == 4);
      }
    }
    WHEN("compute adjacent points of (2,2)") {
      size_t pc = 2*width+2;
      std::vector<int> ap = adj->neighbours(pc);
      THEN("the adjacent points should be { (2,1), (1,2), NoIndex, NoIndex }") {
      	REQUIRE(ap[0] == 7); REQUIRE(ap[1] == 5);
      	REQUIRE(ap[2] == Adjacency::NoAdjacentIndex);
      	REQUIRE(ap[3] == Adjacency::NoAdjacentIndex);
      }
      THEN("the adjacent points set should contains 4 elements") {
	      REQUIRE(ap.size() == 4);
      }
    }
  }
}
