array = []
i = 0
while(i < 100)
{
	array = append(array, i)
	i++
}
i = 0
while(i < 100)
{
	print(get(array, i))
	i++
}