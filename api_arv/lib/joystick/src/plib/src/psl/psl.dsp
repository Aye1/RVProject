# Microsoft Developer Studio Project File - Name="psl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=psl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "psl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "psl.mak" CFG="psl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "psl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "psl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "psl"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "psl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy Library to plib directory
PostBuild_Cmds=copy release\*.lib ..\..\*.*	copy psl.h ..\..\psl.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "psl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "..\util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\psl_d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy Library to plib directory
PostBuild_Cmds=copy debug\*.lib ..\..\*.*	copy psl.h ..\..\psl.h
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "psl - Win32 Release"
# Name "psl - Win32 Debug"
# Begin Source File

SOURCE=.\pslCodeGen.cxx
# End Source File
# Begin Source File

SOURCE=.\pslCompiler.cxx
# End Source File
# Begin Source File

SOURCE=.\pslCompiler.h
# End Source File
# Begin Source File

SOURCE=.\pslContext.cxx
# End Source File
# Begin Source File

SOURCE=.\pslContext.h
# End Source File
# Begin Source File

SOURCE=.\psl.cxx
# End Source File
# Begin Source File

SOURCE=.\pslDump.cxx
# End Source File
# Begin Source File

SOURCE=.\pslError.cxx
# End Source File
# Begin Source File

SOURCE=.\pslExpression.cxx
# End Source File
# Begin Source File

SOURCE=.\pslFileIO.cxx
# End Source File
# Begin Source File

SOURCE=.\pslFileIO.h
# End Source File
# Begin Source File

SOURCE=.\psl.h
# End Source File
# Begin Source File

SOURCE=.\pslLocal.h
# End Source File
# Begin Source File

SOURCE=.\pslOpcodes.h
# End Source File
# Begin Source File

SOURCE=.\pslProgram.cxx
# End Source File
# Begin Source File

SOURCE=.\pslSymbol.h
# End Source File
# Begin Source File

SOURCE=.\pslSymbols.cxx
# End Source File
# Begin Source File

SOURCE=.\pslToken.cxx
# End Source File
# End Target
# End Project
