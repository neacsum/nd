#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <conio.h>
#include <utpp/utpp.h>
#include <mlib/syncque.h>
#include <mlib/thread.h>

#define NUMTHREADS 6

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
    UnitTest::Timer t;
    t.Start();

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
    double tsec = t.GetTimeInMs() / 1000.;
    std::cout << "Finished job " << todo.start_digit << '-' << todo.digits 
      << " in " << std::setprecision(3) << tsec << " sec" << std::endl;
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

  int digits = 8;
  while (digits < 20)
  {
    for (int i = 1; i <= 9; ++i)
      work.produce({i, digits});
    ++digits;
  }
  return 0;
}