
int calculate() 
{
  check(14, ({ 1 + 2 + 4 -2 +9  ;}));
  check(8, ({ 2 * 4;}));
  check(3, ({ 6 / 2;}));
  check(1,({ 10 -  (2 + 5) - 6 / 3;}));
  check(1, ({ 4>2;}));
  check(1, ({ 4>=2;}));
  check(0, ({ 4<2;}));
  check(0, ({ 4<=2;}));
  check(0, ({ 4==2;}));
  check(1, ({ 4!=2;}));
  check(1, ({ 4>(2);}));
  check(1, ({ 4>(2+1);}));
  check(1, ({ 4>=(2+3-2);}));
  check(0, ({ 4<2;}));
  check(0, ({ 4<=2;}));
  check(1, ({ 4-3<=3+4;}));
  check(1, ({ 4-(3+2)<=3+(4+4)-2*3;}));
  check(0, ({ 4-(3+2)==3+(4+4)-2*3;}));
  check(1, ({ 4-(3+2)!=3+(4+4)-2*3;}));
  check(5, ({ 4-(3+2)!=3+(4+4)-2*3;1+4;}));
  check(1, ({ 3*3;3-3;3/3;}));
  check(-2, ({ 10 -  (2 + 5) - 2 - 3;}));
  check(-14, ({ -+10+5-10+1;}));
  check(3, ({ ----1+2;}));
  check(3, ({ int x=3; *&x; }));
  check(3, ({ int x=3; int *y=&x; int **z=&y; **z; }));
  return 0;
}

int sum(int n)
{
  int s = 0, i;
  for(i =1; i<=n; i=i+1)
  {
    s = s + i;
  }
  return s;
}

int fib(int n)
{
  if(n <= 1) return 1;
  return fib(n-1) + fib(n-2);
}

int test()
{
  calculate();
  check(28, sum(7));
  check(34, fib(8));
  return 0;
}
