del		.\FlyCapture2_EX.exe
copy		.\FlyCapture2_EX\release\FlyCapture2_EX.exe	.\FlyCapture2_EX.exe
rd /s /q 	.\FlyCapture2_EX\debug
rd /s /q 	.\FlyCapture2_EX\release
rd /s /q 	.\FlyCapture2_EX\x64
del 		.\FlyCapture2_EX\FlyCapture2_EX.ncb
del 		.\FlyCapture2_EX\FlyCapture2_EX.aps
del		.\FlyCapture2_EX\FlyCapture2_EX.VC.db
pause