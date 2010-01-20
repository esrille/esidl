package org.w3c.dom;

public interface DoubleArray {
  int getLength();
  void setLength(int length);
  double getElement(int index);
  void setElement(int index, double value);
}
