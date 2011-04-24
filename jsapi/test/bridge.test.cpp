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

#include <esjsapi.h>

#include <assert.h>

#include <iostream>

#include "DOMStringMapImp.h"
#include "EventImp.h"
#include "EventListenerImp.h"
#include "EventTargetImp.h"
#include "HTMLCollectionImp.h"

using namespace org::w3c::dom;

static JSClass globalClass = {
    "global", JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

void reportError(JSContext* cx, const char* message, JSErrorReport* report)
{
    std::cerr << (report->filename ? report->filename : "<no filename>") << report->lineno << message;
}

const char* script =
    "var l = function(e) { r += '\\ndone!\\n' };"
    "var r = new String('');"
    "for(prop in target.__proto__)"
    "    r += prop + '\\n';"
    "r += 'EventTarget.TEXT_NODE = ' + EventTarget.TEXT_NODE + '\\n';"
    "target.addEventListener('t', l);"
    "var e = Event('t');"
    "target.dispatchEvent(e);"
    "var m = target.dataset;"
    "var x = m.x;"
    "m.x = 'hello';"
    "r += m.x + '\\n';"
    "delete m.x;"
    "delete e;"
    "var c = target.collection;"
    "var i = c[3];"
    "var i = c(3);"
    "var i = c.item(3);"
    "r += target + '\\n';"
    "target.overloaded(target.collection);"
    "target.overloaded(target.dataset);"
    "c.namedItem(target);"
    "c.namedItem(Function);"
    "r;";

int main(int argc, const char* argv[])
{
    JSRuntime* rt = JS_NewRuntime(8L * 1024L * 1024L);
    if (!rt)
        return EXIT_FAILURE;

    jscontext = JS_NewContext(rt, 8192);
    if (!jscontext)
        return EXIT_FAILURE;
    JS_SetOptions(jscontext, JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
    JS_SetVersion(jscontext, JSVERSION_LATEST);
    JS_SetErrorReporter(jscontext, reportError);

    JSObject* global = JS_NewCompartmentAndGlobalObject(jscontext, &globalClass, NULL);
    if (!global)
        return EXIT_FAILURE;
    if (!JS_InitStandardClasses(jscontext, global))
        return EXIT_FAILURE;

    // Register classes
    EventImp::setStaticPrivate(new NativeClass(jscontext, test::Event::getMetaData(), test::Event::getConstructor));
    EventTargetImp::setStaticPrivate(new NativeClass(jscontext, test::EventTarget::getMetaData()));
    EventListenerImp::setStaticPrivate(new NativeClass(jscontext, test::EventListener::getMetaData()));
    DOMStringMapImp::setStaticPrivate(new NativeClass(jscontext, test::DOMStringMap::getMetaData()));
    HTMLCollectionImp::setStaticPrivate(new NativeClass(jscontext, test::HTMLCollection::getMetaData()));

    // Create an instance of EventTarget for test
    org::w3c::dom::test::EventTarget target = new EventTargetImp;
    ObjectImp* imp = dynamic_cast<ObjectImp*>(target.self());
    JSObject* jstarget = static_cast<NativeClass*>(imp->getStaticPrivate())->createJSObject(jscontext, imp);
    JS_DefineProperty(jscontext, JS_GetGlobalObject(jscontext), "target", OBJECT_TO_JSVAL(jstarget),
                      0, 0,
                      JSPROP_ENUMERATE);

    jsval rval;
    const char* filename = "";
    int lineno = 0;
    JSBool ok = JS_EvaluateScript(jscontext, global, script, strlen(script), filename, lineno, &rval);
    JSString* str = JS_ValueToString(jscontext, rval);
    std::cout << JS_EncodeString(jscontext, str);

    JS_DestroyContext(jscontext);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
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
        EventImp* evt = 0;
        switch (argc) {
        case 1:
            evt = new(std::nothrow) EventImp(argv[0].toString());
            break;
        default:
            break;
        }
        return evt ;
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
