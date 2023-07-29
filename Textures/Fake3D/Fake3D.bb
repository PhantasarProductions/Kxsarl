; This is an quick translation of a BlitzMax program to Blitz3D
; Why? Because the BlitzMax compiler refused to work on me, that's why!

;Rem
;/*
;	Fake 3D
;	
;	
;	
;	
;	(c) Jeroen P. Broks, 2015, All rights reserved
;	
;		This program is free software: you can redistribute it and/or modify
;		it under the terms of the GNU General Public License as published by
;		the Free Software Foundation, either version 3 of the License, or
;		(at your option) any later version.
;		
;		This program is distributed in the hope that it will be useful,
;		but WITHOUT ANY WARRANTY; without even the implied warranty of
;		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;		GNU General Public License for more details.
;		You should have received a copy of the GNU General Public License
;		along with this program.  If not, see <http://www.gnu.org/licenses/>.
;		
;	Exceptions to the standard GNU license are available with Jeroen's written permission given prior 
;	to the project the exceptions are needed for.
;*/
;
;
;Version: 15.02.11
;
;End Rem
;'Strict
;Import sidesign.minib3d
;Import tricky_units.FilePicker
;Import tricky_units.initfile
;Import tricky_units.MaxGUI_Input

Include "Hex2Int.bb"
Include "Args.bb"
;Include "Emp.bb"


DebugLog ArgCount+" parameters parsed"
If ArgCount=0
	RuntimeError "No args"
	End
EndIf

Global Project$=Args[0] ;Global Project$ = FilePicker("Pick a project","Projects") If Not project Return
Global Ready$=Args[7]

;Global I:TIni = New TIni
;I.Load("Projects/"+Project)

Global Png$ = Replace(Args[1],"/","\") ;= RequestFile("Pick a file","image file:png,jpg,tga,jpeg")
If Not png End


Global Author$ = Args[2] ;FilePicker("Pick an author",I.C("JCR"),2,1)
Global Output$ = Args[3] ; MaxGUI_Input$("Texture name string:",StripAll(png))

Global AlphaColor$ 
;Repeat
;alphacolor = MaxGUI_Input("Please enter the alpha color code:","000000")
AlphaColor = Left(Args[4]+"000000",6)
;Until Len(alphacolor)=6
;Global AlphaR = ("$"+Left (alphacolor,2  )).ToInt()
;Global AlphaG = ("$"+Mid  (Alphacolor,3,2)).ToInt()
;Global AlphaB = ("$"+Right(Alphacolor,2  )).toInt()
Global AlphaR = Hex2Int(Left (alphacolor,2  ))
Global AlphaG = Hex2Int(Mid  (Alphacolor,3,2))
Global AlphaB = Hex2Int(Right(Alphacolor,2  ))


; Removed. Better to do this by the manager program (which I'll write in C++
;Global CopyTo$ = I.C("TeddyBear")+Output+".png"
;Global CopyPix:TPixmap = LoadPixmap(png); CreateDir(ExtractDir(copyto),1) DebugLog "Saving: "+CopyTo; SavePixmapPNG(CopyPix,Copyto,9)


Global SaveTex$
Global Width%=Args[5]
Global Height%=Args[6]
If Not(Width And Height) Then RuntimeError("Width and Height not properly set")
;Graphics3D I.C("Width").ToInt(),I.C("Height").ToInt()
AppTitle "Fake3D"

Graphics3D Width,Height,32,2
;Delay 500
DebugLog "Loading: "+Png+" ("+FileType(Png)+")"
Global Tex=LoadTexture(PNG,1) ;Global Tex:ttexture = LoadTexture(Png)

;Tex=LoadTexture(PNG,2) ; 2nd try

DebugLog "=> "+Tex
If Not tex	
	TexPic = LoadImage(PNG)
	
	EndGraphics; 
	BT = WriteFile("Error")
	WriteLine BT,"Error! > "+"Too bad, I couldn't load that texture, sorry! "+Chr(10)+"("+Png+")" 
	WriteLine BT,"Maybe the format is not right? "+ImageWidth(TexPic)+"x"+ImageHeight(TexPic)
	CloseFile bt
	RuntimeError "Too bad, I couldn't load that texture, sorry! "+Chr(10)+"("+Png+")" 
	End
EndIf
	

If Not output End

;Rem
;
;Global Cam:tcamera = CreateCamera()
;'PositionEntity Cam,0,0,0
;Global Light:Tlight = CreateLight() '1,Cam)
;RotateEntity light,90,0,0
;
;Global Cube:TMesh = CreateCube()
;PositionEntity Cube,0,0,5
;RenderWorld
;Flip
;WaitKey
;End Rem

;Removed! I'll have to make ImageMagick do this
;Function MakeAlpha(Pixmap:TPixmap Var,Value$)
;Local X=0; 'EventX()
;Local Y=0; 'EventY()
;Local PY#,PX#
;Local Temp$;
;;Local Pixmap2:TPixmap = CreatePixmap(PixmapWidth(PixMap),PixmapHeight(PixMap),PF_BGRA8888)
;Local P$
;Local Pixel,PixHex$
;If value="" Then
;  If X>PixmapWidth(PixMap) Then Return
;  If Y>PixmapHeight(PixMap) Then Return
;  Pixel = ReadPixel(Pixmap,X,Y)
;  PixHex$ = Right(Hex(Pixel),6)
;  EndIf
;If value<>"" Then 
;	PixHex=Value
;	P = "$"+Value
;	Pixel = P.ToInt();
;	EndIf
;Local RPixel,RPixHex$
;'If Not Confirm("Turn color ($"+PixHex+") into Transparent?") Then Return
;'HideGadget Window
;'Local PC:TGadget = CreateWindow("Processing",200,200,600,25,Null,Window_Titlebar|Window_ClientCoords)
;'Local PB:TGadget = CreateProgBar(10,10,ClientWidth(PC)-20,15,PC)
;DebugLog "Turning color "+pixel+"/"+Hex(Pixel)+" into an alpha channel"
;For PY=0 To PixmapHeight(Pixmap)-1
;	PollEvent
;	'UpdateProgBar PB,PY/(PixmapHeight(PixMap)-1)
;	'DebugLog PY/(PixmapHeight(PixMap)-1)
;	'DebugLog "PY="+PY; Delay 10
;	For PX=0 To PixmapWidth(pixMap)-1
;		RPixel = ReadPixel(PixMap,Int(PX),Int(PY))
;		RPixHex = Right(Hex(RPixel),6)
;		If RPixHex = PixHex Then 
;			Temp="$00"+RPixHex
;			RPixel = Temp.ToInt()
;			'DebugLog "("+Int(PX)+","+Int(PY)+").Color = "+Hex(ReadPixel(PixMap,Px,PY))+" -> "+Hex(RPixel)
;			EndIf
;		WritePixel PixMap2,PX,PY,RPixel
;		If RPixel <> ReadPixel(PixMap2,PX,PY) Then DebugLog "Verify error: "+PX+","+PY
;		Next
;	Next
;'SavePixmapPNG PixMap2,"ProefDruk.png"
;'Image = LoadImage("ProefDruk.png");
;Pixmap=PixMap2
;'ShowGadget Window
;'FreeGadget PC
;End Function



camera=CreateCamera()
CameraClsColor camera,alphar,alphag,alphab

light=CreateLight()
RotateEntity light,90,0,0

; Create cube
cube=CreateCube()
EntityTexture cube,tex

Dim mix(4) ;Global mix[] = [1,0,1,2]
Mix(0)=1
Mix(1)=0
Mix(2)=1
Mix(3)=2

For akx=-6 To 6 Step 2
	x = Abs(akx)/2	
	For akz=(mix(x)*2) To 10 Step 2
		PositionEntity cube,akx,0,akz+.5
		; Rem
		;While Not KeyHit( key_escape )
		;RenderWorld
		;Flip
		;Wend
		; End Rem
		
		RenderWorld
		
		;shoot:TPixmap = GrabPixmap(0,0, I.C("Width").ToInt(), I.C("Height").ToInt())
		;If Not shoot
		;	Notify "Grab failed!"
		;	End
		;	EndIf
		shoot = CreateImage(Width,Height)
		GrabImage Shoot,0,0
		Flip	
		;MakeAlpha shoot,alphacolor	
		;Rem
		;SaveTex$ = I.C("JCR")+"/Author/"+I.C("InJCR")+Output
		;DebugLog "Saving: "+SaveTex+"/Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".png"
		;If Not CreateDir(ExtractDir(SaveTex),1) DebugLog "Creating dir: "+ExtractDir(SaveTex)+" FAILED!"
		;SavePixmapPNG(shoot,SaveTex+"/Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".png",9) 
		;If Not FileType(SaveTex+"/Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".png") DebugLog "SAVING FALED!"
		;EndRem

		;SaveTex$ = I.C("JCR")+"/"+Author+"/"+I.C("InJCR")+Output+"/Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".png"
		;SaveTex   = Args[8]   +"\"+Author+"\"+Args[9]     +Output+"\Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".bmp"
		;		SaveTex   = Args[8]   + Args[9]     +Output+"\Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".bmp"
		
		;? SaveTex=Args[2]+"\Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".bmp"
		SaveTex=Args[3]+"\Pic."+(Int(akx/2))+"."+(Int(akz)/2)+".bmp"



		DebugLog "Saving: "+SaveTex
		;If Not CreateDir(ExtractDir(SaveTex),1) DebugLog "Creating dir: "+ExtractDir(SaveTex)+" FAILED!"
		SaveImage shoot,SaveTex ;SavePixmapPNG(shoot,SaveTex,9) 
		If Not FileType(SaveTex) DebugLog "SAVING FALED!"
		Next
	Next
	
	If Ready
		BTO = WriteFile(Ready)
		WriteLine(BTO,"READY!")
		CloseFile BTO
	EndIf
End





