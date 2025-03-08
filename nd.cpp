/*!
  Brute force attempt to find a Dan number.
*/
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <conio.h>
#include <utpp/utpp.h>
#include <mlib/syncque.h>
#include <mlib/thread.h>

#define NUMTHREADS 9

UnitTest::Timer chrono;


bool inline check(long long num, long long exp)
{
  int msd = (int)(num / exp);
  int lsd = num % 10;

  if (lsd < 2*msd)
    return false;

  long long swapped = num - (lsd - msd) + (lsd - msd) *exp;
  return swapped ==  2*num;
}

void print(char* num, int digits)
{
  for (int i = 0; i < digits; i++)
    putch(num[i] + '0');
  putch('\r');
}

struct task {
  int start_digit;
  int digits;
};

mlib::async_queue<task> work(NUMTHREADS);

int worker()
{
  while (1)
  {
    task todo;
    work.consume(todo);
    long long exp = (long long) pow(10, todo.digits-1);
    long long num = todo.start_digit * exp;
    long long lim = (todo.start_digit + 1) * exp;
    while (num < lim)
    {
      if (check(num, exp))
      {
        std::cout << "stop " << num << std::endl;
        exit(0);
      }
      ++num;
    }
    double tsec = chrono.GetTimeInMs() / 1000.;
    std::cout << "Finished job " << todo.start_digit << '-' << todo.digits 
      << " at " << std::setprecision(3) << std::setiosflags (std::ios::fixed) 
      << tsec << " sec" << std::endl;
  }
}

int main(int argc, char** argv)
{
  mlib::thread* workers[NUMTHREADS];

  for (int i = 0; i < NUMTHREADS; i++)
  {
    workers[i] = new mlib::thread(worker);
    workers[i]->start();
  }
  chrono.Start();

  int digits = 8;
  while (digits < 20)
  {
    for (int i = 1; i <= 9; ++i)
      work.produce({i, digits});
    ++digits;
  }
  return 0;
}