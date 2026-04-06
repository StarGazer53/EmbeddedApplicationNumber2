#include "app.h"
#include "io.h"
#include "config.h"

#include <stdio.h>
#include <signal.h>

/* =========================================================
 * Smart Alarm Clock with FPGA Timer
 * Final Project Release - Milestone 5
 * ========================================================= */

static volatile sig_atomic_t g_run = 1;

/*
 * Handle Ctrl+C and request a clean shutdown.
 */
static void on_sigint(int sig)
{
  (void)sig;
  g_run = 0;
}

/*
 * Program entry point.
 */
int main(void)
{
  signal(SIGINT, on_sigint);

#if USE_FPGA_IO
  printf("=== Smart Alarm Clock Final Release (Milestone 5) ===\n");
  printf("Mode: FPGA MMIO (/dev/mem)\n");
#else
  printf("=== Smart Alarm Clock Final Release (Milestone 5) ===\n");
  printf("Mode: STUB I/O\n");
#endif

  if (!io_init())
  {
    printf("I/O initialization failed. Exiting.\n");
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
  printf("Application exited cleanly.\n");
  return 0;
}