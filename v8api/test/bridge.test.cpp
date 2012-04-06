/*
 * Copyright 2011, 2012 Esrille Inc.
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

#include <esv8api.h>

#include <assert.h>

#include <iostream>

#include "DOMStringMapImp.h"
#include "EventImp.h"
#include "EventListenerImp.h"
#include "EventTargetImp.h"
#include "HTMLCollectionImp.h"

using namespace org::w3c::dom;

const char* script =
    "var r = new String('');"
    "var l = function(e) { r += e.type + ' ' + e.color + ' ' + e.meal + '\\ndone!\\n' };"
    "for(prop in target.__proto__)"
    "    r += prop + '\\n';"
    "r += 'EventTarget.TEXT_NODE = ' + EventTarget.TEXT_NODE + '\\n';"
    "var e = new Event('type', { color: 'blue'});"
    "e.type = 'TYPE';"
    "target.dispatchEvent(e);"
    "var m = target.dataset;"
    "var x = m.x;"
    "m.x = 'hello';"
    "r += m.x + '\\n';"
    "delete m.x;"
    "e = 0;"    // ~EventImp() should be called later.
    "var c = target.collection;"
    "var i = c.item(3);"
    "var i = c[3];"
    "var i = c(3);"
    "var i = c('color');"
    "r += target + '\\n';"
    "target.overloaded(target.collection);"
    "target.overloaded(target.dataset);"
    "c.namedItem(target);"
    "c.namedItem(Function);"
    "r += c.length;"
    "Event.staticOperation();"
    "gc();"
    "r;"
;

void testV8Bridge()
{
    v8::HandleScope handleScope;

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    // Register classes
    EventImp::setStaticPrivate(new NativeClass(global, test::Event::getMetaData(), test::Event::getConstructor));
    EventTargetImp::setStaticPrivate(new NativeClass(global, test::EventTarget::getMetaData()));
    EventListenerImp::setStaticPrivate(new NativeClass(global, test::EventListener::getMetaData()));
    DOMStringMapImp::setStaticPrivate(new NativeClass(global, test::DOMStringMap::getMetaData()));
    HTMLCollectionImp::setStaticPrivate(new NativeClass(global, test::HTMLCollection::getMetaData()));

    const char* extensionNames[] = {
        "v8/gc",
    };
    v8::ExtensionConfiguration extensions(1, extensionNames);
    v8::Persistent<v8::Context> context = v8::Context::New(&extensions, global);

    v8::Context::Scope contextScope(context);

    // Create an instance of EventTarget for test
    org::w3c::dom::test::EventTarget target = new EventTargetImp;
    ObjectImp* imp = dynamic_cast<ObjectImp*>(target.self());
    v8::Handle<v8::Object> jstarget = static_cast<NativeClass*>(imp->getStaticPrivate())->createJSObject(imp);
    context->Global()->Set(v8::String::New("target"), jstarget);

    org::w3c::dom::test::EventListener listener = compileFunction(context, u"r += e.type + ' ' + e.color + ' :-) ' + e.meal + '\\ndone!\\n'");
    target.addEventListener(u"t", listener);

    v8::Handle<v8::String> source = v8::String::New(script);
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    v8::Handle<v8::Value> result = script->Run();

    context.Dispose();

    v8::String::AsciiValue ascii(result);
    std::cout << *ascii << '\n';
}

int main(int argc, const char* argv[])
{
    testV8Bridge();
}

namespace org {
namespace w3c {
namespace dom {

namespace {

class Constructor : public Object
{
public:
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        switch (selector) {
        case 0x674a34b6:
            if (argc == 0) {
                EventImp::staticOperation();
                return Any();
            }
            if (argc == HAS_PROPERTY_)
                return true;
            break;
        case 0:
            if (argc == 1)
                return new(std::nothrow) EventImp(argv[0].toString());
            if (argc == 2)
                return new(std::nothrow) EventImp(argv[0].toString(), argv[1].toObject());
            break;
        default:
            break;
        }
        return Any();
    }
    Constructor() :
        Object(this) {
    }
};

}  // namespace

Object test::Event::getConstructor()
{
    static Constructor constructor;
    return constructor.self();
}

}}}
