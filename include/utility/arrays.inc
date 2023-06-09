<?php
declare(strict_types=1);

require_once './include/utility/utility.inc';

final class Arrays {
    public function __construct() {
        throw new BadFunctionCallException('Arrays is a static class');
    }

    private static function getIndex(array $array, int $position): int {
        return $position >= 0 ? $position : count($array) + $position;
    }

    public static function isEmpty(array $array) {
        return count($array) === 0;
    }

    public static function isList(array $array): bool {
        $index = 0;
        foreach ($array as $key => $value) {
            if ($index++ !== $key) {
                return false;
            }
        }
        return true;
    }

    public static function copy(array &$array, ?int $level = 1): array {
        $copy = [];
        foreach ($array as $key => $value) {
            if ($level === null || $level > 0) {
                $copy[$key] = is_array($value) ? Arrays::copy($value, $level !== null ? $level - 1 : null) : $value;
            } else {
                $copy[$key] = $value;
            }
        }
        return $copy;
    }

    public static function containsKey(array $array, mixed $containsKey): bool {
        foreach ($array as $key => $value) {
            if ($key === $containsKey) {
                return true;
            }
        }
        return false;
    }

    public static function containsValue(array $array, mixed $containsValue): bool {
        foreach ($array as $value) {
            if ($value === $containsValue) {
                return true;
            }
        }
        return false;
    }

    public static function keysOf(array $array): array {
        $keys = [];
        foreach ($array as $key => $value) {
            $keys[] = $key;
        }
        return $keys;
    }

    public static function valuesOf(array $array): array {
        $values = [];
        foreach ($array as $value) {
            $values[] = $value;
        }
        return $values;
    }

    public static function slice(array $array, int $fromPosition = 0, int $toPosition = -1): array {
        if (!Arrays::isList($array)) {
            throw new UnexpectedValueException('$array is not a list');
        }
        $slice = [];
        $fromIndex = Arrays::getIndex($array, $fromPosition);
        $toIndex = Arrays::getIndex($array, $toPosition);
        for ($index = $fromIndex; $index <= $toIndex; $index++) {
            $slice[] = $array[$index];
        }
        return $slice;
    }

    public static function tryGet(array $array, mixed $tryKey, mixed $getValue): mixed {
        return Arrays::containsKey($array, $tryKey) ? $array[$tryKey] : $getValue;
    }

    public static function trySet(array &$array, mixed $tryKey, mixed $setValue): bool {
        if (!Arrays::containsKey($array, $tryKey)) {
            $array[$tryKey] = $setValue;
            return true;
        }
        return false;
    }

    public static function push(array &$array, mixed ...$values): void {
        if (!Arrays::isList($array)) {
            throw new UnexpectedValueException('$array is not a list');
        }
        $array = [...$array, ...$values];
    }

    public static function enqueue(array &$array, mixed ...$values): void {
        if (!Arrays::isList($array)) {
            throw new UnexpectedValueException('$array is not a list');
        }
        $array = [...$values, ...$array];
    }

    public static function pop(array &$array): mixed {
        if (!Arrays::isList($array)) {
            throw new UnexpectedValueException('$array is not a list');
        }
        if (Arrays::isEmpty($array)) {
            throw new UnderflowException('$array is empty');
        }
        $value = $array[count($array) - 1];
        $array = Arrays::slice($array, 0, -2);
        return $value;
    }

    public static function dequeue(array &$array): mixed {
        if (!Arrays::isList($array)) {
            throw new UnexpectedValueException('$array is not a list');
        }
        if (Arrays::isEmpty($array)) {
            throw new UnderflowException('$array is empty');
        }
        $value = $array[0];
        $array = Arrays::slice($array, 1);
        return $value;
    }

    public static function shuffle(array &$array): void {
        $keys = Arrays::keysOf($array);
        for ($leftIndex = 0; $leftIndex < count($array); $leftIndex++) {
            $rightIndex = mt_rand(0, count($array) - 1);
            swap($array[$keys[$leftIndex]], $array[$keys[$rightIndex]]);
        }
    }
}

?>