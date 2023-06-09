<?php
declare(strict_types=1);

final class Strings {
    private static string $searchInString = '';
    private static string $searchForString = '';
    private static int $searchIndex = 0;
    private static int $searchFromIndex = 0;
    private static int $searchToIndex = 0;

    private static string $patternConfiguration = 'pr';
    private static array $patternConfigurationTranslation = [
        'ignoreCase' => 'i',
        'extended'   => 'x',
        'multiline' => 'm',
        'findLongest' => 'l',
        'findEmpty' => 'n',
        'evaluate' => 'e',
        'flavor' => [
            'java' => 'j',
            'gnu' => 'u',
            'grep' => 'g',
            'emacs' => 'e',
            'ruby' => 'r',
            'perl' => 'z',
            'posix' => 'b',
        ]
    ];

    public function __construct() {
        throw new BadFunctionCallException('Strings is a static class');
    }

    // Pattern Configuration

    private static function pushPatternOptions(?array $options): void {
        if (empty($options)) {
            return;
        }

        $patternConfiguration = '';
        foreach ($options as $optionName => $optionValue) {
            if (key_exists($optionName, Strings::$patternConfigurationTranslation)) {
                $translation = Strings::$patternConfigurationTranslation[$optionName];
                if (is_array($translation)) {
                    if (!key_exists($optionValue, Strings::$patternConfigurationTranslation[$optionName])) {
                        throw new UnexpectedValueException('unexpected pattern option value ' . var_export($optionValue, true) . ' for option ' . var_export($optionName, true));
                    }
                    $translation = Strings::$patternConfigurationTranslation[$optionName][$optionValue];
                }
                $patternConfiguration .= $translation;
            }
        }

        Strings::$patternConfiguration = mb_regex_set_options($patternConfiguration);
    }

    private static function popPatternOptions(): void {
        mb_regex_set_options(Strings::$patternConfiguration);
    }

    private static function getIndex(string $string, int $position): int {
        return $position >= 0 ? $position : Strings::length($string) + $position;
    }

    private static function getCharacter(string $string, int $index): string {
        return mb_substr($string, $index, 1);
    }

    public static function length(string $string): int {
        return mb_strlen($string);
    }

    public static function slice(string $string, int $fromPosition = 0, int $toPosition = -1): string {
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);
        return mb_substr($string, $fromIndex, $toIndex - $fromIndex + 1);
    }

    public static function indexOf(string $string, string $ofSubstring, int $fromPosition = 0, int $toPosition = -1): ?int {
        $string = Strings::slice($string, $fromPosition, $toPosition);
        $index = mb_strpos($string, $ofSubstring);
        return $index ? $index + $fromPosition : null;
    }

    public static function lastIndexOf(string $string, string $ofSubstring, int $fromPosition = 0, int $toPosition = -1): ?int {
        $string = Strings::slice($string, $fromPosition, $toPosition);
        $index = mb_strrpos($string, $ofSubstring);
        return $index ? $index + $fromPosition : null;
    }

    public static function contains(string $string, string $containsSubstring, int $fromPosition = 0, int $toPosition = -1): bool {
        return Strings::indexOf($string, $containsSubstring, $fromPosition, $toPosition) !== null;
    }

    public static function containsAny(string $string, string $containsCharacters, int $fromPosition = 0, int $toPosition = -1): bool {
        foreach (Strings::charactersOf($string, $fromPosition, $toPosition) as $character) {
            if (Strings::contains($containsCharacters, $character)) {
                return true;
            }
        }
        return false;
    }

    public static function startsWith(string $string, string $startsWithSubstring, int $fromPosition = 0, int $toPosition = -1): bool {
        return Strings::indexOf($string, $startsWithSubstring, $fromPosition, $toPosition) === $fromPosition;
    }

    public static function endsWith(string $string, string $endsWithSubstring, int $fromPosition = 0, int $toPosition = -1): bool {
        return Strings::lastIndexOf($string, $endsWithSubstring, $fromPosition, $toPosition) === Strings::getIndex($string, $toPosition) - Strings::length($endsWithSubstring) + 1;
    }

    public static function characterAt(string $string, int $atPosition): string {
        return Strings::getCharacter($string, Strings::getIndex($string, $atPosition));
    }

    public static function charactersOf(string $string, int $fromPosition = 0, int $toPosition = -1): array {
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);
        $characters = [];
        for ($index = $fromIndex; $index <= $toIndex; $index++) {
            $characters[] = Strings::getCharacter($string, $index);
        }
        return $characters;
    }

    public static function split(string $string, ?string $withDelimiter = null, bool $ignoreEmptyMatches = true, int $fromPosition = 0, int $toPosition = -1): array {
        if ($withDelimiter !== null) {
            $splits = [];
            $fromIndex = Strings::getIndex($string, $fromPosition);
            $toIndex = Strings::getIndex($string, $toPosition);
            $splitFromIndex = $fromIndex;
            $splitToIndex = Strings::indexOf($string, $withDelimiter, $fromIndex, $toIndex);
            $lengthOfDelimiter = Strings::length($withDelimiter);
            while ($splitToIndex !== null) {
                $split = Strings::slice($string, $splitFromIndex, $splitToIndex - 1 <= $toIndex ? $splitToIndex - 1 : $toIndex);
                if (!$ignoreEmptyMatches || $split !== '') {
                    $splits[] = $split;
                }
                $splitFromIndex = $splitToIndex + $lengthOfDelimiter;
                $splitToIndex = Strings::indexOf($string, $withDelimiter, $splitFromIndex, $toIndex);
            }
            return $splits;
        }
        return Strings::splitWithPattern($string, '\p{Z}+');
    }

    public static function replace(string $string, string $replaceSubstring, string $withSubstring, int $fromPosition = 0, int $toPosition = -1): string {        
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);
        
        $preservedStart = $fromIndex > 0 ? Strings::slice($string, 0, $fromIndex - 1) : '';
        $replacedMiddle = Strings::slice($string, $fromIndex, $toIndex);
        $preservedEnd = $toIndex < Strings::length($string) - 1 ? Strings::slice($string, $toIndex + 1) : '';
        
        $indexOfReplacement = Strings::indexOf($replacedMiddle, $replaceSubstring);
        $lengthOfReplaced = Strings::length($replaceSubstring);
        $lengthOfReplacement = Strings::length($replaceSubstring);
        while ($indexOfReplacement !== null) {
            $replacedMiddle = Strings::slice($replacedMiddle, 0, $indexOfReplacement - 1) . $withSubstring . Strings::slice($replacedMiddle, $indexOfReplacement + $lengthOfReplaced);
            $indexOfReplacement = Strings::indexOf($replacedMiddle, $replaceSubstring, $indexOfReplacement + $lengthOfReplacement);
        }

        return $preservedStart . $replacedMiddle . $preservedEnd;
    }

    public static function splitWithPattern(string $string, string $withPattern, ?array $usingPatternOptions = null, int $fromPosition = 0, int $toPosition = -1): array {
        $string = Strings::slice($string, $fromPosition, $toPosition);

        Strings::pushPatternOptions($usingPatternOptions);
        $splits = mb_split($withPattern, $string);
        if ($usingPatternOptions !== null && key_exists('findEmpty', $usingPatternOptions) && $usingPatternOptions['findEmpty']) {
            $splits = array_filter($splits);
        }
        Strings::popPatternOptions();
        
        return $splits;
    }

    public static function replaceWithPattern(string $string, string $replacePattern, string $withPattern, ?array $usingPatternOptions = null, int $fromPosition = 0, int $toPosition = -1): string {
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);

        $preservedStart = $fromIndex > 0 ? Strings::slice($string, 0, $fromIndex - 1) : '';
        $replacedMiddle = Strings::slice($string, $fromIndex, $toIndex);
        $preservedEnd = $toIndex < Strings::length($string) - 1 ? Strings::slice($string, $toIndex + 1) : '';

        Strings::pushPatternOptions($usingPatternOptions);
        $replacedMiddle = mb_ereg_replace($replacePattern, $withPattern, $replacedMiddle);
        Strings::popPatternOptions();

        return $preservedStart . $replacedMiddle . $preservedEnd;
    }

    public static function join(array $words, string $seperator = ''): string {
        $count = count($words);
        $joined = '';
        foreach ($words as $index => $word) {
            $joined .= $index < $count - 1 ? $word . $seperator : $word;
        }
        return $joined;
    }

    public static function toLowerCase(string $string): string {
        return mb_convert_case($string, MB_CASE_LOWER);
    }

    public static function toUpperCase(string $string): string {
        return mb_convert_case($string, MB_CASE_UPPER);
    }

    public static function toTitleCase(string $string): string {
        return mb_convert_case($string, MB_CASE_TITLE);
    }

    public static function toLowerSnakeCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toLowerCase($word) . '_' : Strings::toLowerCase($word);
        }
        return $cased;
    }

    public static function toUpperSnakeCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toUpperCase($word) . '_' : Strings::toUpperCase($word);
        }
        return $cased;
    }

    public static function toTitleSnakeCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toTitleCase($word) . '_' : Strings::toTitleCase($word);
        }
        return $cased;
    }

    public static function toLowerKebabCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toLowerCase($word) . '-' : Strings::toLowerCase($word);
        }
        return $cased;
    }

    public static function toUpperKebabCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toUpperCase($word) . '-' : Strings::toUpperCase($word);
        }
        return $cased;
    }

    public static function toTitleKebabCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index < count($words) - 1 ? Strings::toTitleCase($word) . '-' : Strings::toTitleCase($word);
        }
        return $cased;
    }

    public static function toCamelCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= $index > 0 ? Strings::toTitleCase($word) : Strings::toLowerCase($word);
        }
        return $cased;
    }

    public static function toPascalCase(string ...$words): string {
        $cased = '';
        foreach ($words as $index => $word) {
            $cased .= Strings::toTitleCase($word);
        }
        return $cased;
    }

    public static function test(string $string, string $withPattern, ?array $usingPatternOptions = null, int $fromPosition = 0, int $toPosition = -1): bool {
        $string = Strings::slice($string, $fromPosition, $toPosition);
        Strings::pushPatternOptions($usingPatternOptions);
        $success = mb_ereg_match($withPattern, $string);
        Strings::popPatternOptions();
        return $success;
    }

    public static function match(string $string, string $withPattern, ?array $usingPatternOptions = null, int $fromPosition = 0, int $toPosition = -1): ?array {
        $string = Strings::slice($string, $fromPosition, $toPosition);
        Strings::pushPatternOptions($usingPatternOptions);
        $matches = [];
        $success = mb_ereg($withPattern, $string, $matches);
        if ($success) {
            array_shift($matches);
        }
        Strings::popPatternOptions();
        return $success ? $matches : null;
    }

    public static function removeAny(string $string, string $removeCharacters): string {
        $removed = '';
        foreach (Strings::charactersOf($string) as $character) {
            if (!Strings::containsAny($removeCharacters, $character)) {
                $removed .= $character;
            }
        }
        return $removed;
    }

    public static function remove(string $string, string $removeSubstring, int $fromPosition = 0, int $toPosition = -1): string {
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);

        $preservedStart = $fromIndex > 0 ? Strings::slice($string, 0, $fromIndex - 1) : '';
        $removedMiddle = Strings::slice($string, $fromIndex, $toIndex);
        $preservedEnd = $toIndex < Strings::length($string) - 1 ? Strings::slice($string, $toIndex + 1) : '';

        $indexOfRemoval = Strings::indexOf($removedMiddle, $removeSubstring);
        $lengthOfRemoval = Strings::length($removeSubstring);
        while ($indexOfRemoval !== null) {
            $removedMiddle = Strings::slice($removedMiddle, 0, $indexOfRemoval - 1) . Strings::slice($removedMiddle, $indexOfRemoval + $lengthOfRemoval);
            $indexOfRemoval = Strings::indexOf($removedMiddle, $removeSubstring, $indexOfRemoval);
        }

        return $preservedStart . $removedMiddle . $preservedEnd;
    }

    public static function removeWithPattern(string $string, string $removePattern, ?array $usingPatternOptions = null, int $fromPosition = 0, int $toPosition = -1): string {
        $fromIndex = Strings::getIndex($string, $fromPosition);
        $toIndex = Strings::getIndex($string, $toPosition);

        $preservedStart = $fromIndex > 0 ? Strings::slice($string, 0, $fromIndex - 1) : '';
        $removedMiddle = Strings::slice($string, $fromIndex, $toIndex);
        $preservedEnd = $toIndex < Strings::length($string) - 1 ? Strings::slice($string, $toIndex + 1) : '';

        Strings::pushPatternOptions($usingPatternOptions);
        $removedMiddle = mb_ereg_replace($removePattern, '', $removedMiddle);
        Strings::popPatternOptions();

        return $preservedStart . $removedMiddle . $preservedEnd;
    }

    public static function trimAny(string $string, string $trimCharacters = ''): string {
        $leftIndex = 0;
        while (Strings::containsAny(Strings::getCharacter($string, $leftIndex++), $trimCharacters));

        $rightIndex = Strings::length($string) - 1;
        while (Strings::containsAny(Strings::getCharacter($string, $rightIndex--), $trimCharacters));

        return Strings::slice($string, $leftIndex, $rightIndex);
    }

    public static function generateUniqueID(string $prefix = '', int $length = 16): string {
        $id = $prefix;
        for ($index = 0; $index < $length; $index++) {
            $id .= Strings::getCharacter('0123456789abcdef', mt_rand(0, 15));
        }
        return $id;
    }
}
?>