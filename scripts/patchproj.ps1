# Project patches to fix stuff that either CMake can't currently do
# or things i don't know how to get CMake to do? dunno, all i know is that this took more time to fix than it should have :(
# if there's a more elegant solution to this, please let me know

$RunSlnPatches = $false
$RunCSProjPatches = $false
$RunVCXProjPatch = $false
$RunZeroCheckPatch = $false

$CSharpProjPatch1 = " <Platforms>x64</Platforms>
    <PlatformTarget>x64</PlatformTarget>
    <ResolveNuGetPackages>false</ResolveNuGetPackages>
    <Nullable>enable</Nullable>
    "

$CSharpProjPatch2 = "  <Target Name=`"PostBuild`" AfterTargets=`"PostBuildEvent`">
    <Exec Command=`"mkdir &quot;`$(ProjectDir)build\`$(Configuration)\bin&quot; &gt; nul&#xD;&#xA;copy &quot;`$(TargetDir)sledgelib.*&quot; &quot;`$(ProjectDir)build\`$(Configuration)\bin&quot;`" />
  </Target>
"

$VCXProjPatch = "
  <PropertyGroup>
    <ResolveNugetPackages>false</ResolveNugetPackages>
  </PropertyGroup>"

$ZeroCheckPatch = "setlocal
powershell -ExecutionPolicy Unrestricted -File `"SCRIPT_PATH`" `"PROJ_PATH`" -sln -vcx -cs -zc 
:cmEnd
endlocal &amp; call :cmErrorLevel %errorlevel% &amp; goto :cmDone
:cmErrorLevel
exit /b %1
:cmDone
if %errorlevel% neq 0 goto :VCEnd
"


# Projects are generated for Any CPU by default, this makes them x64
# ResolveNugetPackages to false, fixes an issue when building submodules
# Setting Nullable to true gets rid of a warning
# Setting ProduceReferenceAssembly to false makes it not create an extra dll
# This also adds a PostBuild command, which CMake currently does not support
function ApplyCSProjPatches([String] $ProjectPath) {
    Get-ChildItem "$ProjectPath" -Filter *.csproj | Foreach-Object {
        $CSharpProj = Get-Content -Raw $_.FullName;
         if (!($CSharpProj -like "*${CSharpProjPatch1}*")) {
            # first patch
            $PatchInsertPos = $CSharpProj.IndexOf("<OutputType>Library</OutputType>");

            $CSharpProjStart = $CSharpProj.Substring(0, $PatchInsertPos - 1);
            $CSharpProjEnd = $CSharpProj.Substring($PatchInsertPos);

            $CSharpProj = "$CSharpProjStart$CSharpProjPatch1$CSharpProjEnd";

            # second patch
            $PatchInsertPos = $CSharpProj.LastIndexOf("</Project>");
            
            $CSharpProjStart = $CSharpProj.Substring(0, $PatchInsertPos - 1);
            $CSharpProjEnd = $CSharpProj.Substring($PatchInsertPos);

            $CSharpProj = "$CSharpProjStart$CSharpProjPatch2$CSharpProjEnd";

            Out-File -FilePath $_.FullName -InputObject $CSharpProj;

            $FileName = $_.Name;
            Write-Output("--> Patched CSProj: ${FileName}");
         }
    }
    Write-Output("CSProj patches applied");
}

# ResolveNugetPackages being false fixes issues with submodules
function ApplyVCXProjPatches([String] $ProjectPath) {
    Get-ChildItem "$ProjectPath" -Filter *.vcxproj | Foreach-Object {
        $VCXProj = Get-Content -Raw $_.FullName;
        if (!($VCXProj -like "*${VCXProjPatch}*")) {
            $PatchInsertPos = $VCXProj.IndexOf("</PropertyGroup>") + "</PropertyGroup>".Length;

            $VCXProjStart = $VCXProj.Substring(0, $PatchInsertPos);
            $VCXProjEnd = $VCXProj.Substring($PatchInsertPos);

            $VCXProj = "$VCXProjStart$VCXProjPatch$VCXProjEnd";

            Out-File -FilePath $_.FullName -InputObject $VCXProj;

            $FileName = $_.Name;
            Write-Output("--> Patched VCXProj: ${FileName}");
        }
    }
    Write-Output("VCXProj patches applied");
}

# Project is generated for Any CPU, this changes it to x64
function ApplySolutionPatch([String] $ProjectPath) {
    $Solution = Get-Content -Raw -Path "$ProjectPath\\sledge.sln";
    $PatchedSolution = $Solution.Replace("Any CPU", "x64");
    Out-File -FilePath "$ProjectPath\\sledge.sln" -InputObject $PatchedSolution;

    Write-Output("Solution patch applied");
}

# Make patch run every time zero check is ran to prevent cmake from undoing our patches
function ApplyZeroCheckPatch([String] $ProjectPath) {
    $ZeroCheck = Get-Content -Raw -Path "$ProjectPath\\ZERO_CHECK.vcxproj";

    $ProjectPath = Resolve-Path $ProjectPath;
    $ScriptPath = "${PSScriptRoot}\patchproj.ps1";

    $ZeroCheckPatch = $ZeroCheckPatch.replace("PROJ_PATH", ${ProjectPath});
    $ZeroCheckPatch = $ZeroCheckPatch.replace("SCRIPT_PATH", ${ScriptPath});

    $HasMatch = $ZeroCheck -match "`".+`" -S.+ -B.+ --check-stamp-list .+ --vs-solution-file .+"
    
    if (!$HasMatch) {
        Write-Output("--> Error occurred while applying zero check patch (Regex failed?)");
        return;
    }
    $CMakeCommand = $Matches[0]

    if ($ZeroCheck -like "*powershell*") {
      Write-Output("zero check already patched");
      return;
    }

    while ($ZeroCheck.IndexOf($CMakeCommand) -ne -1) {
        $PatchPos  = $ZeroCheck.IndexOf($CMakeCommand) + $CMakeCommand.Length + $CommandEnd.Length + 1;

        $PatchedOutput = "${PatchedOutput}$($ZeroCheck.Substring(0, $PatchPos))`n`n${ZeroCheckPatch}`n";

        $ZeroCheck = $ZeroCheck.Substring($PatchPos, $ZeroCheck.Length - $PatchPos);
    }

    $PatchedOutput = "${PatchedOutput}${ZeroCheck}"

    Out-File -FilePath "$ProjectPath\\ZERO_CHECK.vcxproj" -InputObject $PatchedOutput;
}

# Show help
function ShowInfo() {
    Write-Output("Usage: .\patchproj.ps1 [Path] [Options]")
    Write-Output("Options:")
    Write-Output("-sln  : Apply solution patches")
    Write-Output("-cs   : Apply csproj patches")
    Write-Output("-vcx  : Apply vcxproj patches")
    Write-Output("-zc  : Apply zero check patches")
    exit 0;
}

$ProjPath = $args[0];
if (!($args[0]) -or !(Test-Path -Path $ProjPath -PathType "Container")) {
    Write-Output("Invalid path: ${ProjPath}")
    exit 0;
}

foreach ($Arg in $args) {
    if ($Arg -Eq $args[0]) { continue; }
    switch ($Arg) {
        "-sln" { $RunSlnPatches = $true; break; }
        "-cs" { $RunCSProjPatches = $true; break; }
        "-vcx" { $RunVCXProjPatch = $true; break; }
        "-zc" { $RunZeroCheckPatch = $true; break; }
        
        "-h" { ShowInfo(0); exit 0; }
        "--h" { ShowInfo(0); exit 0; }
        "/?" { ShowInfo(0); exit 0; }
        default { Write-Output "Unknown argument: ${Arg}"; ShowInfo(0); }
    }
}

if ($RunSlnPatches) { Write-Output("* Applying solution patch"); ApplySolutionPatch($ProjPath); }
if ($RunCSProjPatches) { Write-Output("* Applying csproj patches"); ApplyCSProjPatches($ProjPath); }
if ($RunVCXProjPatch) { Write-Output("* Applying vcxproj patch"); ApplyVCXProjPatches($ProjPath); }
if ($RunZeroCheckPatch) { Write-Output("* Applying zero check patch"); ApplyZeroCheckPatch($ProjPath); }

if (!$RunSlnPatches -And !$RunCSProjPatches -And !$RunVCXProjPatch -And !$RunZeroCheckPatch ) { ShowInfo(0); }

Write-Output("Patched files at ${ProjPath}");