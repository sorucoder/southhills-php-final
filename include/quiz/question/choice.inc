<?php
declare(strict_types=1);

require_once './include/utility/arrays.inc';
require_once './include/utility/strings.inc';
require_once './include/quiz/quiz.inc';

class ChoiceQuestion extends Question {
    private array $choices;
    private array $order;

    public function __construct(string $prompt, array $choices, string $answer, float $points = 1.0) {
        if (!Arrays::containsKey($choices, $answer)) {
            throw new DomainException('$answer not a key in $choices');
        }

        parent::__construct($prompt, $answer, $points);
        $this->choices = $choices;
        $this->order = Arrays::keysOf($this->choices);
    }

    public function shuffle(): void {
        Arrays::shuffle($this->order);
    }

    public function render(mixed $response = null): void {
        $name = $this->getName();
        $prompt = $this->getPrompt();
        ?>
        <fieldset id="<?= Strings::generateUniqueID('choiceQuestion') ?>">
            <legend><?= $prompt ?></legend>
            <p>Please select the correct answer:</p>
            <?php if ($response === null): $this->shuffle(); ?>
            <ol>
                <?php
                foreach ($this->order as $index => $value):
                    $id = Strings::generateUniqueID("choiceQuestionChoice");
                    $choice = $this->choices[$value];
                ?>
                <li>
                    <input
                        id="<?= $id ?>"
                        type="radio"
                        name="responses[<?= $name ?>]"
                        value="<?= $value ?>"
                        required="required"
                    />
                    <label for="<?= $id ?>">
                        <?= $choice ?>
                    </label>
                </li>
                <?php endforeach ?>
            </ol>
            <?php else:
                $response = strval($response);
                $answer = strval($this->getAnswer());
            ?>
            <ol>
                <?php
                foreach ($this->order as $value):
                    $id = Strings::generateUniqueID();
                    $choice = $this->choices[$value];
                ?>
                <li class="<?= $this->makeElementClass(['answer' => $value === $answer, 'response' => $value === $response]) ?>">
                    <input
                        id="<?= $id ?>"
                        type="radio"
                        disabled="disabled"
                        <?php if ($value === $response): ?>checked="checked"<?php endif ?>
                    />
                    <label for="<?= $id ?>">
                        <?= $choice ?>
                    </label>
                </li>
                <?php endforeach ?>
            </ol>
            <?php endif ?>
        </fieldset>
        <?php
    }
}
?>