nvcc -c -o SinWave.cu.obj SinWave.cu
cl.exe /c /EHsc /IC:\glew-2.1.0\include /I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include" OGL.cpp
rc.exe GRWindow.rc
link.exe OGL.obj SinWave.cu.obj user32.lib gdi32.lib kernel32.lib GRWindow.res /LIBPATH:"C:\glew-2.1.0\lib\Release\x64" /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64" /MACHINE:x64 /SUBSYSTEM:WINDOWS
OGL.exe
