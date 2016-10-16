*****************************************************************************
*   Cartoonifier, for Desktop or Android.
*   by Shervin Emami, 2012 (shervin.emami@gmail.com)
*   http://www.shervinemami.info/
*****************************************************************************

This folder contains 2 Cartoonifier projects, performing the same task:
"Cartoonifier_Desktop": A desktop program (works on Windows, Mac, Linux, etc).
"Cartoonifier_Android": An Android app (requires Android 3.0 SDK or higher, and an Android 2.2 device or higher).

Cartoonifier_Android is a GUI wrapper, so it accesses some of the same C/C++ files in the Cartoonifier_Desktop folder:
    "cartoon.cpp" & "cartoon.h": all of the Cartoonifier image processing.
    "ImageUtils_v0.7.cpp" & "ImageUtils.h": useful functions for debugging OpenCV code.

Each project has code for its user interface and the project files in its own folder:

"Cartoonifier_Desktop" just uses the file "main_desktop.cpp" for its OpenCV user interface on a desktop.
It includes a Visual Studio 2008 project, an Eclipse CDT project, and a Makefile for any other compilers.

"Cartoonifier_Android" has an Android folder tree for its Android user interface, including:
    Java files in the "src" folder, 
    C/C++ NDK files in the "jni" folder,
    app resources in the "res" folder.
It includes an Eclipse project, that you can use for Android cross-development on Windows, Mac & Linux.

