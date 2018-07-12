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

SCENARIO("AttributeComputerQuads should compute area, perimeter (also continuous approx.) and Euler Number (max-tree)") {
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

    std::vector<std::shared_ptr<AttributeFromQuadsComputer>> comps = {std::make_shared<QArea>(), 
      std::make_shared<QCArea>(), std::make_shared<QPerimeter>(), std::make_shared<QCPerimeter>(), 
      std::make_shared<QEulerNumber>() };

    AttributeComputerQuads<unsigned char> attrComputer{ QTreeType::MaxTree, QConnectivity::Eight,
      "../../resource/pomar", comps };
    
    WHEN("call the compute method from the AttributeComputerQuads") {
      auto attrs = attrComputer.compute(ct);

      THEN("The area should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_AREA);
        REQUIRE(attrs[attrIdx][0] == 36); REQUIRE(attrs[attrIdx][1] == 20); REQUIRE(attrs[attrIdx][2] == 14);
        REQUIRE(attrs[attrIdx][3] == 1); REQUIRE(attrs[attrIdx][4] == 10);
      }
      THEN("The perimeter should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_PERIMETER);
        REQUIRE(attrs[attrIdx][0] == 24); REQUIRE(attrs[attrIdx][1] == 18); REQUIRE(attrs[attrIdx][2] == 20);
        REQUIRE(attrs[attrIdx][3] == 4); REQUIRE(attrs[attrIdx][4] == 20);
      }
      THEN("The Euler number should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_EULER_NUMBER);
        REQUIRE(attrs[attrIdx][0] == 1.0); REQUIRE(attrs[attrIdx][1] == 1.0); REQUIRE(attrs[attrIdx][2] == 1.0);
        REQUIRE(attrs[attrIdx][3] == 1.0); REQUIRE(attrs[attrIdx][4] == 0.0);
      }
      THEN("The continuos area should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_CONTINUOS_AREA);
        REQUIRE(attrs[attrIdx][0] == 30.5); REQUIRE(attrs[attrIdx][1] == 16.0); REQUIRE(attrs[attrIdx][2] == 11.0);
        REQUIRE(attrs[attrIdx][3] == 0.5); REQUIRE(attrs[attrIdx][4] == 7.0);
      }      
      THEN("The continuos perimeter should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_CONTINUOS_PERIMETER);
        REQUIRE(attrs[attrIdx][0] == Approx(22.8368794326)); REQUIRE(attrs[attrIdx][1] == Approx(16.8368794326)); 
        REQUIRE(attrs[attrIdx][2] == Approx(15.0921985815)); REQUIRE(attrs[attrIdx][3] == Approx(2.83687943262)); 
        REQUIRE(attrs[attrIdx][4] == Approx(17.6737588652));
      }
    }
  }
}

SCENARIO("AttributeComputerQuads should compute area, perimeter (also continuous approx.) and Euler Number for (min-tree)") {
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

    std::vector<std::shared_ptr<AttributeFromQuadsComputer>> comps = {std::make_shared<QArea>(), 
      std::make_shared<QCArea>(), std::make_shared<QPerimeter>(), std::make_shared<QCPerimeter>(), 
      std::make_shared<QEulerNumber>() };

    AttributeComputerQuads<unsigned char> attrComputer{ QTreeType::MinTree, QConnectivity::Eight,
      "../../resource/pomar", comps };

    WHEN("call the compute method from the AttributeComputerQuads") {
      auto attrs = attrComputer.compute(ct);

      THEN("The area should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_AREA);
        REQUIRE(attrs[attrIdx][0] == 36); REQUIRE(attrs[attrIdx][1] == 24); REQUIRE(attrs[attrIdx][2] == 2);
        REQUIRE(attrs[attrIdx][3] == 21); REQUIRE(attrs[attrIdx][4] == 16);
      }
      THEN("The perimeter should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_PERIMETER);
        REQUIRE(attrs[attrIdx][0] == 24); REQUIRE(attrs[attrIdx][1] == 30); REQUIRE(attrs[attrIdx][2] == 6);
        REQUIRE(attrs[attrIdx][3] == 40); REQUIRE(attrs[attrIdx][4] == 34);
      }
      THEN("The euler number should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_EULER_NUMBER);
        REQUIRE(attrs[attrIdx][0] == 1.0); REQUIRE(attrs[attrIdx][1] == 1.0); REQUIRE(attrs[attrIdx][2] == 1.0);
        REQUIRE(attrs[attrIdx][3] == -1.0); REQUIRE(attrs[attrIdx][4] == 1.0);
      }
      THEN("The continouos area should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_CONTINUOS_AREA);
        REQUIRE(attrs[attrIdx][0] == 30.5); REQUIRE(attrs[attrIdx][1] == 18.0);
        REQUIRE(attrs[attrIdx][2] == 1.0); REQUIRE(attrs[attrIdx][3] == 16.0); REQUIRE(attrs[attrIdx][4] == 9.0);
      }
      THEN("The continouos perimeter should be computed right") {
        auto attrIdx = attrs.attrIndex(AttrType::QUADS_CONTINUOS_PERIMETER);
        REQUIRE(attrs[attrIdx][0] == Approx(22.83687943262));
        REQUIRE(attrs[attrIdx][1] == Approx(27.67375886524)); REQUIRE(attrs[attrIdx][2] == Approx(4.83687943262)); 
        REQUIRE(attrs[attrIdx][3] == Approx(32.76595744680)); REQUIRE(attrs[attrIdx][4] == Approx(31.67375886524));
      }
    }
  }
}