using System

def test()
{
	return "Hello World!"
}

i = 0
while(i < test().size())
{
	printline(test().get(i))
	i++
}