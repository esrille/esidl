#include <string>

class Object
{
public:
    virtual std::string toString() = 0;
};

#include <any.h>

#include <nullable.h>
#include <sequence.h>
#include <reflect.h>

#include <org/w3c/dom/NameList.h>
#include <org/w3c/dom/html/Window.h>

#include <org/w3c/dom/css/CSSStyleDeclaration.h>

#include <org/w3c/dom/Document.h>
#include <org/w3c/dom/Element.h>
#include <org/w3c/dom/html/ApplicationCache.h>
#include <org/w3c/dom/html/BarProp.h>
#include <org/w3c/dom/html/Function.h>
#include <org/w3c/dom/html/History.h>
#include <org/w3c/dom/html/Location.h>
#include <org/w3c/dom/html/MessagePort.h>
#include <org/w3c/dom/html/Navigator.h>
#include <org/w3c/dom/html/Screen.h>
#include <org/w3c/dom/html/Selection.h>
#include <org/w3c/dom/html/StyleMedia.h>
#include <org/w3c/dom/html/UndoManager.h>
#include <org/w3c/dom/html/Window.h>

#include <org/w3c/dom/webdatabase/Database.h>
#include <org/w3c/dom/webdatabase/DatabaseCallback.h>

#include <org/w3c/dom/webstorage/Storage.h>

#include <iostream>
#include <string>

using namespace std;
using namespace org::w3c::dom;

template<class B>
class Object_Bridge : public B
{
public:
    virtual std::string toString()
    {
        return "";
    }
};

Any invoke(Object* object, unsigned interfaceNumber, unsigned methodNumber,
           const char* meta, unsigned offset,
           unsigned argumentCount, Any* arguments)
{
    Reflect::Interface interface(meta);
    cout << interface.getName() << endl;
    Reflect::Method method(meta + offset);
    cout << method.getName() << ':' << method.getParameterCount() << endl;
    return Nullable<string>("Hi");
}

int main()
{
    Object_Bridge<NameList_Bridge<Any, invoke> > nameList;

    Nullable<string> name = nameList.getName(3);
    cout << name.value() << endl;

    name = nameList.getNamespaceURI(2);
    cout << name.value() << endl;

    Object_Bridge<html::Window_Bridge<Any, invoke> > window;
    window.close();
    window.setLength(100);
    window.postMessage("hi", "hmm");
}
