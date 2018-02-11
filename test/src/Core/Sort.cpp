#include "../../catch.hpp"
#include <pomar/Core/Sort.hpp>

using namespace pomar;

SCENARIO("Function isLowSizeType should indicates whether a type has low size or not.") {
  GIVEN("The unsigned char, char, unsigned short, short, unsigned int, int or bool.") {
    WHEN("isLowSizeType is called using one of these types.") {
      THEN("It should return true.") {
        REQUIRE(isLowSizeType<unsigned char>()); REQUIRE(isLowSizeType<char>());
        REQUIRE(isLowSizeType<unsigned short>()); REQUIRE(isLowSizeType<short>());
        REQUIRE(isLowSizeType<unsigned int>()); REQUIRE(isLowSizeType<int>());
        REQUIRE(isLowSizeType<bool>());
      }
    }
  }
  GIVEN("Types long, unsigned long, float, double.") {
    WHEN("isLowSizeType is called using one of these types.") {
      THEN("it should return false.") {
        REQUIRE(!isLowSizeType<long>()); REQUIRE(!isLowSizeType<unsigned long>());
        REQUIRE(!isLowSizeType<float>()); REQUIRE(!isLowSizeType<double>());
      }
    }
  }
}

SCENARIO("Function STLsortIndex should return a vector indexes ordering by a function cmp.") {
  GIVEN("An long vector (2,5,3,0,2,3,0,3).") {
    std::vector<long> v = {2,5,3,0,2,3,0,3};
    WHEN("STLsortIndex is called using v and a descresing function v1 < v2.") {
      auto idx = STLsortIndex<long>(v, [](const long &v1, const long &v2) { return v1 < v2; });
      THEN("It should return (3,6,0,4,2,5,7,1).") {
        REQUIRE(idx == std::vector<int>({3,6,0,4,2,5,7,1}));
      }
    }
    WHEN("STLsortIndex is called using v and an incresing function v1 > v2.") {
      auto idx = STLsortIndex<long>(v, [](const long &v1, const long &v2){ return v1 > v2; });
      THEN("It should return be (1,2,5,7,0,4,3,6).") {
        REQUIRE(idx == std::vector<int>({1,2,5,7,0,4,3,6}));
      }
    }
  }
}

SCENARIO("Counting sort should sort indexing.") {
  GIVEN("An unsigned char vector (2,5,3,0,2,3,0,3).") {
    std::vector<unsigned char> v = {2,5,3,0,2,3,0,3};
    WHEN ("increasingCountingSort is called using v.") {
      auto idx = incresingCountingSortIndex(v);
      THEN ("It should return (3,6,0,4,2,5,7,1).") {
        REQUIRE(idx == std::vector<int>({3,6,0,4,2,5,7,1}));
      }
    }
    WHEN("decreasingCountingSort is called using v.") {
      auto idx = decreasingCountingSortIndex(v);
      THEN("It should return (1,2,5,7,0,4,3,6).") {
        REQUIRE(idx == std::vector<int>({1,2,5,7,0,4,3,6}));
      }
    }
  }
}