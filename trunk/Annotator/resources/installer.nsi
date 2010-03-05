; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CLAM-Annotator"
!define PRODUCT_VERSION "${VERSION}"
!define PRODUCT_PUBLISHER "CLAM devel"
!define PRODUCT_WEB_SITE "http://clam-project.org"
!define PRODUCT_HELP "http://clam-project.org/wiki/Music_Annotator"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Annotator.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

!define ALL_USERS

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "HKLM"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "../COPYING"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Spanish"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\${PRODUCT_NAME}-${PRODUCT_VERSION}_setup.exe"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

XPStyle "On"


Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
   StrCpy $INSTDIR "$PROGRAMFILES\CLAM\Annotator"
FunctionEnd

Section "Principal" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "..\Annotator.exe"
  File "..\ClamExtractorExample.exe"
  File "..\ChordExtractor.exe"
  File "..\BocaClient.exe"
  ; TODO: readd the boca task manager
;  File "..\SimacServicesClient\dist\BocaTaskManager.exe"
;  File "..\SimacServicesClient\dist\library.zip"
;  File "..\SimacServicesClient\dist\w9xpopen.exe"
  File '${QTDIR}\bin\QtCore4.dll'
  File '${QTDIR}\bin\QtGui4.dll'
  File '${QTDIR}\bin\QtOpenGL4.dll'
;  File '${QTDIR}\bin\QtXml4.dll'
;  File '${QTDIR}\bin\QtSvg4.dll'
;  File '${QTDIR}\bin\QtNetwork4.dll'
;  File '${QTDIR}\bin\QtDesigner4.dll'
;  File '${QTDIR}\bin\QtDesignerComponents4.dll'
;  File '${QTDIR}\bin\QtAssistantClient4.dll'
  File '${CLAMINSTALLDIR}\bin\mingwm10.dll'
  File '${CLAMINSTALLDIR}\lib\clam_audioio.dll'
  File '${CLAMINSTALLDIR}\lib\clam_core.dll'
  File '${CLAMINSTALLDIR}\lib\clam_processing.dll'
  File '${EXTERNALDLLDIR}\libogg-0.dll'
  File '${EXTERNALDLLDIR}\libsndfile-1.dll'
  File '${EXTERNALDLLDIR}\libvorbis-0.dll'
;  File '${EXTERNALDLLDIR}\libvorbisenc-2.dll'
;  File '${EXTERNALDLLDIR}\libvorbisfile-3.dll'
  File '${EXTERNALDLLDIR}\portaudio.dll'
  File '${EXTERNALDLLDIR}\pthreadGC2.dll'
  File '${CLAMINSTALLDIR}\lib\libxerces-c2_8_0.dll'
  ; GTK libs
  File '${EXTERNALDLLDIR}\libxml++-2.6-2.dll'
  File '${EXTERNALDLLDIR}\libxml2.dll'
  File '${EXTERNALDLLDIR}\libglibmm-2.4-1.dll'
  File '${EXTERNALDLLDIR}\libsigc-2.0-0.dll'
  File '${EXTERNALDLLDIR}\libgobject-2.0-0.dll'
  File '${EXTERNALDLLDIR}\libglib-2.0-0.dll'
  File '${EXTERNALDLLDIR}\iconv.dll'
  File '${EXTERNALDLLDIR}\intl.dll'
  File '${EXTERNALDLLDIR}\zlib1.dll'
  File '${EXTERNALDLLDIR}\libgmodule-2.0-0.dll'
  SetOutPath "$INSTDIR\example-data\"
  File "..\example-data\CLAMDescriptors.sc"
  File "..\example-data\CLAMDescriptors.pro"
  File "..\example-data\Chords.sc"
  File "..\example-data\Chords.pro"
  SetOutPath "$INSTDIR\example-data\SongsTest"
  File "..\example-data\SongsTest\Debaser-WoodenHouse.mp3"
  File "..\example-data\SongsTest\Debaser-WoodenHouse.mp3.pool"
  File "..\example-data\SongsTest\Debaser-WoodenHouse.mp3.chords"
  File "..\example-data\SongsTest\Debaser-CoffeeSmell.mp3"
  File "..\example-data\SongsTest\Debaser-CoffeeSmell.mp3.pool"
  File "..\example-data\SongsTest\Debaser-CoffeeSmell.mp3.chords"
  File "..\example-data\SongsTest\urls.txt"
  SetOutPath "$INSTDIR\share\annotator\i18n"
  File "..\src\i18n\Annotator_ca.qm"
  File "..\src\i18n\Annotator_es.qm"

!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".task" ""
  StrCmp $1 "" "${Index}-NoBackup"
    StrCmp $1 "MusicAnnotator.BocaTask" "${Index}-NoBackup"
    WriteRegStr HKCR ".task" "backup_val" $1
"${Index}-NoBackup:"
  WriteRegStr HKCR ".task" "" "MusicAnnotator.BocaTask"
  ReadRegStr $0 HKCR "MusicAnnotator.BocaTask" ""
  StrCmp $0 "" 0 "${Index}-Skip"
	WriteRegStr HKCR "MusicAnnotator.BocaTask" "" "Barebone of Collective Annotations task"
	WriteRegStr HKCR "MusicAnnotator.BocaTask\shell" "" "open"
	WriteRegStr HKCR "MusicAnnotator.BocaTask\DefaultIcon" "" "$INSTDIR\bin\Annotator.exe,0"
"${Index}-Skip:"
  WriteRegStr HKCR "MusicAnnotator.BocaTask\shell\open\command" "" '$INSTDIR\bin\BocaClient.exe "%1"'
  WriteRegStr HKCR "MusicAnnotator.BocaTask\shell\edit" "" "Open task"
  WriteRegStr HKCR "MusicAnnotator.BocaTask\shell\edit\command" "" '$INSTDIR\bin\BocaClient.exe "%1"'
 
  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
!undef Index

  CreateDirectory "$SMPROGRAMS\CLAM\Annotator"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Annotator.lnk" "$INSTDIR\bin\Annotator.exe"
  CreateShortCut "$DESKTOP\Annotator.lnk" "$INSTDIR\bin\Annotator.exe"


SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\Annotator.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\ClamExtractorExample.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\ChordExtractor.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\Annotator.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\CLAMExtractorExample.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\ChordExtractor.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "La desinstalaci�n de $(^Name) finaliz� satisfactoriamente."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "�Est� completamente seguro que desea desinstalar $(^Name) junto con todos sus componentes?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$DESKTOP\Annotator.lnk"
  Delete "$SMPROGRAMS\CLAM\Annotator\Uninstall.lnk"
  Delete "$SMPROGRAMS\CLAM\Annotator\Website.lnk"
  Delete "$SMPROGRAMS\CLAM\Annotator\Annotator.lnk"
  RMDir "$SMPROGRAMS\CLAM\Annotator"
  RMDir "$SMPROGRAMS\CLAM"

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  Delete "$INSTDIR\example-data\SongsTest\*"
  RMDir "$INSTDIR\example-data\SongsTest" 
  Delete "$INSTDIR\example-data\*"
  RMDir "$INSTDIR\example-data"
  Delete "$INSTDIR\share\annotator\i18n\*"
  RMDir "$INSTDIR\share\annotator\i18n"
  RMDir "$INSTDIR\share\annotator"
  RMDir "$INSTDIR\share"
  Delete "$INSTDIR\bin\*"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR"

!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".task" ""
  StrCmp $1 "MusicAnnotator.BocaTask" 0 "${Index}-NoOwn" ; only do this if we own it
    ReadRegStr $1 HKCR ".task" "backup_val"
    StrCmp $1 "" 0 "${Index}-Restore" ; if backup="" then delete the whole key
      DeleteRegKey HKCR ".task"
    Goto "${Index}-NoOwn"
"${Index}-Restore:"
      WriteRegStr HKCR ".task" "" $1
      DeleteRegValue HKCR ".task" "backup_val"
   
    DeleteRegKey HKCR "MusicAnnotator.BocaTask" ;Delete key with association settings
 
    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
"${Index}-NoOwn:"
!undef Index

  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
