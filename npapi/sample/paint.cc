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

#include "paint.h"

using namespace org::w3c::dom;

void PluginInstance::initialize() {
  downHandler = new EventHandler(this, &PluginInstance::down);
  moveHandler = new EventHandler(this, &PluginInstance::move);
  upHandler = new EventHandler(this, &PluginInstance::up);
  selectHandler = new EventHandler(this, &PluginInstance::select);

  Document* document = window->getDocument();
  if (document) {
    html::HTMLCanvasElement* canvas =
      interface_cast<html::HTMLCanvasElement*>(
        document->getElementById("canvas"));
    if (canvas) {
      html::CanvasRenderingContext2D* context =
        interface_cast<html::CanvasRenderingContext2D*>(
          canvas->getContext("2d"));
      if (context) {
        context->setFillStyle("white");
        context->fillRect(0, 0, 320, 240);
        context->release();
      }
      events::EventTarget* eventTarget =
        interface_cast<events::EventTarget*>(canvas);
      if (eventTarget) {
        eventTarget->addEventListener("mousedown", downHandler, true);
        eventTarget->addEventListener("mousemove", moveHandler, true);
        eventTarget->addEventListener("mouseup", upHandler, true);
      }
      canvas->release();
    }
    html::HTMLElement* tools =
      interface_cast<html::HTMLElement*>(document->getElementById("tools"));
    if (tools) {
      events::EventTarget* eventTarget =
        interface_cast<events::EventTarget*>(tools);
      if (eventTarget) {
        eventTarget->addEventListener("mousedown", selectHandler, true);
      }
      tools->release();
    }
    document->release();
  }
}

PluginInstance::~PluginInstance() {
  delete downHandler;
  delete moveHandler;
  delete upHandler;
  if (selectHandler) {
    delete selectHandler;
  }
  if (window) {
    window->release();
  }
}

void PluginInstance::down(events::Event* evt) {
  events::MouseEvent* mouse = interface_cast<events::MouseEvent*>(evt);
  printf("down %p\n", mouse);
  if (!mouse) {
    return;
  }
  html::HTMLCanvasElement* canvas =
    interface_cast<html::HTMLCanvasElement*>(mouse->getTarget());
  if (canvas) {
    html::CanvasRenderingContext2D* context =
      interface_cast<html::CanvasRenderingContext2D*>(canvas->getContext("2d"));
    if (context) {
      context->beginPath();
      int x = mouse->getClientX() - canvas->getOffsetLeft();
      int y = mouse->getClientY() - canvas->getOffsetTop();
      context->moveTo(x, y);
      context->release();
    }
    canvas->release();

    penDown = true;
  }
}

void PluginInstance::move(events::Event* evt) {
  events::MouseEvent* mouse = interface_cast<events::MouseEvent*>(evt);
  printf("move %p\n", mouse);
  if (!mouse) {
    return;
  }
  if (!penDown) {
    return;
  }
  html::HTMLCanvasElement* canvas =
    interface_cast<html::HTMLCanvasElement*>(mouse->getTarget());
  if (canvas) {
    html::CanvasRenderingContext2D* context =
      interface_cast<html::CanvasRenderingContext2D*>(canvas->getContext("2d"));
    if (context) {
      int x = mouse->getClientX() - canvas->getOffsetLeft();
      int y = mouse->getClientY() - canvas->getOffsetTop();
      context->lineTo(x, y);
      context->stroke();
      context->release();
    }
    canvas->release();
  }
}

void PluginInstance::up(events::Event* evt) {
  events::MouseEvent* mouse = interface_cast<events::MouseEvent*>(evt);
  printf("up %p\n", mouse);
  if (!mouse) {
    return;
  }
  if (!penDown) {
    return;
  }
  html::HTMLCanvasElement* canvas =
    interface_cast<html::HTMLCanvasElement*>(mouse->getTarget());
  if (canvas) {
    html::CanvasRenderingContext2D* context =
      interface_cast<html::CanvasRenderingContext2D*>(canvas->getContext("2d"));
    if (context) {
      context->closePath();
      context->release();
    }
    canvas->release();

    penDown = false;
  }
}

void PluginInstance::select(events::Event* evt) {
  events::MouseEvent* mouse = interface_cast<events::MouseEvent*>(evt);
  printf("select %p\n", mouse);
  if (!mouse) {
    return;
  }
  Document* document = window->getDocument();
  if (document) {
    html::HTMLCanvasElement* canvas =
      interface_cast<html::HTMLCanvasElement*>(
        document->getElementById("canvas"));
    if (canvas) {
      int y = mouse->getClientY() - canvas->getOffsetTop();
      html::CanvasRenderingContext2D* context =
        interface_cast<html::CanvasRenderingContext2D*>(
          canvas->getContext("2d"));
      if (context) {
        if (y < 24) {
          context->setStrokeStyle("black");
          context->setLineWidth(1);
          canvas->setAttribute("class", "pen");
        } else {
          context->setStrokeStyle("white");
          context->setLineWidth(10);
          canvas->setAttribute("class", "eraser");
        }
        context->release();
      }
      canvas->release();
    }
    document->release();
  }
}
