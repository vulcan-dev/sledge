# Sledge
Sledge is an open source project that aims to bring full C# modding support to [Teardown](https://store.steampowered.com/app/1167630/Teardown/).

The current goal is to port all of the already available Lua functions to C#, then add new / custom ones on top. (i.e. Voxel loading, changing gl uniforms, loading custom shaders, etc)

Need help? Wanna know more about the project?
Join the [Discord server](https://www.discord.gg/SAAmJ3VSAS)

## Build requirements:
* [CMake](https://cmake.org/download/)
* [Visual Studio](https://visualstudio.microsoft.com/)
* C++ and C# SDKs for visual studio
* [.NET Desktop Runtime >6.0.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0)
* [Git](https://git-scm.com/downloads)
* [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) (The full version, not the light version)

## How to build
1. Clone the repository somewhere using the following command:
``git clone --recurse-submodules https://github.com/44lr/sledge/``
2. Run "# 1 - create project.bat"
3. Open sledge.sln, which will now be located in the "proj" folder.
4. Click Build.
5. The built version of the project will now be located on proj/{configuration} (i.e. proj/Debug/)

## Creating a mod
1. Run: .\mod.ps1 -c ModName GenTemplate(y/n)

Options:  
- -c --create(projectName, projectTemplate(y/n)) - Creates a new project  
- -r --rename(projectName, newProjectName) - Renames a project  
- -d --delete(projectName) - Deletes a project  
- -b --build(projectName) - Builds a project  