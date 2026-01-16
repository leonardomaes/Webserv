#!/usr/bin/php-cgi
<?php
// Security check: Only allow execution via web server
if (!getenv('REDIRECT_STATUS')) {
    http_response_code(403);
    exit;
}

// Get the requested filename from the query string (e.g., ?file=myphoto.jpg)
$filename = isset($_GET['file']) ? $_GET['file'] : '';
$baseUploadPath = '../upload/'; // Relative path to upload folder from cgi-php
$displayPath = '/upload/';         // Web path for the <img> tag

// Start HTML output
// echo "Content-Type: text/html\r\n\r\n";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>PHP Image Viewer</title>
    <style>
        body { font-family: Arial, sans-serif; background-color: #eef2f7; text-align: center; margin: 0; padding: 0; }
        h1 { color: #ffffff; margin: 0; }
        .header { background-color: #1f2937; padding: 25px; margin-bottom: 20px; }
        .container { background-color: #ffffff; width: 60%; margin: 0 auto; padding: 20px; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .image-box { margin: 20px 0; border: 1px solid #ddd; padding: 10px; display: inline-block; background: #fafafa; }
        img { max-width: 100%; height: auto; display: block; }
        a { text-decoration: none; color: #2563eb; font-weight: bold; }
        a:hover { text-decoration: underline; }
        ul { list-style: none; padding: 0; }
        li { margin: 8px 0; }
    </style>
</head>
<body>

    <div class="header">
        <h1>PHP Image Viewer</h1>
    </div>

    <div class="container">
        <?php
        if ($filename) {
            // Check if file exists (security: basename avoids directory traversal)
            $safeName = basename($filename);
            $filePath = $baseUploadPath . $safeName;

            if (file_exists($filePath)) {
                echo "<p>Displaying: <b>" . htmlspecialchars($safeName) . "</b></p>";
                echo "<div class='image-box'>";
                echo "<img src='" . $displayPath . htmlspecialchars($safeName) . "' alt='User Image'>";
                echo "</div>";
            } else {
                echo "<h3 style='color: #ef4444;'>Error: File Not Found</h3>";
                echo "<p>The file <code>" . htmlspecialchars($safeName) . "</code> does not exist in the upload folder.</p>";
            }
        } else {
            echo "<h3>Select an image to view</h3>";
        }
        ?>

        <hr style="margin: 30px 0; border: 0; border-top: 1px solid #eee;">

        <h3>Available Files in /upload:</h3>
        <?php
        if (is_dir($baseUploadPath)) {
            $files = scandir($baseUploadPath);
            echo "<ul>";
            $count = 0;
            foreach ($files as $f) {
                if ($f != "." && $f != ".." && $f != ".gitkeep") {
                    echo "<li><a href='?file=" . urlencode($f) . "'>📷 " . htmlspecialchars($f) . "</a></li>";
                    $count++;
                }
            }
            if ($count == 0) echo "<p><i>No files found. Try uploading one first!</i></p>";
            echo "</ul>";
        } else {
            echo "<p style='color: red;'>Error: Upload directory not found.</p>";
        }
        ?>

        <br>
        <a href="/index.html">⬅️ Back to Home</a>
    </div>

</body>
</html>
