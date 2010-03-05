class Object {};
#include <string>
#include <any.h>
#include <nullable.h>
#include <sequence.h>
#include <reflect.h>

#include <org/w3c/dom/NameList.h>

#include <iostream>
#include <string>

using namespace std;
using namespace org::w3c::dom;

Any invoke(Object* object, unsigned interfaceNumber, unsigned methodNumber,
           const char* meta, unsigned offset,
           unsigned paramCount, Any* arguments)
{
    Reflect::Interface interface(meta);
    cout << interface.getName() << endl;
    Reflect::Method method(meta + offset);
    cout << method.getName() << ':' << method.getParameterCount() << endl;
    return Nullable<string>("Hi");
}

int main()
{
    NameList_Bridge<Any, invoke> nameList;

    Nullable<string> name = nameList.getName(3);
    cout << name.value() << endl;

    name = nameList.getNamespaceURI(2);
    cout << name.value() << endl;
}
