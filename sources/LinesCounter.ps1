
$posLines = 0
Get-ChildItem -Recurse -Include *.cpp, *.h | Get-Content | %{ $posLines++ }

$negLines = 0
Get-ChildItem -Path "Modules/" -Recurse -Include *.cpp, *.h | Get-Content | %{ $negLines++ }

echo ($posLines - $negLines)
