<?php
declare(strict_types=1);

if ($_SERVER['REQUEST_METHOD'] !== 'GET') {
    header('Location: error.php?status=405');
    exit();
}

require_once './include/utility/arrays.inc';
require_once './include/quizzes.inc';

$quizName = filter_input(INPUT_GET, 'quiz', FILTER_SANITIZE_FULL_SPECIAL_CHARS);
if ($quizName === null || $quizName === false || !Arrays::containsKey($quizzes, $quizName)) {
    header('Location: error.php?status=404');
    exit();
}
$quiz = $quizzes[$quizName];
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SoruCoder's Quiz Mania &mdash; <?= $quiz->getTitle(true) ?></title>
    <link rel="stylesheet" href="/assets/stylesheets/global.css" />
</head>
<body>
    <?php include_once './content/header.php' ?>
    <main id="siteContent">
        <h2><?= $quiz->getTitle() ?></h2>
        <form method="POST" action="results.php">
            <input type="hidden" name="quiz" value="<?= $quiz->getName() ?>" />
            <?php $quiz->render(); ?>
            <button type="submit">Submit Answers</button>
        </form>
    </main>
</body>
</html>