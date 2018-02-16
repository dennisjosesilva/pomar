#include "../../catch.hpp"
#include <pomar/Attribute/BasicAttributeComputer.hpp>
#include <numeric>
#include <algorithm>

using namespace pomar;

SCENARIO("Area attribute computer should compute area for all node of a component tree.") {
  GIVEN("A component tree 'ct' area attribute computers.") {
    std::vector<unsigned char> elements = { 2,0,3,2,1,3,7,0,3 };
    std::vector<int> parent = { 4,1,4,0,1,2,0,1,2 };
    std::vector<int> sortedIndices(elements.size());
    std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&elements](int i1, int i2) { 
      return elements[i1] < elements[i2]; });

    CTree<unsigned char> ct(parent, sortedIndices, elements);
    AreaAttributeComputer<unsigned char> attrArea;
    WHEN("The area computer computes.") {      
      auto attrs = attrArea.compute(ct);
      THEN("attrs should have AREA as fisrt possition.") {
        REQUIRE(attrs.attrIndex(AttrType::AREA) == 0);
      }
      THEN("the values should have the values (9, 7, 2, 3, 3, 1") {
        auto area = attrs[attrs.attrIndex(AttrType::AREA)];
        REQUIRE(area == std::vector<double>{9.0, 7.0, 3.0, 3.0, 1.0});
      }
    }
  }
}