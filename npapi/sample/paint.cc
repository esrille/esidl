/*
 * Copyright 2011 Esrille Inc.
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
#include <new>

#include <org/w3c/dom/Document.h>
#include <org/w3c/dom/events/MouseEvent.h>
#include <org/w3c/dom/html/CanvasRenderingContext2D.h>
#include <org/w3c/dom/html/HTMLCanvasElement.h>
#include <org/w3c/dom/views/ClientRect.h>

using namespace org::w3c::dom;

void PaintInstance::initialize()
{
    downHandler = new(std::nothrow) EventHandler(this, &PaintInstance::down);
    moveHandler = new(std::nothrow) EventHandler(this, &PaintInstance::move);
    upHandler = new(std::nothrow) EventHandler(this, &PaintInstance::up);
    selectHandler = new(std::nothrow) EventHandler(this, &PaintInstance::select);

    Document document = window.getDocument();
    if (!document)
        return;
    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(document.getElementById("canvas"));
    if (!canvas)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (context) {
        context.setFillStyle("white");
        context.fillRect(0, 0, 320, 240);
    }
    canvas.addEventListener("mousedown", downHandler, true);
    canvas.addEventListener("mouseup", upHandler, true);
    html::HTMLElement tools = interface_cast<html::HTMLElement>(document.getElementById("tools"));
    if (tools)
        tools.addEventListener("mousedown", selectHandler, true);
}

PaintInstance::~PaintInstance()
{
}

void PaintInstance::down(events::Event evt)
{
    events::MouseEvent mouse = interface_cast<events::MouseEvent>(evt);
    if (!mouse)
        return;
    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(mouse.getTarget());
    if (!canvas)
        return;
    views::ClientRect rect = canvas.getBoundingClientRect();
    if (!rect)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (context) {
        context.beginPath();
        float x = mouse.getClientX() - rect.getLeft();
        float y = mouse.getClientY() - rect.getTop();
        context.moveTo(x, y);
    }
    canvas.addEventListener("mousemove", moveHandler, true);
}

void PaintInstance::move(events::Event evt)
{
    events::MouseEvent mouse = interface_cast<events::MouseEvent>(evt);
    if (!mouse)
        return;
    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(mouse.getTarget());
    if (!canvas)
        return;
    views::ClientRect rect = canvas.getBoundingClientRect();
    if (!rect)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (context) {
        float x = mouse.getClientX() - rect.getLeft();
        float y = mouse.getClientY() - rect.getTop();
        context.lineTo(x, y);
        context.stroke();
    }
}

void PaintInstance::up(events::Event evt)
{
    events::MouseEvent mouse = interface_cast<events::MouseEvent>(evt);
    if (!mouse)
        return;
    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(mouse.getTarget());
    if (!canvas)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (context)
        context.closePath();
    canvas.removeEventListener("mousemove", moveHandler, true);
}

void PaintInstance::select(events::Event evt)
{
    events::MouseEvent mouse = interface_cast<events::MouseEvent>(evt);
    if (!mouse)
        return;
    Document document = window.getDocument();
    if (!document)
        return;
    html::HTMLElement tools = interface_cast<html::HTMLElement>(mouse.getTarget());
    if (!tools)
        return;
    views::ClientRect rect = tools.getBoundingClientRect();
    if (!rect)
        return;
    float y = mouse.getClientY() - rect.getTop();
    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(document.getElementById("canvas"));
    if (!canvas)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (context) {
        if (y < 24) {
            context.setStrokeStyle("black");
            context.setLineWidth(1);
            canvas.setAttribute("class", "pen");
        } else {
            context.setStrokeStyle("white");
            context.setLineWidth(10);
            canvas.setAttribute("class", "eraser");
        }
    }
}
