/*
 * Copyright 2011 Esrille Inc.
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

#include "esnpapi.h"

#include <org/w3c/dom/AnonXMLHttpRequest.h>
#include <org/w3c/dom/XMLHttpRequest.h>
#include <org/w3c/dom/eventsource/EventSource.h>
#include <org/w3c/dom/file/FileReader.h>
#include <org/w3c/dom/file/FileReaderSync.h>
#include <org/w3c/dom/html/HTMLAudioElement.h>
#include <org/w3c/dom/html/HTMLImageElement.h>
#include <org/w3c/dom/html/HTMLOptionElement.h>

Object org::w3c::dom::AnonXMLHttpRequest::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("AnonXMLHttpRequest").toObject();
}

Object org::w3c::dom::XMLHttpRequest::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("XMLHttpRequest").toObject();
}

Object org::w3c::dom::eventsource::EventSource::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("EventSource").toObject();
}

Object org::w3c::dom::file::FileReader::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("FileReader").toObject();
}

Object org::w3c::dom::file::FileReaderSync::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("FileReaderSync").toObject();
}

Object org::w3c::dom::html::HTMLAudioElement::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("Audio").toObject();
}

Object org::w3c::dom::html::HTMLImageElement::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("Image").toObject();
}

Object org::w3c::dom::html::HTMLOptionElement::getConstructor()
{
    if (!currentPluginInstance)
        return 0;
    return currentPluginInstance->window.getElement("Option").toObject();
}
