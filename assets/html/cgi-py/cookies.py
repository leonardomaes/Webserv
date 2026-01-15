#!/usr/bin/python3
import os
import http.cookies

# 1. Read existing cookies
cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
count = 0
if "visit_count" in cookie:
    count = int(cookie["visit_count"].value)

# 2. Increment count
count += 1

# 3. Print Headers (Set-Cookie must be before the blank line)
print(f"Set-Cookie: visit_count={count}; Path=/")
print("Content-Type: text/html\r\n\r\n")

# 4. Body
print("<html><body>")
print(f"<h1>Cookie Test</h1>")
print(f"<p>You have visited this page <b>{count}</b> times.</p>")
print("<p>Reload the page to see the count increase.</p>")
print("</body></html>")
