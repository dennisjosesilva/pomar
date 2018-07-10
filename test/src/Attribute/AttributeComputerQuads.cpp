#include "../../catch.hpp"
#include <pomar/AdjacencyRelation/AdjacencyByTranslating.hpp>
#include <pomar/Attribute/AttributeComputerQuads.hpp>
#include <pomar/ComponentTree/CTBuilder.hpp>
#include <numeric>
#include <algorithm>
#include <memory>

using namespace pomar;

SCENARIO("Quads attribute computer should count bit-quads for all nodes of a maxtree with 8-connectivity.") {
  GIVEN("A max-tree 'ct' built from a image 'f'.") {
    std::vector<unsigned char> f = {
      0,0,0,0,0,0,
      0,2,1,3,3,3,
      0,1,2,3,2,3,
      0,1,1,3,2,3,
      0,2,1,3,3,3,
      0,0,0,0,0,0
    };
    int width = 6, height = 6;
    CTBuilder builder;
    auto adj = AdjacencyByTranslating2D::createAdjacency8(width, height);
    auto ct = builder.build(std::make_shared<CTMetaImage2D>(width,height,1), f, std::move(adj), 
      CTBuilder::TreeType::MaxTree);
    AttributeComputerQuads<unsigned char> attrComputer{
      QTreeType::MaxTree,
      QConnectivity::Eight,
      "../../resource/pomar"};
    
    WHEN("The maxtree computed method is called.") {
      auto attr = attrComputer.compute(ct);
      
      THEN("The quantity of Q1 quads for each maxtree node should be computed") {
        auto attr = attrComputer.compute(ct);
        auto q1 = attr.attrIndex(AttrType::QUADS_Q1);
        REQUIRE(attr[q1][0] == 4); REQUIRE(attr[q1][1] == 4); REQUIRE(attr[q1][2] == 8);
        REQUIRE(attr[q1][3] == 4); REQUIRE(attr[q1][4] == 4);
      }
      THEN("The quantity of Q2 quads for each maxtree node should be computed") {
        auto q2 = attr.attrIndex(AttrType::QUADS_Q2);
        REQUIRE(attr[q2][0] == 20); REQUIRE(attr[q2][1] == 14); REQUIRE(attr[q2][2] == 8);
        REQUIRE(attr[q2][3] == 0); REQUIRE(attr[q2][4] == 12);
      }
      THEN("The quantity of Q3 quads for each maxtree node should be computed") {
        auto q3 = attr.attrIndex(AttrType::QUADS_Q3);
        REQUIRE(attr[q3][0] == 0); REQUIRE(attr[q3][1] == 0); REQUIRE(attr[q3][2] == 2);
        REQUIRE(attr[q3][3] == 0); REQUIRE(attr[q3][4] == 4);
      }
      THEN("The quantity of Q4 quads for each maxtree node should be computed") {
        auto q4 = attr.attrIndex(AttrType::QUADS_Q4);
        REQUIRE(attr[q4][0] == 25); REQUIRE(attr[q4][1] == 12); REQUIRE(attr[q4][2] == 6);
        REQUIRE(attr[q4][3] == 0); REQUIRE(attr[q4][4] == 0);
      }
      THEN("The quantity of QD quads for each maxtree node should be computed") {
        auto qd = attr.attrIndex(AttrType::QUADS_QD);
        REQUIRE(attr[qd][0] == 0); REQUIRE(attr[qd][1] == 0); REQUIRE(attr[qd][2] == 1);
        REQUIRE(attr[qd][3] == 0); REQUIRE(attr[qd][4] == 0);
      }
    }
  }
}
SCENARIO("Quads attribute computer should count bit-quads for all nodes of a min-tree with 8-connectivity.") {
  GIVEN("A min-tree 'ct' built from a image 'f'.") {
    std::vector<unsigned char> f = {
      0,0,0,0,0,0,
      0,2,1,3,3,3,
      0,1,2,3,2,3,
      0,1,1,3,2,3,
      0,2,1,3,3,3,
      0,0,0,0,0,0
    };
    int width = 6, height = 6;
    CTBuilder builder;
    auto adj = AdjacencyByTranslating2D::createAdjacency8(width, height);
    auto ct = builder.build(std::make_shared<CTMetaImage2D>(width,height,1), f, std::move(adj), 
      CTBuilder::TreeType::MinTree);
    AttributeComputerQuads<unsigned char> attrComputer{
      QTreeType::MinTree,
      QConnectivity::Eight,
      "../../resource/pomar"};
    
    WHEN("The min-tree computed method is called.") {
      auto attr = attrComputer.compute(ct);
      
      THEN("The quantity of Q1 quads for each min-tree node should be computed") {
        auto q1 = attr.attrIndex(AttrType::QUADS_Q1);
        REQUIRE(attr[q1][4] == 6); REQUIRE(attr[q1][3] == 8); REQUIRE(attr[q1][2] == 4); 
        REQUIRE(attr[q1][1] == 6); REQUIRE(attr[q1][0] == 4); 
      }
      THEN("The quantity of Q2 quads for each min-tree node should be computed") {
        auto q2 = attr.attrIndex(AttrType::QUADS_Q2);
        REQUIRE(attr[q2][4] == 26); REQUIRE(attr[q2][3] == 20); REQUIRE(attr[q2][2] == 2); 
        REQUIRE(attr[q2][1] == 22); REQUIRE(attr[q2][0] == 20); 
      }
      THEN("The quantity of Q3 quads for each min-tree node should be computed") {
        auto q3 = attr.attrIndex(AttrType::QUADS_Q3);
        REQUIRE(attr[q3][4] == 2); REQUIRE(attr[q3][3] == 10); REQUIRE(attr[q3][2] == 0);
        REQUIRE(attr[q3][1] == 2); REQUIRE(attr[q3][0] == 0); 
      }
      THEN("The quantity of Q4 quads for each min-tree node should be computed") {
        auto q4 = attr.attrIndex(AttrType::QUADS_Q4);
        REQUIRE(attr[q4][4] == 0); REQUIRE(attr[q4][3] == 1); REQUIRE(attr[q4][2] == 0);
        REQUIRE(attr[q4][1] == 10); REQUIRE(attr[q4][0] == 25); 
      }
      THEN("The quantity of QD quads for each min-tree node should be computed") {
        auto qd = attr.attrIndex(AttrType::QUADS_QD);
        REQUIRE(attr[qd][4] == 0); REQUIRE(attr[qd][3] == 1); REQUIRE(attr[qd][2] == 0);
        REQUIRE(attr[qd][1] == 0); REQUIRE(attr[qd][0] == 0); 
      }
    }
  }
}