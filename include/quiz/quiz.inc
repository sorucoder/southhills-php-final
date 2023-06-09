<?php
declare(strict_types=1);

require_once './include/utility/strings.inc';

class Quiz {
    private string $name;
    private string $title;
    private array $questions;
    private array $order;

    public function __construct(string $name, string $title) {
        if (!Strings::test($name, '^(?:[a-z0-9]+_)*[a-z0-9]+$')) {
            throw new UnexpectedValueException('$name must be in lower snake case');
        }
        $this->name = $name;
        $this->title = $title;
        $this->questions = [];
        $this->order = [];
    }

    public function getName(): string {
        return $this->name;
    }

    public function getTitle(bool $asPlainText = false): string {
        if ($asPlainText) {
            return Strings::removeWithPattern($this->title, '<.+?>');
        }
        return $this->title;
    }

    public function getPoints(): float {
        $totalPoints = 0.0;
        foreach ($this->questions as $question) {
            $totalPoints += $question->getPoints();
        }
        return $totalPoints;
    }

    public function calculateScore(array $responses): float {
        $earnedPoints = 0.0;
        foreach ($this->order as $questionName) {
            $question = $this->questions[$questionName];
            $response = $responses[$questionName];
            $earnedPoints += $question->calculateScore($response);
        }
        return $earnedPoints;
    }

    public function calculateGrade(array $responses): float {
        return $this->calculateScore($responses) / $this->getPoints();
    }

    public function assign(string $questionName, Question $question): Quiz {
        if (!Strings::test($questionName, '^(?:[a-z0-9]+_)*[a-z0-9]+$')) {
            throw new UnexpectedValueException('$name must be in lower snake case');
        }
        
        $question->setName($questionName);
        $question->setQuiz($this);

        $this->order[] = $questionName;
        $this->questions[$questionName] = $question;

        return $this;
    }

    public function shuffle(): void {
        Arrays::shuffle($this->order);
    }

    public function render(?array $responses = null): void {
        if ($responses === null) {
            $this->shuffle();
            foreach ($this->order as $questionName) {
                $question = $this->questions[$questionName];
                $question->render();
            }
        } else {
            foreach ($this->order as $questionName) {
                $question = $this->questions[$questionName];
                $response = $responses[$questionName];
                $question->render($response);
            }
        }
    }
}

abstract class Question {
    private string $name;
    private string $prompt;
    private float $points;
    private mixed $answer;
    private ?Quiz $quiz;

    public function __construct(string $prompt, mixed $answer, float $points = 1.0) {
        $this->prompt = $prompt;
        $this->answer = $answer;
        $this->points = $points;
    }

    public function getQuiz(): ?Quiz {
        return $this->quiz;
    }

    public function setQuiz(?Quiz $quiz): void {
        $this->quiz = $quiz;
    }

    public function getName(): string {
        return $this->name;
    }

    public function setName(string $name): void {
        if (!Strings::test($name, '^(?:[a-z0-9]+_)*[a-z0-9]+$')) {
            throw new UnexpectedValueException('$name must be in lower snake case');
        }
        $this->name = $name;
    }

    public function getPrompt(): string {
        return $this->prompt;
    }

    public function getAnswer(): mixed {
        return $this->answer;
    }

    public function getPoints(): float {
        return $this->points;
    }

    protected function evaluate(mixed $response): float {
        return $response === $this->answer ? 1.0 : 0.0;
    }

    public function calculateScore(mixed $answer): float {
        return $this->points * $this->evaluate($answer);
    }

    abstract function render(mixed $response = null): void;

    protected function makeElementClass(?array $classes = null): string {
        if ($classes === null) {
            return '';
        }
        $classList = [];
        foreach ($classes as $key => $value) {
            if (is_integer($key) && is_string($value)) {
                $classList[] = $value;
            } else if (is_string($key) && is_bool($value)) {
                if ($value) {
                    $classList[] = $key;
                }
            } else {
                throw new DomainException('$classes contains an invalid class key-value pair');
            }
        }
        return Strings::join($classList, ' ');
    }
}
?>