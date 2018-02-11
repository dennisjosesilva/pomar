#include "../../catch.hpp"
#include <pomar/ComponentTree/CTSorter.hpp>

using namespace pomar;

SCENARIO("maxtree sorter should sort increasing order.") {
  GIVEN("An unsigned char vector with values (2,5,3,0,2,3,0,3).") {
    std::vector<unsigned char> v = {2,5,3,0,2,3,0,3};
    WHEN("maxtreeSort is called.") {
      auto idx = maxTreeSort(v);
      THEN("The result should be (3,6,0,4,2,5,7,1).") {
        REQUIRE(idx == std::vector<int>({3,6,0,4,2,5,7,1}));
      }
    }
  }
  GIVEN("An unsigned long vector with values (2,5,3,0,2,3,0,3).") {
    std::vector<unsigned long> v = {2,5,3,0,2,3,0,3};
    WHEN("maxTreeSort is called.") {
      auto idx = maxTreeSort(v);
      THEN("The result should be (3,6,0,4,2,5,7,1).") {
        REQUIRE(idx == std::vector<int>({3,6,0,4,2,5,7,1}));
      }
    }
  }
}

SCENARIO("mintree sorter should sort in decreasing order.") {
  GIVEN("An unsigned char vector with values (2,5,3,0,2,3,0,3).") {
    std::vector<unsigned char> v = {2,5,3,0,2,3,0,3};
    WHEN("mintreeSort is called.") {
      auto idx = minTreeSort(v);
      THEN("The result should be (1,2,5,7,0,4,3,6).") {
        REQUIRE(idx == std::vector<int>({1,2,5,7,0,4,3,6}));
      }
    }
  }
  GIVEN("An unsigned long vector with values (2,5,3,0,2,3,0,3).") {
    std::vector<unsigned long> v = {2,5,3,0,2,3,0,3};
    WHEN("minTreeSort is called.") {
      auto idx = minTreeSort(v);
      THEN("The result should be (1,2,5,7,0,4,3,6).") {
        REQUIRE(idx == std::vector<int>({1,2,5,7,0,4,3,6}));
      }
    }
  }
}