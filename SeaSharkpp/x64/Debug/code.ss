

def Factorial(x)
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

def intPow(x, y)
{
	result = 1
	i = 0
	while(i != y)
	{
		result = result * x
		i = i + 1
	}
	return result
}

def sin(x)
{
	result = 0
	i = 0
	while(i != 20)
	{
		num = 2 * i + 1
		pow = intPow(x, num)
		fact = Factorial(num)
		diff = pow / fact
		if(i % 2 == 0)
		{
			result = result + diff
		}
		if(i % 2 == 1)
		{
			result = result - diff
		}
		print(i)
		i = i + 1
	}
	return result
}

print(sin(1.5))