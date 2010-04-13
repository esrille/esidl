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

#ifndef SAMPLE_MD5SUM_H_
#define SAMPLE_MD5SUM_H_

#include <esnpapi.h>

#include <org/w3c/dom.h>
#include <com/rsa/MD5.h>

class MD5Class;

class MD5SumInstance : public PluginInstance {
 public:
  MD5SumInstance(NPP npp, NPObject* window)
      : PluginInstance(npp, window) {
    initialize();
  }
  ~MD5SumInstance();

 private:
  void initialize();

  MD5Class* md5class;
};

class MD5Object : public com::rsa::MD5 {
 public:
  explicit MD5Object(MD5SumInstance* instance)
      : instance(instance) {
  }
  virtual void update(std::string input) {
  }
  virtual std::string final() {
    return "900150983cd24fb0d6963f7d28e17f72";  // dummy
  }
  unsigned int retain() {
    return instance->retain(this);
  };
  unsigned int release() {
    return instance->release(this);
  };

 private:
  MD5SumInstance* instance;
};

class MD5Class : public com::rsa::MD5_Constructor {
 public:
  explicit MD5Class(MD5SumInstance* instance)
      : instance(instance) {
  }
  virtual com::rsa::MD5* createInstance() {
    return new (std::nothrow) MD5Object(instance);
  }
  unsigned int retain() {
    return instance->retain(this);
  };
  unsigned int release() {
    return instance->release(this);
  };

 private:
  MD5SumInstance* instance;
};

#endif  // SAMPLE_MD5SUM_H_
