//inlcude 42 header later

#include "Signal.hpp"

volatile sig_atomic_t g_running = 0;

void signal_handler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
        g_running = 1;
}