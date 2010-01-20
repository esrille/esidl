package org.w3c.dom;

public interface ShortArray {
  int getLength();
  void setLength(int length);
  short getElement(int index);
  void setElement(int index, short value);
}
