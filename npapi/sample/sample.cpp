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

#include <math.h>

namespace
{

void drawCharts(es::CanvasRenderingContext2D* context)
{
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
}

}  // namespace

void PluginInstance::test()
{
    NPVariant document;
    VOID_TO_NPVARIANT(document);
    NPN_GetProperty(npp, window, NPN_GetStringIdentifier("document"), &document);
    if (NPVARIANT_IS_OBJECT(document))
    {
        NPVariant name;
        NPN_GetProperty(npp, NPVARIANT_TO_OBJECT(document), NPN_GetStringIdentifier("title"), &name);
        if (NPVARIANT_IS_STRING(name))
        {
            printf("title = %.*s\n", NPVARIANT_TO_STRING(name).utf8length, NPVARIANT_TO_STRING(name).utf8characters);
            NPN_ReleaseVariantValue(&name);
        }

        STRINGZ_TO_NPVARIANT("canvas", name);
        NPVariant canvas;
        VOID_TO_NPVARIANT(canvas);
        NPN_Invoke(npp, NPVARIANT_TO_OBJECT(document), NPN_GetStringIdentifier("getElementById"), &name, 1, &canvas);
        if (NPVARIANT_IS_OBJECT(canvas))
        {
            NPVariant context;
            VOID_TO_NPVARIANT(context);
            STRINGZ_TO_NPVARIANT("2d", name);
            NPN_Invoke(npp, NPVARIANT_TO_OBJECT(canvas), NPN_GetStringIdentifier("getContext"), &name, 1, &context);
            if (NPVARIANT_IS_OBJECT(context))
            {
                std::string interfaceName = getInterfaceName(npp, NPVARIANT_TO_OBJECT(context));
                printf("'%s'\n", interfaceName.c_str());
                STRINGZ_TO_NPVARIANT("red", name);
                NPN_SetProperty(npp, NPVARIANT_TO_OBJECT(context), NPN_GetStringIdentifier("fillStyle"), &name);

                NPVariant rect[4];
                DOUBLE_TO_NPVARIANT(0.0, rect[0]);
                DOUBLE_TO_NPVARIANT(0.0, rect[1]);
                DOUBLE_TO_NPVARIANT(1024.0, rect[2]);
                DOUBLE_TO_NPVARIANT(768.0, rect[3]);
                NPN_Invoke(npp, NPVARIANT_TO_OBJECT(context), NPN_GetStringIdentifier("fillRect"), rect, 4, &name);

                drawCharts(static_cast<es::CanvasRenderingContext2D*>(createProxy(npp, NPVARIANT_TO_OBJECT(context))));
            }
            NPN_ReleaseVariantValue(&context);
        }
        NPN_ReleaseVariantValue(&canvas);
    }
    NPN_ReleaseVariantValue(&document);
}