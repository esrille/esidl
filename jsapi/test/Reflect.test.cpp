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

#include "Reflect.h"

#include <iostream>

#include <Object.h>

const char* nodeData =
            "I21::org::w3c::dom::Node"
            "X36::org::w3c::dom::events::EventTarget"
            "K12ELEMENT_NODE"
            "K14ATTRIBUTE_NODE"
            "K9TEXT_NODE"
            "K18CDATA_SECTION_NODE"
            "K21ENTITY_REFERENCE_NODE"
            "K11ENTITY_NODE"
            "K27PROCESSING_INSTRUCTION_NODE"
            "K12COMMENT_NODE"
            "K13DOCUMENT_NODE"
            "K18DOCUMENT_TYPE_NODE"
            "K22DOCUMENT_FRAGMENT_NODE"
            "K13NOTATION_NODE"
            "A8nodeType"
            "A8nodeName"
            "A7baseURI"
            "A13ownerDocument"
            "A10parentNode"
            "A13parentElement"
            "F13hasChildNodes"
            "A10childNodes"
            "A10firstChild"
            "A9lastChild"
            "A15previousSibling"
            "A11nextSibling"
            "K30DOCUMENT_POSITION_DISCONNECTED"
            "K27DOCUMENT_POSITION_PRECEDING"
            "K27DOCUMENT_POSITION_FOLLOWING"
            "K26DOCUMENT_POSITION_CONTAINS"
            "K30DOCUMENT_POSITION_CONTAINED_BY"
            "K41DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC"
            "F23compareDocumentPosition"
            "A9nodeValue"
            "A11textContent"
            "F12insertBefore"
            "F12replaceChild"
            "F11removeChild"
            "F11appendChild"
            "F9cloneNode"
            "F10isSameNode"
            "F11isEqualNode"
            "F12lookupPrefix"
            "F18lookupNamespaceURI"
            "F18isDefaultNamespace";

const char* imageData =
            "Ic39::org::w3c::dom::html::HTMLImageElement"
            "X34::org::w3c::dom::html::HTMLElement"
            "A3alt"
            "A3src"
            "A6useMap"
            "A5isMap"
            "A5width"
            "A6height"
            "A12naturalWidth"
            "A13naturalHeight"
            "A8complete"
            "A4name"
            "A5align"
            "A6border"
            "A6hspace"
            "A8longDesc"
            "A6vspace";

void test(Reflect::Interface& interface)
{
    std::cout << interface.getOperationCount() << '\n';
    std::cout << interface.getAttributeCount() << '\n';
    std::cout << interface.getConstantCount() << '\n';
    std::cout << interface.getStringSize() << '\n';
    Reflect::Property prop(interface);
    while (prop.getType()) {
        std::cout << prop.getType() << ": " << prop.getName().c_str() << '\n';
        prop.next();
    }
}

int main()
{
    Reflect::Interface node(nodeData);
    test(node);
    Reflect::Interface image(imageData);
    test(image);
}