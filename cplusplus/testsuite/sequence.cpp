#include <iostream>
#include <string>

using namespace std;

#include "sequence.h"

void test1(Sequence<int> a)
{
    cout << a[0] << a[1] << a[2] << '\n';
}

void test2(const Sequence<int>& a)
{
    cout << a[0] << a[1] << a.at(2) << '\n';
}

int main()
{
    Sequence<int> x;
    int a[] = { 0, 1, 2 };
    Sequence<int> y(a, 3);

    cout << y[0] << y[1] << y[2] << '\n';
    y[2] = 3;
    y.at(1) = 4;
    cout << y[0] << y[1] << y[2] << '\n';

    test1(y);
    test2(y);

#if 1
    string b[] = { "a", "b", "c" };
    Sequence<string> s(b, 3);
    cout << (string) s[0] << (string) s[1] << (string) s[2] << ".\n";

    Sequence<string> t = s;
    cout << (string) t[0] << (string) t[1] << (string) t[2] << ".\n";
#endif
}
