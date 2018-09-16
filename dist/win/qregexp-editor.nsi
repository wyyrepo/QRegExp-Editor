; NSIS installer script for qregexp-editor
; --------------- Headers --------------
!include MUI2.nsh

; --------------- General --------------
CRCCheck force
XPStyle on
SetCompressor /FINAL /SOLID lzma

!define QT_DIR $%QTDIR%                       ; Qt Installation directory
!define MINGW_DIR $%MINGW%                    ; MinGW Installation directory
!define V $%VERSION%                          ; Program version
!define ARCH $%ARCH%                          ; Architecture 32 or 64

!define P "QRegExp-Editor"                    ; Program name
!define P_NORM "qregexp-editor"               ; Program name (normalized)
!define ROOT_DIR "..\.."                      ; Program root directory
!define BUILD_DIR "${ROOT_DIR}\build"         ; Build dir
!define ADD_REMOVE "Software\Microsoft\Windows\CurrentVersion\Uninstall\${P}"
!define PRODUCT_REG_KEY "${P}"

InstallDir "$PROGRAMFILES\${P}"               ; Default installation directory
Name "${P}"                                   ; Name displayed on installer
OutFile "setup-${P_NORM}-${V}-win${ARCH}.exe" ; Resulting installer filename
BrandingText /TRIMLEFT "${P_NORM}-${V}"
RequestExecutionLevel admin

; ----------- Icon and Bitmap ---------
;!define MUI_ICON install.ico                 ; TODO: find suitable icon
;!define MUI_UNICON uninstall.ico             ; TODO: find suitable icon

; -------------------------------------
!define MUI_ABORTWARNING

;------------- Language Selection Dialog Settings --------------
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${P}\${V}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;-------------- Install Pages -------------
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${ROOT_DIR}\COPYING
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
    ; These indented statements modify settings for MUI_PAGE_FINISH
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_RUN "$INSTDIR\bin\${P_NORM}.exe"
    !define MUI_FINISHPAGE_RUN_CHECKED
    !define MUI_FINISHPAGE_RUN_TEXT "Launch ${P}-${V}"
    !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
	!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

;-------------- Uninstall Pages -------------
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
	; These indented statements modify settings for MUI_UNPAGE_FINISH
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH

;--------------- Languages ---------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Esperanto"

; ------------- Reserve Files ---------------------
!insertmacro MUI_RESERVEFILE_LANGDLL

; ------------- Installer Functions ---------------
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function checkAlreadyInstalled
	; check for already installed instance
	ClearErrors
	ReadRegStr $R0 HKLM "SOFTWARE\${PRODUCT_REG_KEY}" "Version"
	StrCmp $R0 "" 0 +2
	Return
	MessageBox MB_YESNO|MB_ICONQUESTION "${P} version $R0 seems \
	to be already installed on your system.$\nWould you like to \
	proceed with the installation of version ${V}?$\n Beware! This \
	will uninstall the already installed instance first." IDYES UnInstall
	MessageBox MB_OK|MB_ICONEXCLAMATION "Installation Canceled!"
	Quit
	UnInstall:
        ClearErrors
        ReadRegStr $R0 HKLM "${ADD_REMOVE}" "UninstallString"
		DetailPrint "Uninstalling already installed instance first!"
        ExecWait '$R0 _?=$INSTDIR'
		IfErrors OnError 0
		Return
	OnError:
		MessageBox MB_OK|MB_ICONSTOP "Error While Uinstalling already \
		installed Software. Please uninstall it manually and start the \
		installer again."
		Quit
FunctionEnd

;-------------- Uninstaller Functions ------------
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

;-------------- Installer -------------------------
Section "" ; No components page, name is not important
Call checkAlreadyInstalled

SetOutPath $INSTDIR ; Set output path to the installation directory.
WriteUninstaller $INSTDIR\uninstall.exe ; Tell it where to put the uninstaller

; Readme, License etc.
File ${ROOT_DIR}\COPYING
File ${ROOT_DIR}\COPYING.html
File /oname=README.txt ${ROOT_DIR}\README

; Bin
SetOutPath $INSTDIR\bin
File ${BUILD_DIR}\src\${P_NORM}.exe
File ${QT_DIR}\bin\mingwm10.dll
File ${QT_DIR}\bin\libgcc_s_dw2-1.dll
File ${MINGW_DIR}\bin\libstdc++-6.dll
File ${QT_DIR}\bin\QtCore4.dll
File ${QT_DIR}\bin\QtGui4.dll

; Translations
SetOutPath $INSTDIR\share\${P_NORM}\translations
File /r ${BUILD_DIR}\*.qm

; Shortcuts 
CreateDirectory "$SMPROGRAMS\${P}"
CreateShortCut "$SMPROGRAMS\${P}\${P}.lnk" "$INSTDIR\bin\${P_NORM}.exe"
CreateShortCut "$SMPROGRAMS\${P}\uninstall.lnk" "$INSTDIR\uninstall.exe"

; Add version number to Registry
WriteRegStr HKLM "Software\${PRODUCT_REG_KEY}" "Version" "${V}"

; Add uninstall information to "Add/Remove Programs"
WriteRegStr HKLM ${ADD_REMOVE} "DisplayName" "${P}-${V}"
WriteRegStr HKLM ${ADD_REMOVE} "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
WriteRegStr HKLM ${ADD_REMOVE} "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
WriteRegStr HKLM ${ADD_REMOVE} "Version" "${V}"
SectionEnd

;------------ Uninstaller -------------
Section "uninstall"

; Bin
Delete $INSTDIR\bin\${P_NORM}.exe
Delete $INSTDIR\bin\mingwm10.dll
Delete $INSTDIR\bin\libgcc_s_dw2-1.dll
Delete $INSTDIR\bin\libstdc++-6.dll
Delete $INSTDIR\bin\QtCore4.dll
Delete $INSTDIR\bin\QtGui4.dll
RMDir  $INSTDIR\bin

; Translations
RMDir /r $INSTDIR\share\${P_NORM}\translations
RMDir $INSTDIR\share\${P_NORM}
RMDir $INSTDIR\share

; Readme, License etc.
Delete $INSTDIR\COPYING
Delete $INSTDIR\COPYING.html
Delete $INSTDIR\README.txt
Delete $INSTDIR\uninstall.exe
RMDir  $INSTDIR

; Removing shortcuts
Delete "$SMPROGRAMS\${P}\${P}.lnk"
Delete "$SMPROGRAMS\${P}\uninstall.lnk"
RMDir "$SMPROGRAMS\${P}"

; Remove Procut Registry Entries
DeleteRegKey HKLM "Software\${PRODUCT_REG_KEY}"

; Remove entry from "Add/Remove Programs"
DeleteRegKey HKLM ${ADD_REMOVE}

SectionEnd
