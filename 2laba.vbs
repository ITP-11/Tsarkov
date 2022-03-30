'* Имя: 2laba.vbs

'* Язык: VBScript

'* Описание: лабораторная работа №2

'**********************************************************************************

Dim s, s1, FSO, file, F, oFSO 
' Выводим строку на экран
do
WScript.StdOut.WriteLine "Menu"
WScript.StdOut.WriteLine "----------------------------"
WScript.StdOut.WriteLine "1. Informatia ob avtore"
WScript.StdOut.WriteLine "2. Pereimenovanie"
WScript.StdOut.WriteLine "3. Soxranenie v bloknote dati sozdania i razmera zadannogo faila"
WScript.StdOut.WriteLine "4. Exit"
WScript.StdOut.Write "Choose a task:"
' Считываем строку
s = WScript.StdIn.ReadLine
' Создаем объект WshShell
Set WshShell = WScript.CreateObject("WScript.Shell")
if (s="1") Then
WScript.StdOut.WriteLine "Tsarkov Vladislav Pavlovich, ITP - 11"
elseif (s="2") Then
    WScript.StdOut.WriteLine "Vvedite put k failu"
    s = WScript.StdIn.ReadLine
    WScript.StdOut.WriteLine "Novoe nazvanie"
    s1 = WScript.StdIn.ReadLine
    Set FSO = CreateObject("Scripting.FileSystemObject")
    FSO.MoveFolder s, s1
elseif (s="3") Then
    WScript.StdOut.WriteLine "Vvedite put k failu"
    file = WScript.StdIn.ReadLine
    Set oFSO = WScript.CreateObject("Scripting.FileSystemObject")
    set file = oFSO.GetFile(WScript.ScriptFullName)
    Set FSO = WScript.CreateObject("Scripting.FileSystemObject")
    set F = FSO.CreateTextFile("data.txt", true)
    F.WriteLine "Data sozdania:" & file.DateCreated
    F.WriteLine "Razmer fila:" & file.Size
    F.Close
End if
loop until (s="4")

'************* Конец *********************************************