/*
 * Copyright 2011 Esrille Inc.
 * Copyright 2008-2010 Google Inc.
 * Copyright 2006, 2007 Nintendo Co., Ltd.
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

#include "test.h"

#include <org/w3c/dom/Text.h>
#include <org/w3c/dom/events/MouseEvent.h>
#include <org/w3c/dom/html/CanvasRenderingContext2D.h>
#include <org/w3c/dom/html/Function.h>
#include <org/w3c/dom/html/HTMLCanvasElement.h>
#include <org/w3c/dom/html/HTMLAnchorElement.h>
#include <org/w3c/dom/html/BarProp.h>
#include <org/w3c/dom/views/ClientRect.h>
#include <org/w3c/dom/stylesheets/StyleSheet.h>

using namespace org::w3c::dom;

#include <math.h>
#include <new>

void TestInstance::down(events::Event evt)
{
    events::MouseEvent mouse = interface_cast<events::MouseEvent>(evt);
    printf("down %p\n", mouse.self());
    if (!mouse)
        return;
    printf("(%d, %d), (%d, %d), (%d, %d), (%d, %d), (%d, %d) @ %llu\n",
            mouse.getScreenX(), mouse.getScreenY(),
            mouse.getClientX(), mouse.getClientY(),
            mouse.getPageX(), mouse.getPageY(),
            mouse.getX(), mouse.getY(),
            mouse.getOffsetX(), mouse.getOffsetY(),
            mouse.getTimeStamp());

    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(mouse.getTarget());
    if (!canvas)
        return;
    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (!context)
        return;
    int x = mouse.getClientX() - canvas.getOffsetLeft();
    int y = mouse.getClientY() - canvas.getOffsetTop();
    context.setFillStyle("blue");
    context.fillRect(x, y, 5, 5);

#if 0
    // Test variadic operation
    com::getfirebug::Console* console = interface_cast<com::getfirebug::Console*>(window->getElement("console"));
    printf("console = %p\n", console);
    if (console)
    {
        console->log("Hello %d %d %d!\n", Sequence<Any>({ 1, 2, 3 }));
    }
#endif
}

void TestInstance::drawCharts(Document document)
{
    // Test 'stringifier'
    html::HTMLAnchorElement anchor = interface_cast<html::HTMLAnchorElement>(document.getElementById("anchor"));
    printf("anchor = %p\n", anchor.self());
    if (anchor)
    {
        printf("anchor.getHref() = \"%s\"\n", anchor.getHref().c_str());
    }

    // Test Nullable
    Nullable<std::string> documentURI = document.getDocumentURI();
    printf("documentURI = %s\n", documentURI.hasValue() ? documentURI.value().c_str() : "null");

    // Test getting an array object
    stylesheets::StyleSheetList styleSheets = document.getStyleSheets();
    printf("styleSheets.length = %u\n", styleSheets.getLength());
    for (unsigned i = 0; i < styleSheets.getLength(); ++i)
    {
        stylesheets::StyleSheet styleSheet = styleSheets.getElement(i);
        if (styleSheet)
        {
            printf("    %s\n", styleSheet.getHref().c_str());
        }
    }

#if 0
    // Test passing a sequence object
    webgl::WebGLByteArray_Constructor* byteArrayConstructor = interface_cast<webgl::WebGLByteArray_Constructor*>(window->getElement("WebGLByteArray"));
    printf("WebGLByteArray_Constructor = %p\n", byteArrayConstructor);
    if (byteArrayConstructor)
    {
        webgl::WebGLByteArray* byteArray = byteArrayConstructor->createInstance(Sequence<webgl::GLbyte>({ 1, 2, 3 }));
        printf("byteArray = %p\n", byteArray);
        if (byteArray)
        {
            for (unsigned i = 0; i < 3; ++i)
            {
                printf("    %d\n", byteArray->get(i));
            }
        }
    }
#endif

    // Test BarProp
    html::BarProp prop = interface_cast<html::BarProp>(window.getElement("statusbar"));
    printf("prop = %p\n", prop.self());
    if (prop)
    {
        printf("statusbar.visible = %d\n", prop.getVisible());
    }

    std::string title = document.getTitle();
    printf("title = %s\n", title.c_str());

    Element element = document.createElement("div");
    if (element)
    {
        if (Node node = document.createTextNode("hello, world."))
        {
            element.appendChild(node);
        }
        if (html::HTMLElement body = document.getBody())
        {
            body.appendChild(element);
        }
    }

    html::HTMLCanvasElement canvas = interface_cast<html::HTMLCanvasElement>(document.getElementById("canvas"));
    if (!canvas)
    {
        return;
    }

    html::CanvasRenderingContext2D context = interface_cast<html::CanvasRenderingContext2D>(canvas.getContext("2d"));
    if (!context)
    {
        return;
    }

    context.setFillStyle("white");
    context.fillRect(0, 0, 640, 480);

    // Bar graph
    float top = 50.0f;
    float bottom = 250.0f;

    float width = 20.0f;
    float height;
    float x;

    context.setStrokeStyle("rgb(0, 0, 0)");

    height = 100.0f;
    x = 100.0f;
    context.setLineWidth(3);
    context.setFillStyle("rgb(255, 0, 0)");

    context.fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 200.0f;
    context.setFillStyle("rgb(0, 255, 0)");
    context.fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 80.0f;
    context.setFillStyle("rgb(0, 0, 255)");
    context.fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 50.0f;
    context.setFillStyle("rgb(255, 255, 0)");
    context.fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 30.0f;
    context.setFillStyle("rgb(0, 255, 255)");
    context.fillRect(x, bottom - height, width, height);

    context.moveTo(80.0f, top);
    context.lineTo(80.0f, bottom);
    context.lineTo(350.0f, bottom);
    context.setLineWidth(4);
    context.stroke();

    // Circle graph
    float r = 100.0f;  // radius
    float cx = 180.0f; // center
    float cy = 450.0f; // center
    // angle
    float s = 270.0f/180.0f;
    float e = 120.0f/180.0f;

    context.setFillStyle("rgb(255, 0, 0)");
    context.beginPath();
    context.arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context.lineTo(cx, cy);
    context.closePath();
    context.fill();

    s = e;
    e = 240.0f/180.0f;
    context.setFillStyle("rgb(0, 255, 0)");
    context.beginPath();
    context.arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context.lineTo(cx, cy);
    context.closePath();
    context.fill();

    s = e;
    e = 260.0f/180.0f;
    context.setFillStyle("rgb(0, 0, 255)");
    context.beginPath();
    context.arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context.lineTo(cx, cy);
    context.closePath();
    context.fill();

    s = e;
    e = 270.0f/180.0f;
    context.setFillStyle("rgb(255, 255, 0)");
    context.beginPath();
    context.arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context.lineTo(cx, cy);
    context.closePath();
    context.fill();

    if (canvas && downHandler)
        canvas.addEventListener("mouseup", downHandler, true);
}

void TestInstance::initialize()
{
    downHandler = new(std::nothrow) EventHandler(this, &TestInstance::down);

    testClass = new(std::nothrow) TestClass(this);
    if (testClass)
        window.setElement("Test", testClass);

    html::Function function = interface_cast<html::Function>(window.getElement("testFunction"));
    printf("function = %p\n", function.self());
    if (function)
        function.call({ window, "hello, ", "world." });

    document = window.getDocument();
    if (document)
        drawCharts(document);
}

TestInstance::~TestInstance()
{
}