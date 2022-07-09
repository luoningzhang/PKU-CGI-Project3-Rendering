{
    files = {
        [[build\.objs\SoftRender\windows\x64\release\src\main.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\common\helperfunc.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\graphics\camera.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\graphics\framebuffer.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\graphics\globillum.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\graphics\image.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\graphics\scene.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\renderer\renderer.cpp.obj]],
        [[build\.objs\SoftRender\windows\x64\release\src\platforms\win32.cpp.obj]]
    },
    values = {
        [[D:\Software\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\HostX64\x64\link.exe]],
        {
            "-nologo",
            "-dynamicbase",
            "-nxcompat",
            "-machine:x64",
            "/opt:ref",
            "/opt:icf",
            "Gdi32.lib",
            "User32.lib"
        }
    }
}