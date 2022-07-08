def f(x)
{
	result = 1
	i = 2
	while(i != x + 1)
	{
		result = result * i
		i = i + 1
	}
	return result
}

def double(x)
{
	return x + x
}

print(double(f(5)))