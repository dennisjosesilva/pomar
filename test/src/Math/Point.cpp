#include "../../catch.hpp"
#include <pomar/Math/Point.hpp>

using namespace pomar;

SCENARIO("2D points arithmetics should be correctly") {
  GIVEN("The points p = (3,4) and q =(-2,5)") {
    IPoint2D p(3, 4), q(-2, 5);

    WHEN("r = p == q is performed") {
      auto r = p == p;
      THEN("r should be true") {
	REQUIRE(r == true);
      }
    }
    WHEN("r = p == q is performed") {
      auto r = p == q;
      THEN("r should be false") {
	REQUIRE(r == false);
      }
    }
    WHEN("r = p + q is perfomed") {
      auto r = p + q;  
      THEN("r should be equal to (1, 9)") {
	REQUIRE(r == IPoint2D(1,9));
      }
    }
    WHEN("p += q is performed") {
      p += q;
      THEN("p should be equal to (1, 9)") {
	REQUIRE(p == IPoint2D(1,9));
      }
    }
    WHEN("r = p * 2 is performed") {
      auto r = p * 2;
      THEN("r should be equal to (6, 8)") {
	REQUIRE(r == IPoint2D(6,8));
      }
    }
    WHEN("p *= 2 is perfomed") {
      p *= 2;
      THEN("p should be equal to (6,8)") {
	REQUIRE(p == IPoint2D(6,8));
      }	
    }
    WHEN("r = p - q is perfomed") {
      auto r = p - q;
      THEN("r should be equal to (5,-1)") {
	REQUIRE(r == IPoint2D(5, -1));
      }
    }
    WHEN("p -= q is perfomed") {
      p -= q;
      THEN("p should be equal to (5,-1)") {
	REQUIRE(p == IPoint2D(5,-1));
      }
    }
    WHEN("r = p / 2 is performed") {
      auto r = p / 2;
      THEN("r should be equal to (1, 2)") {
	REQUIRE(r == IPoint2D(1,2));
      }
    }
    WHEN("p /= 2 is performed") {
       p /= 2;
      THEN("p should be equal to (1, 2)") {
	REQUIRE(p == IPoint2D(1,2));
      }
    }
  }
}
