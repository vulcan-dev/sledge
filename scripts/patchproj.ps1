# Project patches to fix stuff that either CMake can't currently do
# or things i don't know how to get CMake to do

# If there's a more elegant solution to this, please let me know

# Projects are generated for Any CPU by default, this makes them x64
# ResolveNugetPackages to false, fixes an issue when building submodules
# Setting Nullable to true gets rid of a warning
# Setting ProduceReferenceAssembly to false makes it not create an extra dll
# this also adds a PostBuild command, which CMake currently does not support
$CSharpProjPatch1 = " <Platforms>x64</Platforms>
    <PlatformTarget>x64</PlatformTarget>
    <ResolveNuGetPackages>false</ResolveNuGetPackages>
    <Nullable>enable</Nullable>
    "

$CSharpProjPatch2 = "  <Target Name=`"PostBuild`" AfterTargets=`"PostBuildEvent`">
    <Exec Command=`"if not exist `$(ProjectDir)build\`$(Configuration)\bin mkdir `$(ProjectDir)build\`$(Configuration)\bin&#xD;&#xA;copy `$(TargetDir)sledgelib.* `$(ProjectDir)build\`$(Configuration)\bin`" />
  </Target>
"

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
$VCXProjPatch = "
  <PropertyGroup>
    <ResolveNugetPackages>false</ResolveNugetPackages>
  </PropertyGroup>"
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

if ($args[0]) {
    $ProjectPath = $args[0]

    Write-Output("Applying project patches at: ${ProjectPath}")

    Write-Output("* Applying csproj patches");
    ApplyCSProjPatches($ProjectPath)

    Write-Output("* Applying vcxproj patches");
    ApplyVCXProjPatches($ProjectPath)

     Write-Output("* Applying solution patch");
    ApplySolutionPatch($ProjectPath)

    Write-Output("All patches applied.")
} else {
     Write-Output("Missing arguments: Project path");
}