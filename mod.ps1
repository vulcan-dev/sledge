function Create-LibCS([String] $projectName, [String] $projectTemplate) {
    $fileContents = ""

    if ($projectTemplate -eq "Y") {
        $fileContents = "// Documention: https://github.com/44lr/sledge/wiki/API-Documentation

using SledgeLib;

namespace $projectName {
    public class Mod {
        // Post Player Update
        private static dCallback cb_PostPlayerUpdateFunc = new dCallback(() => {
        
        });
    
        // Post Update
        private static dCallback cb_PostUpdateFunc = new dCallback(() => {
        
        });
    
        // Pre Player Update
        private static dCallback cb_PrePlayerUpdateFunc = new dCallback(() => {
            
        });
    
        // Pre Update
        private static dCallback cb_PreUpdateFunc = new dCallback(() => {
        
        });
    
        // Player Spawn
        private static dCallback cb_PlayerSpawnFunc = new dCallback(() => {
            Log.General(""Player Spawned"");
        });

        private static CCallback? cb_PrePlayerUpdate;
        private static CCallback? cb_PostPlayerUpdate;
        private static CCallback? cb_PreUpdate;
        private static CCallback? cb_PostUpdate;
        private static CCallback? cb_PlayerSpawn;
    
        public static void Init() {
            cb_PostPlayerUpdate = new CCallback(ECallbackType.PostPlayerUpdate, cb_PostPlayerUpdateFunc);
            cb_PostUpdate = new CCallback(ECallbackType.PostUpdate, cb_PostUpdateFunc);
            cb_PrePlayerUpdate = new CCallback(ECallbackType.PrePlayerUpdate, cb_PrePlayerUpdateFunc);
            cb_PreUpdate = new CCallback(ECallbackType.PreUpdate, cb_PrePlayerUpdateFunc);
            cb_PlayerSpawn = new CCallback(ECallbackType.PlayerSpawn, cb_PlayerSpawnFunc);

            Log.General(""$projectName Initialized"");
        }

        public static void Shutdown() {
            if (cb_PrePlayerUpdate != null) { cb_PrePlayerUpdate.Unregister(); cb_PrePlayerUpdate = null; }
            if (cb_PostPlayerUpdate != null) { cb_PostPlayerUpdate.Unregister(); cb_PostPlayerUpdate = null; }
            if (cb_PreUpdate != null) { cb_PreUpdate.Unregister(); cb_PreUpdate = null; }
            if (cb_PostUpdate != null) { cb_PostUpdate.Unregister(); cb_PostUpdate = null; }
            if (cb_PlayerSpawn != null) { cb_PlayerSpawn.Unregister(); cb_PlayerSpawn = null; }

            Log.General(""$projectName Shutdown"");
        }
    }
}"
    } else {
        $fileContents = "// Documention: https://github.com/44lr/sledge/wiki/API-Documentation

using SledgeLib;

namespace $projectName {
    public class Mod {
        public static void Init() {
            Log.General(""$projectName Initialized"");
        }

        public static void Shutdown() {
            Log.General(""$projectName Shutdown"");
        }
    }
}"
    }

    New-Item -Path . -Name "Lib.cs" -ItemType "file" -Force -Value $fileContents
}

function Create-CSProj([String] $projectName) {
    New-Item -Path . -Name "Lib.csproj" -ItemType "file" -Force -Value "<Project Sdk=""Microsoft.NET.Sdk"">
    <PropertyGroup>
       <TargetFramework>net6.0</TargetFramework>
       <AssemblyName>$projectName</AssemblyName>
       <ImplicitUsings>enable</ImplicitUsings>
       <Nullable>enable</Nullable>
       <Platforms>x64</Platforms>
       <ProduceReferenceAssembly>False</ProduceReferenceAssembly>
    </PropertyGroup>
    
    <Target Name=""PostBuild"" AfterTargets=""PostBuildEvent"">
        <Exec Command=""xcopy `$(ProjectDir)`$(OutDir)$projectName.dll ..\..\mods\$projectName /y"" />
    </Target>
    <ItemGroup>
       <Reference Include=""sledgelib"">
           <HintPath>..\..\mods\sledgelib.dll</HintPath>
       </Reference>
    </ItemGroup>
</Project>"
}

function Create-Project([String] $projectName, [String] $projectTemplate) {
    Write-Output "Creating Project: $projectName"

    New-Item -ItemType Directory -Force -Path $projectName
    if (-not(Test-Path -Path "./mods/$projectName" -PathType Container)) {
        New-Item -Path "./mods/$projectName" -ItemType Directory -Force
        Write-Output "Created mods directory"
    }

    Write-Output "Current Directory: $(Get-Location)"

    Set-Location $projectName

    dotnet new sln -n $projectName --force
    dotnet new classlib -o Lib -f net6.0 --no-restore --force
    dotnet sln add .\Lib\Lib.csproj

    Set-Location Lib

    if (Test-Path -Path "Class1.cs") {
        Remove-Item -Path "Class1.cs"
    }
 
    # create variable
    Create-LibCS $projectName $projectTemplate
    Create-CSProj $projectName

    # replace configuration platforms
    (Get-Content -Path "../$projectName.sln").replace("Debug|Any CPU", "Debug|x64") | Set-Content -Path "../$projectName.sln"
    (Get-Content -Path "../$projectName.sln").replace("Release|Any CPU", "Release|x64") | Set-Content -Path "../$projectName.sln"

    # Create Build.bat
    New-Item -Path ../ -Name "build.bat" -ItemType "file" -Force -Value "@echo off
dotnet build /p:Configuration=Release /p:Platform=""x64""
if /i ""%1"" equ ""-r"" (
    cd ..
    .\sledge.exe -nolauncher   
)"
}

function Rename-Project([String] $projectName, [String] $newName) {
    $newName  = $newName -replace(" ", "_")

    Write-Output "Renaming Project: $projectName to $newName"

    if (Test-Path -Path "$projectName/Lib/bin") {
        Remove-Item -Path "$projectName/Lib/bin" -Recurse
    }

    if (Test-Path -Path "$projectName/Lib/obj") {
        Remove-Item -Path "$projectName/Lib/obj" -Recurse
    }

    if (Test-Path -Path "./mods/$projectName") {
        Remove-Item -Path "./mods/$projectName/$projectName.dll"
    }

    try {
        Rename-Item -Path $projectName -NewName $newName -ErrorAction Stop
    } catch {
        Write-Output "Project Rename Failed. Maybe a process is using the project"
    }

    if (Test-Path -Path "./mods/$projectName" -PathType Container) {
        Remove-Item -Path "./mods/$projectName/*" -Recurse
    }

    if (Test-Path -Path "./mods/$projectName") {
        Rename-Item -Path ./mods/$projectName -NewName $newName
    }

    $files = Get-ChildItem -Path $newName -Recurse -File | ? { $_.Name -ne "build.bat" }
    foreach ($file in $files) {
        $fileName = $file.Name
        $fileName = $fileName -replace $projectName, $newName
        $filePath = $file.FullName
        Rename-Item -Path $filePath -NewName $fileName
    }

    $files = Get-ChildItem -Path $newName -Recurse -File | ? { $_.Name -ne "build.bat" }
    foreach ($file in $files) {
        $filePath = $file.FullName
        $fileContents = Get-Content $filePath
        $fileContents = $fileContents -replace $projectName, $newName
        Set-Content -Path $filePath -Value $fileContents
    }
}

if ($args[0] -eq "--help" -or $args[0] -eq "-h") {
    Write-Output "Usage: .\mod.ps1 [options(-create(projectName, projectTemplate), -rename(projectName), -delete(projectName))]"
    Write-Output "Options: -c --create(projectName, projectTemplate(y/n)) - Creates a new project"
    Write-Output "         -r --rename(projectName, newProjectName) - Renames a project"
    Write-Output "         -d --delete(projectName) - Deletes a project"
    Write-Output "         -b --build(projectName) - Builds a project"
    exit
} elseif ($args[0] -eq "--create" -or $args[0] -eq "-c") {
    # Get Project Name
    $projectName = $args[1]

    if ($null -eq $projectName) {
        Write-Host -NoNewline "Enter Project Name: "
        $projectName = Read-Host

        if ("" -eq $projectName) {
            Write-Host "No Project Name Specified"
            exit
        }
    }

    $projectName = $projectName -replace(" ", "_")

    # Project Template?
    $projectTemplate = $args[2]
    if ($null -eq $projectTemplate) {
        Write-Host -NoNewline "Project Template? [Y/N]: "
        $projectTemplate = Read-Host

        if ("" -eq $projectTemplate) {
            $projectTemplate = "Y"
        }

        if ("Y" -eq $projectTemplate) {
            $projectTemplate = "Y"
        } else {
            $projectTemplate = "N"
        }
    }

    Create-Project $projectName $projectTemplate
    Set-Location ..
    Write-Output (Get-Item .).FullName
    .\build.bat
    Set-Location ..
} elseif ($args[0] -eq "--rename" -or $args[0] -eq "-r") {
    # Get Project Name
    $projectName = $args[1]

    if ($null -eq $projectName) {
        Write-Host -NoNewline "Enter Project Name: "
        $projectName = Read-Host

        if ("" -eq $projectName) {
            Write-Host "No Project Name Specified"
            exit
        }
    }

    if (-not(Test-Path -Path $projectName)) {
        Write-Output "Project $projectName does not exist"
        return
    }

    $newName = $args[2]
    if ($null -eq $newName) {
        Write-Host -NoNewline "Enter new name: "
        $newName = Read-Host
        if ($null -eq $newName) {
            Write-Output "No name specified"
            return
        }
    }

    Rename-Project $projectName $newName
    if (Test-Path -Path $newName) {
        Set-Location $newName
        .\build.bat
        Set-Location ..
    }
} elseif ($args[0] -eq "--delete" -or $args[0] -eq "-d") {
    $projectName = $args[1]

    if ($null -eq $projectName) {
        Write-Host -NoNewline "Enter Project Name: "
        $projectName = Read-Host

        if ("" -eq $projectName) {
            Write-Host "No Project Name Specified"
            exit
        }
    }

    if (-not(Test-Path -Path $projectName)) {
        Write-Output "Project $projectName does not exist"
        return
    }

    Write-Output "Deleting Project: $projectName"

    if (Test-Path -Path "$projectName") {
        Remove-Item -Path "$projectName" -Recurse
    }

    if (Test-Path -Path "./mods/$projectName") {
        Remove-Item -Path "./mods/$projectName" -Recurse
    }
} elseif ($args[0] -eq "--build" -or $args[0] -eq "-b") {
    $projectName = $args[1]

    if ($null -eq $projectName) {
        Write-Host -NoNewline "Enter Project Name: "
        $projectName = Read-Host

        if ("" -eq $projectName) {
            Write-Host "No Project Name Specified"
            exit
        }
    }

    Set-Location $projectName
    .\build.bat
    Set-Location ..
} else {
    Write-Output "Usage: .\mod.ps1 [options(-create(projectName, projectTemplate), -rename(projectName), -delete(projectName))]"
    Write-Output "Options: -c --create(projectName, projectTemplate(y/n)) - Creates a new project"
    Write-Output "         -r --rename(projectName, newProjectName) - Renames a project"
    Write-Output "         -d --delete(projectName) - Deletes a project"
    Write-Output "         -b --build(projectName) - Builds a project"
    exit
}