# x264_encoder
Slightly Modified x264_encoder for Davinci Resolve (based on Black Magic's x264_encoder_plugin sample)

[WINDOWS]

To compile this under WINDOWS, you'll need the following tools installed

1) Visual Studio 2022
2) MSYS2

Instructions:

[Pre-Req]

Create Directory C:\VideoEditngUtils\x264_plugin_build

[Download x264]

1) run "MSYS2 MSYS" - type it in the search bar in the Windows Task Bar
2) pacman -Sy pacman
3) pacman -Syu
4) pacman -Su
5) pacman -S yasm
6) pacman -S nasm
7) pacman -S make
8) pacman -S git
9) cd /C/VideoEditingUtils/x264_plugin_build   
10) git clone https://code.videolan.org/videolan/x264

[Compile x264]

1) run the "x64 Native Tools Command Prompt for VS 2022" - type it in the seach bar in the Windows Task Bar
2) Inside the command prompt, type "cd /d C:\msys64"
3) Execute the command "msys2_shell.cmd -mingw64 -full-path"
4) This will open up another terminal, type in (in the new terminal)

> cd /C/VideoEditingUtils/x264_plugin/build/x264
> CC=cl ./configure --enable-shared
> make

[Download x264_encoder]

1) run "MSYS2 MSYS" - type it in the search bar in the Windows Task Bar
2) cd /C/VideoEditingUtils/x264_plugin_build
3) git clone https://github.com/UDaManFunks/x264_encoder

[Compile x264_encoder]

1) run the "x64 Native Tools Command Prompt for VS 2022" - type it in the seach bar in the Windows Task Bar
2) Inside the command prompt, type "cd /d C:\VideoEditingUtils\x264_plugin_build\x264_encoder"
3) nmake /f NMakefile
   
[Packaging / Installing]

1) Create folder called "IOPlugins" under %PROGRAMDATA%\Blackmagic Design\DaVinci Resolve\Support

  Note: you can open up %PROGRAMDATA% folder via explorer by typing it verbatim in a run window (Win + R) 

2) Create a folder named "x264_encoder.dvcp.bundle" under the IOPlugins folder
3) Create a folder named "Contents" under the "x264_encoder.dvcp.bundle" folder
4) Create a folder named "Win64" under the "Contents" folder
5) Copy the built plugin from C:\VideoEditingUtils\x264_plugin_build\x264_encoder\bin\x264_encoder.dvcp" and place it in the "Win64" folder (which you've created via Step 1-4)
6) Start Davinci Resolve Studio
   
You can export using X264 if you pick "QUICKTIME" or "MP4" as your FORMAT in Davnci Resolve, then selecting the "X264 (8-bit)" Codec option.

[LINUX]

TODO
