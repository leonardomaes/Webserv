//inlcude 42 header later

#pragma once

#include <csignal>

//global variable
extern volatile sig_atomic_t g_running;

//definition
void signal_handler(int signum);
