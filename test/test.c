
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

  check(5, ({ int x=3; int *y = &x; *y = 5; x;}));
  check(5, ({ int x=3; int y = 4; int *z = &x; *(z+1) = 5; y;}));
  check(5, ({ int x=3; int y = 4; int *z = &y; *(z-1) = 5; x;}));
  check(1, ({ int x=3; int y = 4; int *z = &y; int *k = &x; z-k;}));
  check(-1, ({ int x=3; int y = 4; int *z = &x; int *k = &y; z-k;}));
  check(5, ({ int x=3; int y = 4; *(&y-1) = 5; x;}));

  check(8, ({ int x=3; int *y = &x;  sizeof(x);}));
  check(8, ({ int x=3; int *y = &x;  sizeof(y);}));
  check(8, ({ int x=3; int *y = &x;  sizeof y;}));
  check(8, ({ int x=3; int *y = &x;  sizeof 10;}));
  check(8, ({ int x=3; int *y = &x;  sizeof -10;}));

  check(4, ({int a[3]; int *p = &a; *p = 4; *a;})); 
  check(4, ({int a[3]; int *c = a; *c = 4; *(c+1) = 5; *(c+2) = 6; *c;}));
  check(4, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *a;}));
  check(5, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+1);}));
  check(6, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+2);}));
  check(4, ({int a[3]; int *c = a; *c = 4; *(c+1) = 5; *(c+2) = 6; *a;}));
  check(5, ({int a[3]; int *c = a; *c = 4; *(c+1) = 5; *(c+2) = 6; *(a+1);}));
  check(6, ({int a[3]; int *c = a; *c = 4; *(c+1) = 5; *(c+2) = 6; *(a+2);}));
  check(0, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;**b;}));
  check(1, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;*(*b+1);}));
  check(2, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;*(*b+2);}));
  check(3, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;*(*b+3);}));
  check(4, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;*(*b+4);}));
  check(5, ({int b[2][3]; int *c = b; *c = 0; *(c+1) = 1; *(c+2) = 2; *(c+3) = 3;*(c+4) = 4; *(c+5) = 5;*(*b+5);}));
  
  check(28, sum(7));
  check(34, fib(8));
  return 0;
}

int test()
{
  calculate();
  return 0;
}
