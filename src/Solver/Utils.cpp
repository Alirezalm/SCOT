//
// Created by alireza on 12/06/22.
//

#include "Utils.h"
namespace Scot::Utils{

double dot(VectorDouble x, VectorDouble y) {
  assert(x.size() == y.size());
  double dot = 0.0;

  for (int i = 0; i < x.size(); ++i) {
    dot += x[i] * y[i];
  }
  return dot;
}
}