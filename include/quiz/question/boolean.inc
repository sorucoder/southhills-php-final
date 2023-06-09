<?php
declare(strict_types=1);

require_once './include/quiz/quiz.inc';

final class BooleanQuestion extends Question {
    private bool $answer;
 
    public function __construct(string $prompt, bool $answer, float $points = 1.0) {
        parent::__construct($prompt, $answer, $points);
    }

    public function render(mixed $response = null): void {
        $name = $this->getName();
        $prompt = $this->getPrompt();

        $trueID = Strings::generateUniqueID('booleanQuestionChoiceTrue');
        $falseID = Strings::generateUniqueID('booleanQuestionChoiceFalse');
        ?>
        <fieldset id="<?= Strings::generateUniqueID('booleanQuestion') ?>">
            <legend><?= $prompt ?></legend>
            <p>True or false?</p>
            <?php if ($response === null): ?>
            <ol>
                <li>
                    <input
                        id="<?= $trueID ?>"
                        type="radio"
                        name="responses[<?= $name ?>]"
                        value="true"
                        required="required"
                    >
                    <label for="<?= $trueID ?>">True</label>
                </li>
                <li>
                    <input
                        id="<?= $falseID ?>"
                        type="radio"
                        name="responses[<?= $name ?>]"
                        value="false"
                        required="required"
                    >
                    <label for="<?= $falseID ?>">False</label>
                </li>
            </ol>
            <?php else:
                $answer = boolval($this->getAnswer());
                $response = boolval($response);
            ?>
            <ol>
                <li class="<?= $this->makeElementClass(['answer' => $answer, 'response' => $response]) ?>">
                    <input
                        id="<?= $trueID ?>"
                        type="radio"
                        name="responses[<?= $name ?>]"
                        value="true"
                        disabled="disabled"
                        checked="<?= $response ? 'checked' : '' ?>"
                    >
                    <label for="<?= $trueID ?>">True</label>
                </li>
                <li class="<?= $this->makeElementClass(['answer' => !$answer, 'response' => !$response]) ?>">
                    <input
                        id="<?= $falseID ?>"
                        type="radio"
                        name="responses[<?= $name ?>]"
                        value="false"
                        disabled="disabled"
                        checked="<?= !$response ? 'checked' : '' ?>"
                    >
                    <label for="<?= $falseID ?>">False</label>
                </li>
            </ol>
            <?php endif ?>
        </fieldset>
        <?php
    }
}

?>