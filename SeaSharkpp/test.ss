using System
using Time

struct asd
{
    b = 10
}

struct test
{
    val = asd()
}

a = [test()]

a = set(a, 0, get(a, 0).set("val", get(a, 0).get("val").set("b", 100)))

printline(get(a, 0).val.b)