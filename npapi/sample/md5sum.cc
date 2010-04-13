/*
 * Copyright 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "md5sum.h"
#include <new>

using namespace org::w3c::dom;

void MD5SumInstance::initialize() {
  md5class = new (std::nothrow) MD5Class(this);
  if (md5class) {
    window->setElement("MD5", md5class);
  }
}

MD5SumInstance::~MD5SumInstance() {
  if (md5class) {
      md5class->release();
  }
}
