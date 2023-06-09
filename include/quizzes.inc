<?php
declare(strict_types=1);

require_once './include/quiz/quiz.inc';
require_once './include/quiz/question/choice.inc';
require_once './include/quiz/question/boolean.inc';
require_once './include/quiz/question/word.inc';

$quizzes = [
    'ace_attorney_1' => (new Quiz('ace_attorney_1', '<cite>Phoenix Wright: Ace Attorney</cite>'))
        ->assign(
            'series_name',
            new ChoiceQuestion(
                'What is the Japanese name for the <cite>Ace Attorney</cite> series?',
                [
                    'gyakuten_saiban' => '逆転裁判 (<i>Gyakuten Saiban</i>)',
                    'boogie_woogie_innocence' => 'ブギーウギーの潔白 (<i>Boogie Woogie Innocence</i>)',
                    'naruhodou_kuns_screaming_trials' => '成歩堂の絶叫判決 (<i>Naruhodou-kun\'s Screaming Trials</i>)'
                ],
                'gyakuten_saiban'
            )
        )
        ->assign(
            'series_director',
            new ChoiceQuestion(
                'Who was the original director of the <cite>Ace Attorney</cite> series?',
                [
                    'shu_takumi' => '巧 舟 (<i>Shū Takumi</i>)',
                    'atsushi_inaba' => '稲葉 敦志 (<i>Atsushi Inaba</i>)',
                    'alexander_smith' => 'Alexander O. Smith',
                    'masakazu_sugimori' => '杉森 雅和 (<i>Masakazu Sugimori</i>)'
                ],
                'shu_takumi'
            )
        )
        ->assign(
            'edgeworth_name',
            new WordQuestion(
                'What is Prosecutor Edgeworth\'s first name?',
                'Miles'
            )
        )
        ->assign(
            'release_date',
            new BooleanQuestion(
                'The first ever release of <cite>Phoenix Wright: Ace Attorney</cite> was released in 2005.',
                false
            )
        )
        ->assign(
            'first_case',
            new ChoiceQuestion(
                'What is the name of the first case in <cite>Phoenix Wright: Ace Attorney</cite>?',
                [
                    'first_turnabout' => 'The First Turnabout',
                    'turnabout_samurai' => 'Turnabout Samurai',
                    'turnabout_sisters' => 'Turnabout Sisters',
                    'rise_from_the_ashes' => 'Rise from the Ashes'
                ],
                'first_turnabout'
            )
        )
];
?>