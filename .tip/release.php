<?php
/*
 * Script for packaging chromium release mode.
 *
 * @author Ammar Faizi <ammarfaizi2@gmail.com>
 *
 */
const VERSION = "99.0.4828.1";

$argc = $_SERVER["argc"] ?? 0;
$argv = $_SERVER["argv"] ?? ["not_in_cli"];

if ($argc < 2) {
	printf("Usage: %s <release_dir>\n", $argv[0]);
	exit(0);
}

$fulldir = realpath($argv[1]);
if (!$fulldir) {
	printf("Cannot find realpath of %s\n", $argv[1]);
	exit(1);
}

$targetDir = date("Ymd")."_chromium_".VERSION;
if (!is_dir($targetDir)) {
	if (!mkdir($targetDir)) {
		printf("Error: Cannot create directory: %s", $targetDir);
		exit(1);
	}
}

$files = scandir($fulldir);
foreach ($files as $file) {
	if ($file === "." || $file === "..")
		continue;

	/*
	 * Do not copy irt_x64, obj and gen directory.
	 */
	if ($file === "irt_x64" || $file === "obj" || $file === "gen")
		continue;

	/*
	 * Do not copy pdb file (it's Windows debug file)
	 */
	$tmp = explode(".", $file);
	$tmp = end($tmp);
	if (strtolower($tmp) === "pdb")
		continue;

	$sfile = $fulldir."/".$file;
	$dfile = $targetDir."/".$file;
	if (is_dir($sfile)) {
		copy_dir($sfile, $dfile);
		continue;
	}
	copy($sfile, $dfile);
}

function copy_dir(string $src, string $dst): void
{
	if (!mkdir($dst)) {
		printf("Cannot create destination directory: %s", $dst);
		return;
	}

	$files = scandir($src);
	foreach ($files as $file) {
		if ($file === "." || $file === "..")
			continue;

		$sfile = $src."/".$file;
		$dfile = $dst."/".$file;
		if (is_dir($sfile)) {
			copy_dir($sfile, $dfile);
			continue;
		}
		copy($sfile, $dfile);
	}
}
