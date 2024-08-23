strCommand = "cmd /c scrcpy.exe  "
Dim oShell,exeRs
Set oShell = CreateObject("WSCript.shell")
commandLine = "adb.exe devices"
Set exeRs = oShell.Exec(commandLine) 
outs = exeRs.StdOut.ReadAll()
strs= Split(outs,VbCrLf)
Dim dev
for i=1 to ubound(strs) 
	dev = Replace(strs(i), "device", "") 
	dev = Replace(dev, " ", "") 
	dev = Trim(dev) 
	If (Len(dev)) then
		v=MsgBox("scrcpy -s "&dev&"?",1)
		If ( v =1 ) then	
			strCommand = strCommand & " -s " & dev

' https://github.com/barry-ran/QtScrcpy/issues/692

			v=MsgBox("OMX.google.h264.encoder?",1)
			If ( v =1 ) then	
 				strCommand = strCommand & " --video-codec=h264 --video-encoder='OMX.google.h264.encoder'"
	                End If

			For Each Arg In WScript.Arguments
				strCommand = strCommand  & " """ & replace(Arg, """", """""""""") & """"
			Next
			CreateObject("Wscript.Shell").Run strCommand, 0, false
		End If
	End If
next
Set oShell = Nothing
Set exeRs = Nothing


' List of video encoders:
' --video-codec=h264 --video-encoder='OMX.rk.video_encoder.avc'
' --video-codec=h264 --video-encoder='c2.android.avc.encoder'
' --video-codec=h264 --video-encoder='OMX.google.h264.encoder'