
int sum(int n)
{
  int s = 0, i;
  for(i =1; i<=n; i=i+1)
  {
    s = s + i;
  }
  return s;
}

int fia(int n)
{
  if(n <= 1) return 1;
  return fia(n-1) + fia(n-2);
}

char add_char(char a, char b, char c,char d,char e,char f) {
  return a+b+c+d+e+f;
}

short add_short(short a, short b, short c,short d,short e,short f) {
  return a+b+c+d+e+f;
}

int add_int(int a, int b, int c,int d,int e,int f) {
  return a+b+c+d+e+f;
}

long add_long(long a, long b, long c,int d,long e,char f) {
  return a+b+c+d+e+f;
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
  check(4, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *x;}));
  check(4, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *a;}));
  check(5, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+1);}));
  check(6, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+2);}));
  check(4, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *a;}));
  check(5, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *(a+1);}));
  check(6, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *(a+2);}));
  check(0, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;**a;}));
  check(1, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+1);}));
  check(2, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+2);}));
  check(3, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+3);}));
  check(4, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+4);}));
  check(5, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+5);}));
  
  check(28, sum(7));
  check(34, fia(8));
  return 0;
}

int test()
{
  check(1+2+3+4+5+6, add_char(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_short(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_int(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_long(1,2,3,4,5,6));
  return 0;
}
