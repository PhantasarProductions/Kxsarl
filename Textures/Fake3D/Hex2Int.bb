; Simplistic, yet it does what it has to do
Function H2IP(A$)
	Select Upper(A)
		Case "1"
			Return 1
		Case "2"
			Return 2
		Case "3"
			Return 3
		Case "4"
			Return 4

		Case "5"
			Return 5
		Case "6"
			Return 6
		Case "7"
			Return 7
		Case "8"
			Return 8



		Case "9"
			Return 9
		Case "A"
			Return 10
		Case "B"
			Return 11
		Case "C"
			Return 12

		Case "D"
			Return 13
		Case "E"
			Return 14
		Case "F"
			Return 15
		Default
			Return 0
	End Select 
End Function


Function Hex2Int(A$)
	Return (H2IP(Left(A,1))*16)+(H2IP(Mid(A,2,1)))
End Function
