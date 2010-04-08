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

#ifndef SAMPLE_XHR_H_
#define SAMPLE_XHR_H_

#include <esnpapi.h>

#include <org/w3c/dom.h>

class EventHandler;

class XHRInstance : public PluginInstance {
 public:
  XHRInstance(NPP npp, NPObject* window)
      : PluginInstance(npp, window) {
    initialize();
  }
  ~XHRInstance();

 private:
  EventHandler* loadHandler;
  EventHandler* displayHandler;

  void initialize();
  void load(org::w3c::dom::events::Event* evt);
  void display(org::w3c::dom::events::Event* evt);
};

class EventHandler : public org::w3c::dom::events::EventListener {
 public:
  EventHandler(XHRInstance* instance,
               void (XHRInstance::*handler)(org::w3c::dom::events::Event*))
      : instance(instance),
        handler(handler) {
  }
  virtual void handleEvent(org::w3c::dom::events::Event* evt) {
    (instance->*handler)(evt);
  }
  unsigned int retain() {
    return 1;
  };
  unsigned int release() {
    return 1;
  };

 private:
  XHRInstance* instance;
  void (XHRInstance::*handler)(org::w3c::dom::events::Event* evt);
};

#endif  // SAMPLE_XHR_H_
