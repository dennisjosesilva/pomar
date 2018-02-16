#include "../../catch.hpp"
#include <pomar/Attribute/AttributeCollection.hpp>

using namespace pomar;

SCENARIO("AttributeCollection stores a collection of attributes for a component tree node.") {
  GIVEN("An AttributeCollection instance with 2 attributes and 10 component tree nodes.") {
    AttributeCollection attrs;
    attrs.push(AttrType::AREA, 10);
    attrs.push(AttrType::PERIMETER, 10);
    WHEN("Then AttrType is called.") {
      THEN("It should return 0 for AREA and 1 for PERIMETER.") {
        REQUIRE(attrs.attrIndex(AttrType::AREA) == 0); 
        REQUIRE(attrs.attrIndex(AttrType::PERIMETER) == 1);
      }
    }
    WHEN("The clear function is called and add perimeter attribute again.") {
      attrs.clear();
      attrs.push(AttrType::PERIMETER, 10);
      THEN("Then attrs should return 0 for PERIMETER.") {
        REQUIRE(attrs.attrIndex(AttrType::PERIMETER) == 0);
      }
    }
    WHEN("Set value 42.0 at node id 5 for attribute Area using operator []") {
      attrs[attrs.attrIndex(AttrType::AREA)][5] = 42.0;
      THEN("It should return 42.0 after access id 25 of attribute area using operator []") {
        REQUIRE(attrs[attrs.attrIndex(AttrType::AREA)][5] == 42.0);
      }
    }
    WHEN("set 42.0 at node id 5 for attribute PERIMETER using function set.") {
      attrs.set(attrs.attrIndex(AttrType::PERIMETER), 5, 42.0);
      THEN("get 42.0 at node id 5 for attribute PERIMETER using function get.") {
        REQUIRE(attrs.get(attrs.attrIndex(AttrType::PERIMETER), 5) == 42.0);
      }
    }
  }
}