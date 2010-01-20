package org.w3c.dom;

public interface OctetArray {
  int getLength();
  void setLength(int length);
  byte getElement(int index);
  void setElement(int index, byte value);
}
