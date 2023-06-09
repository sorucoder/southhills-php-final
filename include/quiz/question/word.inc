<?php
declare(strict_types=1);

require_once './include/utility/strings.inc';
require_once './include/quiz/quiz.inc';

final class WordQuestion extends Question {
    public function __construct(string $prompt, string $answer, float $points = 1.0) {
        parent::__construct($prompt, $answer, $points);
    }

    public function evaluate(mixed $response): float {
        $response = Strings::toLowerCase(Strings::trimAny($response, ' '));
        $answer = Strings::toLowerCase(Strings::trimAny($this->getAnswer(), ' '));
        return $response === $answer ? 1.0 : 0.0;
    }

    public function render(mixed $response = null): void {
        $name = $this->getName();
        $prompt = $this->getPrompt();
        $answer = $this->getAnswer();

        $inputID = Strings::generateUniqueID('wordQuestionInput');
        ?>
        <fieldset id="<?= Strings::generateUniqueID('wordQuestion') ?>">
            <legend><?= $prompt ?></legend>
            <p>Please enter the correct answer.</p>
            <?php if ($response === null): ?>
            <input
                id="<?= $inputID ?>"
                type="text"
                width="<?= Strings::length($answer) ?>"
                name="responses[<?= $name ?>]"
                required="required"
            >
            <?php else:
                $response = strval($response);
                $answer = strval($this->getAnswer());
            ?>
            <input
                id="<?= $inputID ?>"
                class="<?= $this->makeElementClass(['response', 'answer' => $response === $answer]) ?>"
                type="text"
                width="<?= Strings::length($answer) ?>"
                name="responses[<?= $name ?>]"
                value="<?= $response ?>"
                disabled="disabled"
            >
            <?php if ($response !== $answer): ?>
            <label class="answer" for="<?= Strings::generateUniqueID() ?>">
                The correct answer was <em><?= $answer ?>.</em>
            </label>
            <?php endif ?>
            <?php endif ?>
        </fieldset>
        <?php
    }
}

?>