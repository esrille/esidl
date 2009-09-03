/*
 * Copyright 2008, 2009 Google Inc.
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

#include "sample.h"
#include "proxyImpl.h"

#include "w3c/html5.h"
#include "w3c/svg.h"

#include <math.h>

namespace
{

void drawCharts(es::Document* document)
{
    // Test dynamic_cast implied by the 'implements' statement while Document does
    // not inherit DocumentEvent.
    es::DocumentEvent* documentEvent = dynamic_cast<es::DocumentEvent*>(document);
    printf("documentEvent = %p\n", documentEvent);

    std::string title = document->getTitle();
    printf("title = %s\n", title.c_str());

    es::Element* element = document->createElement("div");
    if (element)
    {
        if (es::Node* node = document->createTextNode("hello, world."))
        {
            element->appendChild(node);
            node->release();
        }
        if (es::HTMLElement* body = document->getBody())
        {
            body->appendChild(element);
            body->release();
        }
        element->release();
    }

    es::HTMLCanvasElement* canvas = dynamic_cast<es::HTMLCanvasElement*>(document->getElementById("canvas"));
    if (!canvas)
    {
        return;
    }

    es::CanvasRenderingContext2D* context = dynamic_cast<es::CanvasRenderingContext2D*>(canvas->getContext("2d"));
    if (!context)
    {
        canvas->release();
        return;
    }

    context->setFillStyle("white");
    context->fillRect(0, 0, 1024, 768);

    // Bar graph
    float top = 50.0f;
    float bottom = 250.0f;

    float width = 20.0f;
    float height;
    float x;
    float y;

    context->setStrokeStyle("rgb(0, 0, 0)");

    height = 100.0f;
    x = 100.0f;
    context->setLineWidth(3);
    context->setFillStyle("rgb(255, 0, 0)");

    context->fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 200.0f;
    context->setFillStyle("rgb(0, 255, 0)");
    context->fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 80.0f;
    context->setFillStyle("rgb(0, 0, 255)");
    context->fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 50.0f;
    context->setFillStyle("rgb(255, 255, 0)");
    context->fillRect(x, bottom - height, width, height);

    x += 50.0f;
    height = 30.0f;
    context->setFillStyle("rgb(0, 255, 255)");
    context->fillRect(x, bottom - height, width, height);

    context->moveTo(80.0f, top);
    context->lineTo(80.0f, bottom);
    context->lineTo(350.0f, bottom);
    context->setLineWidth(4);
    context->stroke();

    // Circle graph
    float r = 100.0f;  // radius
    float cx = 180.0f; // center
    float cy = 450.0f; // center
    // angle
    float s = 270.0f/180.0f;
    float e = 120.0f/180.0f;

    context->setFillStyle("rgb(255, 0, 0)");
    context->beginPath();
    context->arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context->lineTo(cx, cy);
    context->closePath();
    context->fill();

    s = e;
    e = 240.0f/180.0f;
    context->setFillStyle("rgb(0, 255, 0)");
    context->beginPath();
    context->arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context->lineTo(cx, cy);
    context->closePath();
    context->fill();

    s = e;
    e = 260.0f/180.0f;
    context->setFillStyle("rgb(0, 0, 255)");
    context->beginPath();
    context->arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context->lineTo(cx, cy);
    context->closePath();
    context->fill();

    s = e;
    e = 270.0f/180.0f;
    context->setFillStyle("rgb(255, 255, 0)");
    context->beginPath();
    context->arc(cx, cy, r, M_PI * s, M_PI * e, 0);
    context->lineTo(cx, cy);
    context->closePath();
    context->fill();

    context->release();
    canvas->release();

    es::SVGCircleElement* circle = dynamic_cast<es::SVGCircleElement*>(document->getElementById("circle"));
    if (circle)
    {
        circle->setAttribute("r", "20");
        circle->setAttribute("fill", "pink");
        circle->release();
    }
}

}  // namespace

void PluginInstance::test()
{
    NPVariant document;
    VOID_TO_NPVARIANT(document);
    NPN_GetProperty(npp, window, NPN_GetStringIdentifier("document"), &document);
    if (NPVARIANT_IS_OBJECT(document))
    {
        std::string interfaceName = getInterfaceName(npp, NPVARIANT_TO_OBJECT(document));
        printf("'%s'\n", interfaceName.c_str());
        drawCharts(dynamic_cast<es::Document*>(createProxy(npp, NPVARIANT_TO_OBJECT(document))));
    }
    NPN_ReleaseVariantValue(&document);
}
