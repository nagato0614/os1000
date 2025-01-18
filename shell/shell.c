//
// Created by toru on 2025/01/11.
//
#include "user.h"
#define DELETE_KEY 0x7f

int main(void)
{
  while (1)
  {
    prompt:
    printf("> ");
    char cmdline[128];
    for (int i = 0;; i++)
    {
      char ch = getchar();
      if (i == sizeof(cmdline) - 1)
      {
        printf("command too long\n");
        goto prompt;
      }
      else if (ch == '\r')
      {
        printf("\n");
        cmdline[i] = '\0';
        break;
      }
      else
      {
        // バックスペースの場合
        if (ch == DELETE_KEY)
        {
          if (i > 0)
          {
            i--;
            printf("\b \b");
          }
          continue;
        }

        cmdline[i] = ch;


        //カーソルを先頭に戻す
        printf("\r> ");
        for (int j = 0; j <= i; j++)
        {
          putchar(cmdline[j]);
        }
      }
    }
    if (strcmp(cmdline, "hello") == 0)
    {
      printf("Hello, World from Shell!\n");
    }
    else if (strcmp(cmdline, "exit") == 0)
    {
      exit();
    }
    else if (strcmp(cmdline, "readfile") == 0)
    {
      char buf[128];
      int len = readfile("hello.txt", buf, sizeof(buf));
      buf[len] = '\0';
      printf("%s\n", buf);
    }
    else if (strcmp(cmdline, "writefile") == 0)
    {
      writefile("hello.txt", "Hello from shell!\n", 19);
    }
    else
    {
      printf("unknown command: %s\n", cmdline);
    }
  }

  return 0;
}