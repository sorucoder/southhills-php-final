<?php
declare(strict_types=1);

function swap(mixed &$left, mixed &$right): void {
    [$left, $right] = [$right, $left];
}

?>