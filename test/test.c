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

int ret() {
  return 100;
}

int sum(int n) {
  int res = 0, i = 0;
  for (i=0;i<=n;i=i+1) {
    res = res + i;
  }
  return res;
}

int fib(int n) {
  if (n <= 1) {
    return 1;
  }
  return fib(n-1) + fib(n-2);
}


int test() 
{
  check(14, ({ 1 + 2 + 4 -2 +9  ;}));
  check(-2, ({ 10 -  (2 + 5) - 2 - 3;}));
  check(-14, ({ -+10+5-10+1;}));
  check(3, ({ ----1+2;}));
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

  check(5, ({int a = 4; if(1) a=5; a;}));
  check(5, ({int a = 4; if(1) a=5; else a=6; a;}));
  check(5, ({int a = 4; if(2>=1) a=5; else a=6; a;}));
  check(6, ({int a = 4; if(0) a=5; else a=6; a;}));
  check(6, ({int a = 4; if(1-1) a=5; else a=6; a;}));
  check(6, ({int a = 4; if(1<1) a=5; else a=6; a;}));
  check(6, ({int a = 4; if(1>=2) a=5; else a=6; a;}));
  check(25, ({int a = 4; if(a) {a=5;a=a*a;} else a=6; a;}));
  check(7, ({int a = 4; if(a) {a=5;a=a*a;} else a=6; a=7; a;}));
  check(25, ({int a = 4; if(a) {a=5;a=a*a;} else {a=6; a=7;} a;}));
  check(6, ({int a = 4; if(1>=2); else a=6; a;}));
  check(0, ({int a=10; while (a) a = a-1; a;}));
  check(10, ({int i=10; int s = 0; while (i) {i=i-1;s=s+1;} s;}));
  check(45, ({int i=10; int s = 0; while (i) {i=i-1;s=s+i;} s;}));
  check(4950, ({int i=0; int s = 0; while (i<100) {s=s+i;i=i+1;} s;}));
  check(0, ({int a=10; do {a = a-1;} while (a); a;}));
  check(10, ({int i=10; int s = 0; do {i=i-1;s=s+1;} while (i);  s;}));
  check(45, ({int i=10; int s = 0; do {i=i-1;s=s+i;} while (i);  s;}));
  check(4950, ({int i=0; int s = 0; do {s=s+i;i=i+1;} while (i<100);  s;}));
  check(18, ({int i = 0,j=0; int sum= 0;for (i=0;i<10;i=i+1) {sum = i+j;j=j+1;} sum;}));
  check(18, ({int i = 0,j=0; int sum= 0;for (;i<10;i=i+1) {sum = i+j;j=j+1;} sum;}));
  check(18, ({int i = 0,j=0; int sum= 0;for (;i<10;) {sum = i+j;j=j+1;i=i+1;} sum;}));

  check(3, ({ int x=3; *&x; }));
  check(3, ({ int x=3; int *y=&x; int **z=&y; **z; }));
  check(5, ({ int x=3; int *y = &x; *y = 5; x;}));
  check(5, ({ int x=3; int y = 4; int *z = &x; *(z-1) = 5; y;}));
  check(5, ({ int x=3; int y = 4; int *z = &y; *(z+1) = 5; x;}));
  check(5, ({ int x=3; int y = 4; int *z = &y; *(1+z) = 5; x;}));
  check(5, ({ int x=3; int y = 4; int *z = &x; *(-1+z) = 5; y;}));
  check(1, ({ int x=3; int y = 4; int *z = &y; int *k = &x; k-z;}));
  check(-1, ({ int x=3; int y = 4; int *z = &x; int *k = &y; k-z;}));
  check(5, ({ int x=3; int y = 4; *(&y+1) = 5; x;}));
  check(4, ({ int x=3; int *y = &x;  sizeof(x);}));
  check(8, ({ int x=3; int *y = &x;  sizeof(y);}));
  check(8, ({ int x=3; int *y = &x;  sizeof y;}));
  check(8, ({ int x=3; int *y = &x;  sizeof 10;}));
  check(8, ({ int x=3; int *y = &x;  sizeof -10;}));
  check(1, ({char x; sizeof(x);}));
  check(2, ({short x; sizeof(x);}));
  check(4, ({int x; sizeof(x);}));
  check(8, ({long x; sizeof(x);}));
  // check(-23, ({int x = 4567456745; char *p = &x; *p;}));
  // check(-37, ({int x = 4567456745; char *p = &x; *(p+1);}));
  // check(61, ({int x = 4567456745; char *p = &x; *(p+2);}));
  // check(16, ({int x = 4567456745; char *p = &x; *(p+3);}));
  // check(-9239, ({int x = 4567456745; short *p = &x; *p;}));
  // check(4157, ({int x = 4567456745; short *p = &x; *(p+1);}));

  check(4, ({int a[3]; int *p = &a; *p = 4; *a;}));
  check(4, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *x;}));
  check(4, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *a;}));
  check(5, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+1);}));
  check(6, ({int a[3]; *a = 4; *(a+1) = 5; *(a+2) = 6; *(a+2);}));
  check(4, ({int a[3]; a[0] = 4; int *x = a; *x;}));
  check(5, ({int a[3]; a[1] = 5; int *x = a; *(x+1);}));
  check(6, ({int a[3]; a[2] = 6; int *x = a; *(x+2);}));
  check(4, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *a;}));
  check(5, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *(a+1);}));
  check(6, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; *(a+2);}));
  check(3*4, ({int a[3]; sizeof(a);}));
  check(12*4, ({int a[3][4]; sizeof(a);}));
  check(4, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; a[0];}));
  check(5, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; a[1];}));
  check(6, ({int a[3]; int *x = a; *x = 4; *(x+1) = 5; *(x+2) = 6; a[2];}));
  check(0, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;**a;}));
  check(1, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+1);}));
  check(2, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+2);}));
  check(3, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+3);}));
  check(4, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+4);}));
  check(5, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;*(*a+5);}));
  check(0, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[0][0];}));
  check(1, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[0][1];}));
  check(2, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[0][2];}));
  check(3, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[1][0];}));
  check(4, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[1][1];}));
  check(5, ({int a[2][3]; int *x = a; *x = 0; *(x+1) = 1; *(x+2) = 2; *(x+3) = 3;*(x+4) = 4; *(x+5) = 5;a[1][2];}));
  check(0, ({int a[2][3]; int *x = a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][0];}));
  check(1, ({int a[2][3]; int *x = a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][1];}));
  check(2, ({int a[2][3]; int *x = a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][2];}));
  check(0, ({int a[2][3]; int *x = *(0+a); *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][0];}));
  check(0, ({int a[2][3]; int *x = 0[a]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][0];}));
  check(1, ({int a[2][3]; int *x = 0[a]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][1];}));
  check(2, ({int a[2][3]; int *x = 0[a]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][2];}));
  check(0, ({int a[2][3]; int *x = &a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][0];}));
  check(1, ({int a[2][3]; int *x = &a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][1];}));
  check(2, ({int a[2][3]; int *x = &a[0]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[0][2];}));
  check(0, ({int a[2][3]; int *x = &a[1]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[1][0];}));
  check(1, ({int a[2][3]; int *x = &a[1]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[1][1];}));
  check(2, ({int a[2][3]; int *x = &a[1]; *x = 0; *(x+1) = 1; *(x+2) = 2;a[1][2];}));
  check(30, ({ int a[2][3]; a[1][0] = 5;a[1][1] = 6;a[1][0] * a[1][1];}));
  check(10*1, ({char a[10]; sizeof(a);}));
  check(10*2, ({short a[10]; sizeof(a);}));
  check(10*4, ({int a[10]; sizeof(a);}));
  check(10*8, ({long a[10]; sizeof(a);}));

  check(1+2+3+4+5+6, add_char(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_short(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_int(1,2,3,4,5,6));
  check(1+2+3+4+5+6, add_long(1,2,3,4,5,6));
  check(89, fib(sum(({int i = 4, j;}))));
  check(17711, fib(sum(({int i = 4, j = 5; j * 2 - i;}))));

  return 0;
}
