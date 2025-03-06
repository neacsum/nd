#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <conio.h>
#include <utpp/utpp.h>
#include <mlib/syncque.h>
#include <mlib/thread.h>

#define NUMTHREADS 10

UnitTest::Timer chrono;


bool check(char* num, int digits)
{
  if (num[0] >= num[digits - 1])
    return false;

  long long sum1 = 0;
  long long sum2 = num[digits - 1];
  for (int i = 0; i < digits; i++)
  {
    sum1 = sum1 * 10 + num[i];
    if (i < digits - 1)
      sum2 = sum2 * 10 + num[i];
  }
  return sum2 ==  2*sum1;
}

void inc(char* num, int digits)
{
  digits--;
  do {
    if (++num[digits] != 10)
      break;
    num[digits--] = 0;
  } while (digits >= 0);
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
  char num[20];
  while (1)
  {
    task todo;
    work.consume(todo);
    memset(num, 0, sizeof(num));

    num[0] = todo.start_digit;
    while (num[0] == todo.start_digit)
    {
      if (check(num, todo.digits))
      {
        std::cout << "stop" << std::endl;
        print(num, todo.digits);
        check(num, todo.digits);
        exit(0);
      }
      inc(num, todo.digits);
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