sum(n)
{
	if(n == 1) return 1;
	return sum(n-1) + n;
}

fib(n)
{
	if(n <= 1) return 1;
	return fib(n-1) + fib(n-2);
}

prog()
{
	return sum(5);
}