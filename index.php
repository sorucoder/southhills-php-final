<?php
declare(strict_types=1);

require_once './include/quizzes.inc';

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SoruCoder's Quiz Mania</title>
    <link rel="stylesheet" href="/assets/stylesheets/global.css" />
</head>
<body>
    <?php include_once './content/header.php' ?>
    <main id="siteContent">
        <h2>Available Quizzes:</h2>
        <nav>
            <ul>
                <?php foreach ($quizzes as $quiz): ?>
                <li>
                    <a href="/ask.php?quiz=<?= $quiz->getName() ?>">Take the <?= $quiz->getTitle() ?> quiz!</a>
                </li>
                <?php endforeach ?>
            </ul>
        </nav>
    </main>
</body>
</html>