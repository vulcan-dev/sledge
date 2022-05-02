# Sledge

<p>
    <a href="https://store.steampowered.com/app/1167630/Teardown/">
        <img src="https://img.shields.io/badge/Teardown-1.0.0-orange">
    </a>
    <a href="https://isocpp.org/">
        <img src="https://img.shields.io/badge/language-C%2B%2B20-blue.svg">
    </a>
    <a href="https://ci.appveyor.com/project/44lr/sledge">
        <img src="https://ci.appveyor.com/api/projects/status/ka8ikyfdreboptdu?svg=true">
    </a>
    <a href="https://discord.gg/SAAmJ3VSAS">
        <img src="https://img.shields.io/discord/919624550466064494?color=%235865F2&label=Discord">
    </a>
</p>


Sledge is an open source project that aims to bring full C# modding support to [Teardown](https://store.steampowered.com/app/1167630/Teardown/).

The current goal is to port all of the already available Lua functions to C#, then add new / custom ones on top. (i.e. Voxel loading, changing gl uniforms, loading custom shaders, etc).
It also features other neat things like Ultralight support.

You can download the latest build [here](https://github.com/44lr/sledge/releases).

## IMPORTANT
This project is still very early in development, so expect tons of API changes (and possibly crashes).

## Build requirements:
* [CMake >3.23](https://cmake.org/download/)
* [Visual Studio](https://visualstudio.microsoft.com/)
* [.NET Desktop Runtime >6.0.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0)
* [Git](https://git-scm.com/downloads)
* [OpenSSL 3.0.1+ - full version](https://slproweb.com/products/Win32OpenSSL.html)
* [Child Process Debugging Tool](https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool) (Not required, but recommended for debugging)

## How to build
1. Clone the repository somewhere using the following command:
``git clone --recurse-submodules https://github.com/44lr/sledge/``
2. Run "Create project.bat"
3. Open sledge.sln, which will now be located in the "proj" folder.
4. Click Build.
5. The built version of the project will now be located on proj/build/{configuration} (i.e. proj/build/Debug/)

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

## Special thanks to:
* [SK83RJOSH](https://github.com/SK83RJOSH)
* [Xorberax](https://github.com/ss-gnalvesteffer)
* [Nymda](https://github.com/nymda)
* [Nitro](https://github.com/BuilderHD)
* [Daniel](https://github.com/vulcan-dev)
* [Alexandar](https://github.com/alexandargyurov)

For your help, ideas and support