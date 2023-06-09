<?php
declare(strict_types=1);

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    header('Location: error.php?status=405');
    exit();
}

require_once './include/utility/arrays.inc';
require_once './include/quizzes.inc';

$quizName = filter_input(INPUT_POST, 'quiz', FILTER_SANITIZE_FULL_SPECIAL_CHARS);
if ($quizName === null || $quizName === false || !Arrays::containsKey($quizzes, $quizName)) {
    header('Location: error.php?status=404');
    exit();
}
$quiz = $quizzes[$quizName];

$responses = filter_input(INPUT_POST, 'responses', FILTER_SANITIZE_FULL_SPECIAL_CHARS, FILTER_REQUIRE_ARRAY);
if ($responses === null || $responses === false) {
    header('Location: error.php?status=400');
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <link rel="stylesheet" href="/assets/stylesheets/global.css" />
</head>
<body>
    <?php include_once './content/header.php' ?>
    <main id="siteContent">
        <hgroup>
            <h2><?= $quiz->getTitle() ?></h2>
            <h3>Your Score: <?= intval($quiz->calculateGrade($responses) * 100) ?>%</h3>
        </hgroup>
        <a href="ask.php?quiz=<?= $quizName ?>">Take the quiz again</a>
        <form>
            <?php $quiz->render($responses); ?>
        </form>
    </main>
</body>
</html>