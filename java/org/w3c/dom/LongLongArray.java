package org.w3c.dom;

public interface LongLongArray {
  int getLength();
  void setLength(int length);
  long getElement(int index);
  void setElement(int index, long value);
}
