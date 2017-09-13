#include "../../catch.hpp"
#include <pomar/Math/Point.hpp>

using namespace pomar;

SCENARIO("two 2D Points should added up correctly") {
  GIVEN("The points p = (3,4) and q =(-2,5)") {
    IPoint2D p(3, 4), q(-2, 5);

    WHEN("we add p with q and store it in r") {
      auto r = p + q;
      
      THEN("r should be equal to (1, 9)") {
	REQUIRE(r.x() == 1);
	REQUIRE(r.y() == 9);
      }
    }
    WHEN("p += q is performed") {
      p += q;
      THEN("p should be equal to (1, 9)") {
	REQUIRE(p.x() == 1);
	REQUIRE(p.y() == 9);
      }
    }
  }
}
