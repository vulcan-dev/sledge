$ProjectTemplate = "// Documentation: https://github.com/44lr/sledge/wiki/API-Documentation

using SledgeLib;

public class MOD_NAME : ISledgeMod
{
    public string GetName() { return `"MOD_NAME`"; }
    public string GetDescription() { return `"`"; }
    public string GetVersion() { return `"0.0.1`"; }
    public string GetAuthor() { return `"AUTHOR_NAME`"; }

    public void Load()
    {

    }

    public void Unload()
    {

    }
}"

$CSProjTemplate = "<Project Sdk=`"Microsoft.NET.Sdk`">
  <PropertyGroup>
    <TargetFramework>net6.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
    <Platforms>x64</Platforms>
    <PlatformTarget>x64</PlatformTarget>
    <ProduceReferenceAssembly>False</ProduceReferenceAssembly>
  </PropertyGroup>
  <ItemGroup>
    <Reference Include=`"sledgelib`">
      <HintPath>..\..\bin\sledgelib.dll</HintPath>
    </Reference>
  </ItemGroup>
  <Target Name=`"PostBuild`" AfterTargets=`"PostBuildEvent`">
    <Exec Command=`"copy /Y &quot;`$(TargetDir)`$(TargetName).dll&quot; &quot;`$(ProjectDir)..\`$(TargetName).dll&quot;&#xD;&#xA;copy /Y &quot;`$(TargetDir)`$(TargetName).pdb&quot; &quot;`$(ProjectDir)..\`$(TargetName).pdb&quot;&#xD;&#xA;`"/>
  </Target>
</Project>"

function CreateProject([string] $ProjectName) {
    if (!$ProjectName) {
        Write-Output "Missing project name";
        return;
    }

    Write-Output "Creating project: ${ProjectName}";
    
    # Create mod's folder
    if (-not (Test-Path -Path "./mods/${ProjectName}")) { New-Item "./mods/${ProjectName}/" -ItemType Directory | Out-Null};

    # Create mod's main script
    $CurrentModTemplate = $ProjectTemplate
    $CurrentModTemplate = $CurrentModTemplate.replace("MOD_NAME", "${ProjectName}");
    $CurrentModTemplate = $CurrentModTemplate.replace("AUTHOR_NAME", "${env:username}");
    New-Item -Path "./mods/${ProjectName}/" -Name "main.cs" -ItemType "file" -Force -Value ${CurrentModTemplate} | Out-Null;

    # Change location into the mods folder
    Set-Location "./mods/";

    # Create sln file for mod
    dotnet new sln --name ${ProjectName} --force;
    dotnet new classlib --output ${ProjectName} --framework net6.0 --no-restore --force;
    dotnet sln add .\${ProjectName}\${ProjectName}.csproj;

    # Create mod's csproj file
    New-Item -Path "./${ProjectName}/" -Name "${ProjectName}.csproj" -ItemType "file" -Force -Value ${CSProjTemplate} | Out-Null;

    # Delete Class1.cs if it exists
    if (Test-Path -Path "${ProjectName}\Class1.cs") { Remove-Item -Path "${ProjectName}\Class1.cs" };

    # Replace configuration platforms
    (Get-Content -Path "./${ProjectName}.sln").replace("Debug|Any CPU", "Debug|x64") | Set-Content -Path "./${ProjectName}.sln";
    (Get-Content -Path "./${ProjectName}.sln").replace("Release|Any CPU", "Release|x64") | Set-Content -Path "./${ProjectName}.sln";

    # Change location back to where the script was located
    Set-Location "../";

    Write-Output "Created project files for mod: ${ProjectName}";
}

function DeleteProject([string] $ProjectName) {
  # Check if the mod exists
  if (-not (Test-Path -Path "./mods/${ProjectName}")) {
    Write-Output("Mod ${ProjectName} does not exist");
    return;
  }

  # Prompt the user to confirm if they really wanna delete the mod
  $Option ="";
  while (($Option -ne "y") -and ($Option -ne "n")) {
    Write-Host -NoNewline "Are you sure you want to delete ${ProjectName}? (y/n)";
    $Option = Read-Host;
  }
  if ($Option -eq "n") { return; }

  # Delete the mod's folder and files 
  Remove-Item -Path "./mods/${ProjectName}" -Recurse -Force -Confirm:$false;
  Remove-Item -Path "./mods/${ProjectName}.*" -Force -Confirm:$false;

  # Check if the mod has a vs database, if so, delete that as well
  if (Test-Path -Path "./mods/.vs/${ProjectName}") { Remove-Item -Path "./mods/.vs/${ProjectName}" -Recurse -Force -Confirm:$false; }
}

function BuildProject([string] $ProjectName) {
  if (-not (Test-Path -Path "./mods/${ProjectName}/${ProjectName}.csproj")) {
    Write-Output("Mod not found");
  }

  $CSProjPath = Resolve-Path "./mods/${ProjectName}/${ProjectName}.csproj"

  "dotnet build `"${CSProjPath}`" /p:Configuration=Release /p:Platform=`"x64`"" | cmd
}

if ($args[0] -eq "--help" -or $args[0] -eq "-h" -or !$args[0]) {
    Write-Output "Usage: ./mod.ps1 [options]";
    Write-Output "Options:  -c --create(ProjectName)) - Creates a new project";
    Write-Output "          -d --delete(ProjectName) - Deletes a project";
    Write-Output "          -b --build(ProjectName) - Builds a project";
    exit;
} else {
    $Mode = $args[0];

    switch ($Mode) {
        "--create" { CreateProject($args[1]); exit 0; }
        "-c" { CreateProject($args[1]); exit 0; }
        "--delete" { DeleteProject($args[1]); exit 0; }
        "-d" { DeleteProject($args[1]); exit 0; }
        "--build" { BuildProject($args[1]); exit 0; }
        "-b" { BuildProject($args[1]); exit 0; }
        default { Write-Output "Unknown mode: ${Mode}"; exit 0; }
    }
}