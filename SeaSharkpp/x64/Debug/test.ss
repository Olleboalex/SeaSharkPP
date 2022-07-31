using System

struct test
{
	value = 5
	check = true
	def hello()
	{
		return "Hello World"
	}
}

a = test()
a = a.set("value", 100)
a = a.set("check", false)

if(!a.check)
{
	printline(a.hello())
	printline(a.value)
}