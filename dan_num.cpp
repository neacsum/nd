/*
  A program to find the "Dan numbers".

  A DAN(n) number is a number with the property that, if you take the least
  significant digit and place it in the most significant position, you obtain
  a number that is "n" times the original.

  For instance 102564 is DAN(4) number because 102564*4 = 410256
*/
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <conio.h>

#define DAN 4

void print(char* num, int digits)
{
  for (int i = digits; i >=0; --i)
    putch(num[i] + '0');
  putch('\n');
}

int main(int argc, char** argv)
{
  char num[100];

  char a0 = DAN;
  while (a0 < 10)
  {
    memset(num, 0xff, sizeof(num));
    num[0] = a0;
    int carry = 0;
    int i = 1;
    while (i < sizeof(num))
    {
      num[i] = DAN * num[i - 1];
      if (carry)
        num[i] += carry;

      if (num[i] > 9)
      {
        carry = num[i] / 10;
        num[i] %= 10;
      }
      else
        carry = 0;
      if (!carry && num[i] == a0)
      {
        --i;
        break;
      }

      ++i;
    }
    if (i < sizeof(num))
    {
      std::cout << "For a0=" << (int)a0 << " solution found\n";
      print(num, i);
    }
    else
    {
      std::cout << "For a0=" << (int)a0 << " no solution\n";
    }
    a0 += DAN;
  }
  return 0;
}