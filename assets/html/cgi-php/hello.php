#!/usr/bin/php-cgi
<?php
    echo "Content-Type: text/html\r\n\r\n";
    echo "<h1>Hello from PHP!</h1>";
    echo "<p>Your IP address is: " . $_SERVER['REMOTE_ADDR'] . "</p>";
?>