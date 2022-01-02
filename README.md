# Sledge

[![Build status](https://ci.appveyor.com/api/projects/status/ka8ikyfdreboptdu?svg=true)](https://ci.appveyor.com/project/44lr/sledge)\
Sledge is an open source project that aims to bring full C# modding support to [Teardown](https://store.steampowered.com/app/1167630/Teardown/).

The current goal is to port all of the already available Lua functions to C#, then add new / custom ones on top. (i.e. Voxel loading, changing gl uniforms, loading custom shaders, etc)

You can download the latest build [here](https://github.com/44lr/sledge/releases).

Need help? Wanna know more about the project?
Join the [Discord server](https://www.discord.gg/SAAmJ3VSAS)

## Build requirements:
* [CMake](https://cmake.org/download/)
* [Visual Studio](https://visualstudio.microsoft.com/)
* C++ and C# SDKs for visual studio
* [.NET Desktop Runtime >6.0.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0)
* [Git](https://git-scm.com/downloads)
* [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) (3.0.1+, full version)

## How to build
1. Clone the repository somewhere using the following command:
``git clone --recurse-submodules https://github.com/44lr/sledge/``
2. Run "Create project.bat"
3. Open sledge.sln, which will now be located in the "proj" folder.
4. Click Build.
5. The built version of the project will now be located on proj/{configuration} (i.e. proj/Debug/)

## Creating a mod
1. Head on over to Sledge's install dir
2. Open a powershell window there
3. Run: ``& '.\mod.ps1' -c YourModName``
4. Type Y if you wanna use the project template, or N if you dont
5. Press enter

Options:  
- -c --create(projectName, projectTemplate(y/n)) - Creates a new project  
- -r --rename(projectName, newProjectName) - Renames a project  
- -d --delete(projectName) - Deletes a project  
- -b --build(projectName) - Builds a project  
