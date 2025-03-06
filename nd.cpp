#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <conio.h>
#include <utpp/utpp.h>

bool check(char* num, int digits)
{
  long long sum1 = 0;
  for (int i = 0; i < digits ; i++)
    sum1 = sum1*10 + num[i];

  long long sum2 = num[digits-1];
  for (int i = 0; i < digits-1; i++)
    sum2 = sum2 * 10 + num[i];

  return sum2 == 2*sum1;
}

void inc(char* num, int digits)
{
  digits--;
  do {
    num[digits] = (num[digits] + 1) % 10;
  } while (num[digits] == 0 && --digits >= 0);
}

void print(char* num, int digits)
{
  for (int i = 0; i < digits; i++)
    putch(num[i] + '0');
  putch('\r');
}

int main(int argc, char** argv)
{
  char num[20];
  int digits = 8;
  while (digits < 20)
  {
    memset (num, 0, sizeof(num));
    UnitTest::Timer t;
    t.Start();

    num[0] = 1;
    while (num[0] != 0)
    {
      if (check(num, digits))
      {
        std::cout << "stop" << std::endl;
        print(num, digits);
        check(num, digits);
        exit(0);
      }
      inc(num, digits);
    }
    double tsec = t.GetTimeInMs() / 1000.;
    std::cout << "Finished " << digits << " digits in " 
      << std::setprecision(3) << tsec << " sec" << std::endl;
    ++digits;
  }
  return 0;
}