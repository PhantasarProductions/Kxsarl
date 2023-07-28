Global Args$[255]
Global ArgCount%

Function GetArgs(A$)
	Local i=0
	;Local A$=CommandLine$
	DebugLog "Chopping command parameters: "+A$
	If A=""
		ArgCount=0
		Return
	EndIf
	For j=1 To Len(A)
		If Mid(A,j,1)=","
			i=i+1
		Else
			Args[i]=Args[i]+Mid(A,j,1)
		EndIf 
	Next
	For j=0 To i
		DebugLog j+"/"+i+"> "+Args[j]
	Next
	ArgCount=i+1
End Function
GetArgs CommandLine()
	