package org.w3c.dom;

public interface LongArray {
  int getLength();
  void setLength(int length);
  int getElement(int index);
  void setElement(int index, int value);
}
