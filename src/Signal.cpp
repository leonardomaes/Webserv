/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 19:10:43 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/12/09 19:10:43 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Signal.hpp"

volatile sig_atomic_t g_running = 1;

void signal_handler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
        g_running = 0;
}