@echo off
mkdir proj
cd proj
cmake ../ -A x64 -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE=x64
cd ..
echo f | xcopy /y /f "./scripts/sledge.ChildProcessDbgSettings" "./proj/sledge.ChildProcessDbgSettings"
powershell -ExecutionPolicy Unrestricted -File "./scripts/patchproj.ps1" "./proj" -sln -vcx -cs -zc 
pause