package org.w3c.dom;

public interface FloatArray {
  int getLength();
  void setLength(int length);
  float getElement(int index);
  void setElement(int index, float value);
}
