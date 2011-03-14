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

#include "xhr.h"
#include <new>

#include <org/w3c/dom/Document.h>
#include <org/w3c/dom/XMLHttpRequest.h>
#include <org/w3c/dom/html/HTMLButtonElement.h>
#include <org/w3c/dom/html/HTMLFormElement.h>
#include <org/w3c/dom/html/HTMLInputElement.h>

#include <stdio.h>

using namespace org::w3c::dom;

void XHRInstance::initialize()
{
    loadHandler = new(std::nothrow) EventHandler(this, &XHRInstance::load);
    displayHandler = new(std::nothrow) EventHandler(this, &XHRInstance::display);
    Document document = interface_cast<Document>(window.getDocument());
    if (!document)
        return;
    html::HTMLFormElement form = interface_cast<html::HTMLFormElement>(document.getElement("form"));
    if (!form)
        return;
    html::HTMLButtonElement button = interface_cast<html::HTMLButtonElement>(form.namedItem("button"));
    if (!button)
        return;
    button.addEventListener("click", loadHandler, true);
}

XHRInstance::~XHRInstance()
{
}

void XHRInstance::load(events::Event evt)
{
    XMLHttpRequest xhr;
    if (!xhr)
        return;
    xhr.addEventListener("load", displayHandler, true);
    xhr.open("GET", "xhr.txt", true);
    xhr.send();
}

void XHRInstance::display(events::Event evt)
{
    XMLHttpRequest xhr = interface_cast<XMLHttpRequest>(evt.getTarget());
    if (!xhr)
        return;
    Document document = window.getDocument();
    if (!document)
        return;
    html::HTMLFormElement form = interface_cast<html::HTMLFormElement>(document.getElement("form"));
    if (!form)
        return;
    html::HTMLInputElement result = interface_cast<html::HTMLInputElement>(form.namedItem("result"));
    if (result)
        result.setValue(xhr.getResponseText());
}
