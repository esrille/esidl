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

#include "xhr.h"

using namespace org::w3c::dom;

void PluginInstance::initialize() {
  loadHandler = new EventHandler(this, &PluginInstance::load);
  displayHandler = new EventHandler(this, &PluginInstance::display);

  html::HTMLDocument* document = interface_cast<html::HTMLDocument*>(window->getDocument());
  if (document) {
    html::HTMLFormElement* form = interface_cast<html::HTMLFormElement*>(document->getElement("form"));
    if (form) {
      html::HTMLButtonElement* button = interface_cast<html::HTMLButtonElement*>(form->namedItem("button"));
      if (button) {
        events::EventTarget* eventTarget = interface_cast<events::EventTarget*>(button);
        if (eventTarget) {
          eventTarget->addEventListener("click", loadHandler, true);
        }
        button->release();
      }
      form->release();
    }
    document->release();
  }
}

PluginInstance::~PluginInstance() {
  if (loadHandler) {
    delete loadHandler;
  }
  if (displayHandler) {
    delete displayHandler;
  }
  if (window) {
    window->release();
  }
}

void PluginInstance::load(events::Event* evt) {
  XMLHttpRequest_Constructor* xmlHttpRequest = interface_cast<XMLHttpRequest_Constructor*>(window->getElement("XMLHttpRequest"));
  if (xmlHttpRequest) {
    XMLHttpRequest* xhr = xmlHttpRequest->createInstance();
    if (xhr) {
      xhr->addEventListener("load", displayHandler, true);
      xhr->open("GET", "xhr.txt", true);
      xhr->send();
      xhr->release();
    }
    xmlHttpRequest->release();
  }
}

void PluginInstance::display(events::Event* evt) {
  XMLHttpRequest* xhr = interface_cast<XMLHttpRequest*>(evt->getTarget());
  if (xhr) {
    html::HTMLDocument* document = interface_cast<html::HTMLDocument*>(window->getDocument());
    if (document) {
      html::HTMLFormElement* form = interface_cast<html::HTMLFormElement*>(document->getElement("form"));
      if (form) {
        html::HTMLInputElement* result = interface_cast<html::HTMLInputElement*>(form->namedItem("result"));
        if (result) {
            result->setValue(xhr->getResponseText());
            result->release();
        }
        form->release();
      }
      document->release();
    }
    xhr->release();
  }
}
