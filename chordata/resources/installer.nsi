; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CLAM-Chordata"
!define PRODUCT_VERSION "${VERSION}"
!define PRODUCT_PUBLISHER "CLAM devel"
!define PRODUCT_WEB_SITE "http://clam-project.org"
!define PRODUCT_HELP "http://clam-project.org/wiki/Chordata_tutorial"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\chordata.exe"
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
   StrCpy $INSTDIR "$PROGRAMFILES\CLAM\Chordata"
FunctionEnd

Section "Principal" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "..\chordata.exe"
  ; TODO: readd the boca task manager
;  File "..\SimacServicesClient\dist\BocaTaskManager.exe"
;  File "..\SimacServicesClient\dist\library.zip"
;  File "..\SimacServicesClient\dist\w9xpopen.exe"
  File '${QTDIR}\bin\QtCore4.dll'
  File '${QTDIR}\bin\QtGui4.dll'
  File '${QTDIR}\bin\QtOpenGL4.dll'
  File '${QTDIR}\bin\QtXml4.dll'
  File '${QTDIR}\bin\QtSvg4.dll'
;  File '${QTDIR}\bin\QtNetwork4.dll'
;  File '${QTDIR}\bin\QtDesigner4.dll'
;  File '${QTDIR}\bin\QtDesignerComponents4.dll'
;  File '${QTDIR}\bin\QtAssistantClient4.dll'
  File '${CLAMINSTALLDIR}\bin\mingwm10.dll'
  File '${CLAMINSTALLDIR}\bin\clam_audioio.dll'
  File '${CLAMINSTALLDIR}\bin\clam_core.dll'
  File '${CLAMINSTALLDIR}\bin\clam_processing.dll'
  File '${CLAMINSTALLDIR}\bin\clam_qtmonitors.dll'
  File '${EXTERNALDLLDIR}\libogg-0.dll'
  File '${EXTERNALDLLDIR}\libsndfile-1.dll'
  File '${EXTERNALDLLDIR}\libvorbis-0.dll'
;  File '${EXTERNALDLLDIR}\libvorbisenc-2.dll'
;  File '${EXTERNALDLLDIR}\libvorbisfile-3.dll'
  File '${EXTERNALDLLDIR}\portaudio.dll'
  File '${EXTERNALDLLDIR}\pthreadGC2.dll'
  File '${EXTERNALDLLDIR}\libxerces-c2_8_0.dll'
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
  SetOutPath "$INSTDIR\bin\iconengines"
  File '${QTDIR}\plugins\iconengines\qsvgicon4.dll'
  SetOutPath "$INSTDIR\share\chordata\i18n"
;  File "..\src\i18n\Chordata_ca.qm"
;  File "..\src\i18n\Chordata_es.qm"

  CreateDirectory "$SMPROGRAMS\CLAM\Chordata"
  CreateShortCut "$SMPROGRAMS\CLAM\Chordata\Chordata.lnk" "$INSTDIR\bin\chordata.exe"
  CreateShortCut "$DESKTOP\Chordata.lnk" "$INSTDIR\bin\chordata.exe"


SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\CLAM\Chordata\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\CLAM\Chordata\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\chordata.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\chordata.exe"
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
  Delete "$DESKTOP\Chordata.lnk"
  Delete "$SMPROGRAMS\CLAM\Chordata\Uninstall.lnk"
  Delete "$SMPROGRAMS\CLAM\Chordata\Website.lnk"
  Delete "$SMPROGRAMS\CLAM\Chordata\Chordata.lnk"
  RMDir "$SMPROGRAMS\CLAM\Chordata"
  RMDir "$SMPROGRAMS\CLAM"

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  Delete "$INSTDIR\share\chordata\i18n\*"
  RMDir "$INSTDIR\share\chordata\i18n"
  RMDir "$INSTDIR\share\chordata"
  RMDir "$INSTDIR\share"
  Delete "${INSTDIR}\plugins\iconengines\*"
  RMDir "$INSTDIR\bin\iconengines"
  Delete "$INSTDIR\bin\*"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR"

  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
