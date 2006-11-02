# Microsoft Developer Studio Project File - Name="MloDecoder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MloDecoder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MloDecoder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MloDecoder.mak" CFG="MloDecoder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MloDecoder - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MloDecoder - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MloDecoder - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "../../../../Source" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MloDecoder - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../../Source" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "MloDecoder - Win32 Release"
# Name "MloDecoder - Win32 Debug"
# Begin Group "Private"

# PROP Default_Filter ""
# Begin Group "Huffman"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbPair.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbPair.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbQuad.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbQuad.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbScaleFactor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHcbScaleFactor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHuffman.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloHuffman.h
# End Source File
# End Group
# Begin Group "IMDCT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Source\MloFft.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFft.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloImdct.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloImdct.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\Source\MloAdtsSamplingFreq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloAdtsSamplingFreq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementCce.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementCce.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementCpe.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementCpe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementDse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementDse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementFil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementFil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementPce.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementPce.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementSceLfe.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloElementSceLfe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFilterBank.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFilterBank.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFloatFix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFloatFpu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIcsInfo.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIcsInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIndivChnPool.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIndivChnPool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIndivChnStream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIndivChnStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloInvQuant.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloInvQuant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloIs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloMs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloMs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloPns.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloPns.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloScaleFactor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloScaleFactor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloSectionData.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloSectionData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloSyntacticElements.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloSyntacticElements.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloTns.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloTns.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\Source\Melo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloAdtsParser.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloAdtsParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloBitStream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloBitStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloDecoder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloResults.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloSampleBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloUtils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Source\MloUtils.h
# End Source File
# End Target
# End Project
