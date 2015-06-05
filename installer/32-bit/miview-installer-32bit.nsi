; miview.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;

;--------------------------------

; The name of the installer
Name "MIView 0.7 (build 53)"

; The file to write
OutFile "MIView-Setup-32bit.exe"

; The default installation directory
InstallDir $PROGRAMFILES\MIView

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\MIView" "Install_Dir"

LicenseText "License"
LicenseData "eula.txt"
;--------------------------------

; Pages

Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "MIView (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put files there
  File "MIView.exe"
  File "glut32.dll"
  File "gdcmcharls.dll"
  File "gdcmCommon.dll"
  File "gdcmDICT.dll"
  File "gdcmDSED.dll"
  File "gdcmexpat.dll"
  File "gdcmgetopt.dll"
  File "gdcmIOD.dll"
  File "gdcmjpeg8.dll"
  File "gdcmjpeg12.dll"
  File "gdcmjpeg16.dll"
  File "gdcmMSFF.dll"
  File "gdcmopenjpeg.dll"
  File "gdcmzlib.dll"
  File "miview.ico"
  File "WindowLevel.frag"
  File "RayTrace.frag"
  File "RayTraceSSD.frag"
  File "RayTrace.vert"
  File "Default.vert"
  File "colormaps.txt"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\MIView "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MIView" "DisplayName" "MIView"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MIView" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MIView" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MIView" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  ; back up old value of .opt
  !define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".dcm" ""
  StrCmp $1 "" "${Index}-NoBackup"
    StrCmp $1 "DICOMFile" "${Index}-NoBackup"
    WriteRegStr HKCR ".dcm" "backup_val" $1
  "${Index}-NoBackup:"
  WriteRegStr HKCR ".dcm" "" "DICOMFile"
  ReadRegStr $0 HKCR "DICOMFile" ""
  StrCmp $0 "" 0 "${Index}-Skip"
	WriteRegStr HKCR "DICOMFile" "" "DICOM File"
	WriteRegStr HKCR "DICOMFile\shell" "" "open"
	WriteRegStr HKCR "DICOMFile\DefaultIcon" "" "$INSTDIR\MIView.exe,0"
  "${Index}-Skip:"
  WriteRegStr HKCR "DICOMFile\shell\open\command" "" '"$INSTDIR\MIView.exe" "%1"'
  WriteRegStr HKCR "DICOMFile\shell\edit" "" "Edit DICOM File"
  WriteRegStr HKCR "DICOMFile\shell\edit\command" "" '"$INSTDIR\MIView.exe" "%1"'
 
  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
  !undef Index
  ; Rest of script

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\MIView"
  CreateShortCut "$SMPROGRAMS\MIView\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\MIView\MIView 32-bit.lnk" "$INSTDIR\MIView.exe" "" "$INSTDIR\MIView.exe" 0
  
SectionEnd

Section "Install MS VC++ 2010 Redistributables"

  File "vcredist_x86.exe"
  Exec '"$INSTDIR\vcredist_x86.exe" /q'

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MIView"
  DeleteRegKey HKLM SOFTWARE\MIView

  ; Remove files and uninstaller
  Delete $INSTDIR\MIView.exe
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\gdcm.dll
  Delete $INSTDIR\glut32.dll
  Delete $INSTDIR\gdcmcharls.dll
  Delete $INSTDIR\gdcmCommon.dll
  Delete $INSTDIR\gdcmDICT.dll
  Delete $INSTDIR\gdcmDSED.dll
  Delete $INSTDIR\gdcmexpat.dll
  Delete $INSTDIR\gdcmgetopt.dll
  Delete $INSTDIR\gdcmIOD.dll
  Delete $INSTDIR\gdcmjpeg8.dll
  Delete $INSTDIR\gdcmjpeg12.dll
  Delete $INSTDIR\gdcmjpeg16.dll
  Delete $INSTDIR\gdcmMSFF.dll
  Delete $INSTDIR\gdcmopenjpeg.dll
  Delete $INSTDIR\gdcmzlib.dll
  Delete $INSTDIR\libeay32.dll
  Delete $INSTDIR\miview.ico
  Delete $INSTDIR\MRI_brain.bmp
  Delete $INSTDIR\WindowLevel.frag
  Delete $INSTDIR\RayTrace.frag
  Delete $INSTDIR\RayTraceSSD.frag
  Delete $INSTDIR\RayTraceRed.frag
  Delete $INSTDIR\RayTrace.vert
  Delete $INSTDIR\Default.vert
  Delete $INSTDIR\logfile.log
  Delete $INSTDIR\colormaps.txt

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\MIView\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\MIView"
  RMDir "$INSTDIR"

SectionEnd
