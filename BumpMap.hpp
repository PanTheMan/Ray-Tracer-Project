#pragma once
#include <string>
#include "Image.hpp"
#include "Intersection.hpp"
#include <iostream>

class BumpMap {
public:
  ~BumpMap();
  BumpMap(const std::string & filename);
  Image *bumpMap;
  bool hasBumpMap;
  void applyBumpMap(Intersection &intersect);
};