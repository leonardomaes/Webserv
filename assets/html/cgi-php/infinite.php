#!/usr/bin/php-cgi

<?php
// 1. Try to disable PHP's internal time limit 
set_time_limit(0);

// 2. Send headers immediately so the server knows the script started
echo "Content-Type: text/html\r\n\r\n";
echo "<h1>Infinite Loop Test</h1>";
echo "<p>This script will run forever until the server kills it...</p>";

// 3. Force flush the output buffer to the server
flush();
ob_flush();

// 4. Infinite Loop
while (true) {
    // Sleep to avoid using 100% CPU while waiting
    sleep(1);
}
?>
