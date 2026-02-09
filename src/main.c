#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"
#include "io.h"
#include "app.h"

/* Allow clean exit with Ctrl+C */
static volatile sig_atomic_t g_running = 1;

static void on_sigint(int sig)
{
  (void)sig;
  g_running = 0;
}

int main(void)
{
  signal(SIGINT, on_sigint);

  printf("=== Embedded App Release 1 (Milestone 2) ===\n");
#if USE_FPGA_IO
  printf("Mode: REAL MMIO (/dev/mem)\n");
#else
  printf("Mode: STUB I/O (safe mode)\n");
#endif

  if (!io_init())
  {
    fprintf(stderr, "ERROR: io_init failed.\n");
    return 1;
  }

  app_state_t state;
  app_init(&state);

  while (g_running)
  {
    io_inputs_t in;
    io_read_inputs(&in);

    app_step(&state, &in);
  }

  io_close();
  printf("\nExiting cleanly.\n");
  return 0;
}
