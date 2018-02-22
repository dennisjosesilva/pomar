#include "../../catch.hpp"
#include <pomar/Core/PixelIndexer.hpp>
#include <memory>

using namespace pomar;

SCENARIO("Each sub-class of Pixel Indexer should compute a pixel index correctly") {
  GIVEN("A PixelIndexerNearestBorder instance") {    
    std::unique_ptr<PixelIndexer> pixelIndexer{new PixelIndexerNearestBorder{3,3}};
    WHEN("index method is called with values 1,1") {
      auto r = pixelIndexer->index(1,1);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with values 5,5") {
      auto r = pixelIndexer->index(5,5);
      THEN("It should return 8") { REQUIRE(r == 8); }
    }
    WHEN("index method is called with value 4") {
      auto r = pixelIndexer->index(4);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 11") {
      auto r = pixelIndexer->index(11);
      THEN("It should return 8") { REQUIRE(r == 8); }
    }
    WHEN("index method is called with point 1,1") {
      auto r = pixelIndexer->index(IPoint2D(1,1));
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with point 5,5") {
      auto r = pixelIndexer->index(IPoint2D(5,5));
      THEN("It should return 8") { REQUIRE (r == 8); }
    }
    WHEN("pixel method is called with value 4") {
      auto r = pixelIndexer->pixel(4);
      THEN("It should return the point 1,1") { REQUIRE(r == IPoint2D(1,1)); }
    }
    WHEN("pixel method is called with value 11") {
      auto r = pixelIndexer->pixel(11);
      THEN("It should return the point 2,2") { REQUIRE(r == IPoint2D(2,2)); }
    }
  }
  GIVEN("A PixelDefaultValue with defaultvalue=-1.") {
    std::unique_ptr<PixelIndexer> pixelIndexer{new PixelIndexerDefaultValue{3,3}};
    WHEN("index method is called with value 4") {
      auto r = pixelIndexer->index(1,1);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 5,5") {
      auto r = pixelIndexer->index(5,5);
      THEN("It should return -1") { REQUIRE(r == -1); }
    }
    WHEN("index method is called with value 4") {
      auto r = pixelIndexer->index(4);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value -1") {
      auto r = pixelIndexer->index(11);
      THEN("It should return 8") { REQUIRE(r == -1); }
    }
    WHEN("index method is called with point 1,1") {
      auto r = pixelIndexer->index(IPoint2D(1,1));
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("pixel method is called with values 4") {
      auto r = pixelIndexer->pixel(4);
      THEN("It should return point 1,1") { REQUIRE(r == IPoint2D(1,1)); }
    }
    WHEN("pixel method is called with values 11") {
      auto r = pixelIndexer->pixel(11);
      THEN("It should return -1,-1") { REQUIRE(r == IPoint2D(-1,-1)); }
    }
  }
  GIVEN("A PixelIndexerMirrorImage error") {
    std::unique_ptr<PixelIndexer> pixelIndexer(new PixelIndexerMirrorImage(3,3));
    WHEN("index method is called with value 1,1") {
      auto r = pixelIndexer->index(1,1);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 5,5") {
      auto r = pixelIndexer->index(5,5);
      THEN("It should return 8") { REQUIRE(r == 8); }
    }
    WHEN("index method is called with value 4") {
      auto r = pixelIndexer->index(4);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 11") {
      auto r = pixelIndexer->index(11);
      THEN("It should return 2") { REQUIRE(r == 2); }
    }
    WHEN("pixel method is called with value 4") {
      auto r = pixelIndexer->pixel(4);
      THEN("It should return 1,1") { REQUIRE(r == IPoint2D(1,1)); }
    }
    WHEN("pixel method is called with value 11") {
      auto r = pixelIndexer->pixel(11);
      THEN("It should return 2,0") { REQUIRE(r == IPoint2D(2,0)); }
    }
  }
  GIVEN("A PixelIndexerException instance") {
    std::unique_ptr<PixelIndexer> pixelIndexer(new PixelIndexerException{3,3});
    WHEN("index method is called with value 1,1") {
      auto r = pixelIndexer->index(1, 1);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 5,5") {      
      THEN("It should throw an out of bounds pixel indexer exception") {
        REQUIRE_THROWS(pixelIndexer->index(5,5));
      }
    }
    WHEN("index method is called with value 4") {
      auto r = pixelIndexer->index(4);
      THEN("It should return 4") { REQUIRE(r == 4); }
    }
    WHEN("index method is called with value 11") {
      THEN("It should throw an ount of bounds pixel indexer exception") {
        REQUIRE_THROWS(pixelIndexer->index(11));
      }
    }
    WHEN("pixel method is called with value 4") {
      auto r = pixelIndexer->pixel(4);
      THEN("It should return 1,1") {
        REQUIRE(r == IPoint2D(1,1));
      }
    }
    WHEN("pixel method is called with value 11") {
      THEN("It should throw an out of bounds pixel indexer exception") {
        REQUIRE_THROWS(pixelIndexer->pixel(11));
      }
    }
  }
}