param(
        [string]$file = "*"
)
if ($map -eq "*") {
        echo "Listen to what?"
} else {
	E:/Projects/Applications/Slyvina/Apps/JCR6_Show/Exe/JCR6_Show.exe debug_jql/music.jql "$file"
}
