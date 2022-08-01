using System

struct vector
{
	Name = ""
	x = 0
	y = 0
	z = 0

	def GetName()
	{
		return "Position"
	}
}

def PrintVector(vec)
{
	print(vec.Name)
	print(": (")
	print(vec.x)
	print(", ")
	print(vec.y)
	print(", ")
	print(vec.z)
	printline(")")
}	

position = vector()

position = position.set("x", 50)
position = position.set("Name", "Position vector")

PrintVector(position)
printline(position.GetName())