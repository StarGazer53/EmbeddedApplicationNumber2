#include "app.h"
#include "io.h"
#include "config.h"

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

static volatile sig_atomic_t g_run = 1;

static void on_sigint(int sig)
{
  (void)sig;
  g_run = 0;
}

int main(void)
{
  signal(SIGINT, on_sigint);

#if USE_FPGA_IO
  printf("=== Embedded App Release 2 (Milestone 3) ===\n");
  printf("Mode: FPGA MMIO (/dev/mem)\n");
#else
  printf("=== Embedded App Release 2 (Milestone 3) ===\n");
  printf("Mode: STUB I/O (safe mode)\n");
#endif

  if (!io_init())
  {
    printf("I/O init failed. Exiting.\n");
    return 1;
  }

  app_state_t app;
  app_init(&app);

  while (g_run)
  {
    io_inputs_t in;
    if (!io_read_inputs(&in))
    {
      printf("WARN: io_read_inputs failed (continuing)\n");
      continue;
    }

    app_step(&app, &in);
  }

  io_close();
  printf("Exiting cleanly.\n");
  return 0;
}