<?php
declare(strict_types=1);

$status = filter_input(INPUT_GET, 'status', FILTER_VALIDATE_INT);
if ($status === null || $status === false) {
    $status = 404;
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="refresh" content="5;url=index.php">
    <title>SoruCoder's Quiz Mania - Something went wrong</title>
</head>
<body>
    <main>
        <h2>Something went wrong...</h2>
        <img src="https://httpcats.com/<?= $status ?>.jpg" alt="HTTP Cat <?= $status ?>" width="300" height="256" />
        <p>Redirecting back to the landing page in 5 seconds...</p>
    </main>
</body>
</html>