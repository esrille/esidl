#include <iostream>
#include <string>

using namespace std;

class Object
{
public:
};

#include <any.h>

void test1(Sequence<int> a)
{
    cout << a[0] << a[1] << a[2] << endl;
}

void test2(const Sequence<int>& a)
{
    cout << a[0] << a[1] << a.at(2) << endl;
}

int main()
{
    Sequence<int> x;
    int a[] = { 0, 1, 2 };
    Sequence<int> y(a, 3);

    cout << y[0] << y[1] << y[2] << endl;
    y[2] = 3;
    y.at(1) = 4;
    cout << y[0] << y[1] << y[2] << endl;

    test1(y);
    test2(y);

    string b[] = { "a", "b", "c" };
    Sequence<string> s(b, 3);
    cout << (string) s[0] << (string) s[1] << (string) s[2] << endl;

    Sequence<string> t = s;
    cout << (string) t[0] << (string) t[1] << (string) t[2] << endl;

    cout << sizeof(Sequence<int>) << ' ' << sizeof(Sequence<string>) << endl;

    Sequence<int> y2 = y;
    test1(y2);

    Any a1 = y;
    Sequence<int> y3 = a1;
    test1(y3);

    Any a2 = s;
    Sequence<string> y4 = a2;
    cout << y4.getLength() << (string) y4[0] << (string) y4[1] << (string) y4[2] << endl;

    Any array[1];
    array[0] = s;
    Sequence<string> z = array[0];
    cout << z.getLength() << (string) z[0] << (string) z[1] << (string) z[2] << endl;
    
}
