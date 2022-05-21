
param(
    [Parameter (Mandatory=$false)]
    [String]$m,

    [Parameter (Mandatory=$false)]
    [String]$g,

    [Parameter (Mandatory=$false)]
    [String]$a
)

function cmake_config_auto {
    Write-Output "[cmake_config]Generator will be set automaticaly"
    Start-Process -FilePath "cmake" -ArgumentList '-S','./','-B','./ProgramRelease/build' -NoNewWindow -Wait
}

function cmake_config_auto_add([String]$Add) {
    Write-Output "[cmake_config]Generator will be set automaticaly"
    Write-Output "[cmake_config]Additional will be $Add"
    Start-Process -FilePath "cmake" -ArgumentList '-S','./','-B','./ProgramRelease/build',$Add -NoNewWindow -Wait
}

function cmake_config_with_gen([String]$Gen) {
    Write-Output "[cmake_config]Will use generator $Gen"
    Start-Process -FilePath "cmake" -ArgumentList '-S','./','-B','./ProgramRelease/build','-G',"`"$Gen`"" -NoNewWindow -Wait
}

function cmake_config_with_gen_add([String]$Gen, [String]$Add) {
    Write-Output "[cmake_config]Will use generator $Gen"
    Write-Output "[cmake_config]Additional will be $Add"
    Start-Process -FilePath "cmake" -ArgumentList '-S','./','-B','./ProgramRelease/build','-G',"`"$Gen`"",$Add -NoNewWindow -Wait
}

if ($m.Length -eq 0) {
    $m = "Debug"
}

Write-Output "[builder] Removing old release"
Remove-Item -LiteralPath ./ProgramRelease -Force -Recurse

Write-Output "[builder] Creating neccessary dirs"
new-item -Name ./ProgramRelease -ItemType directory
new-item -Name ./ProgramRelease\build -ItemType directory

Write-Output "[builder] Preparing cmake"
if ($g.Length -gt 0)
{
    if ($a.Length -gt 0) {
        cmake_config_with_gen_add $g $a
    } else {
        cmake_config_with_gen $g
    }
} else {    
    if ($a.Length -gt 0) {
        cmake_config_auto_add $a
    } else {
        cmake_config_auto
    }
}

Write-Output "[builder] Building"
Write-Output "[cmake_build] Will be built with mode $m"
Start-Process -FilePath "cmake" -ArgumentList '--build','./ProgramRelease/build','--config',"$m",'--target','ALL_BUILD' -NoNewWindow -Wait


Write-Output  "[builder] Copying files"
Write-Output  "[builder] Copying core"
Copy-Item -Path "./ProgramRelease/build/configurator/$m/Configurator.exe" -Destination "./ProgramRelease/"
Copy-Item -Path "./ProgramRelease/build/archiver/$m/archiver.dll" -Destination "./ProgramRelease/"
Copy-Item -Path "./ProgramRelease/build/external_libs/lib_lz4/$m/lz4.dll" -Destination "ProgramRelease/"

Write-Output "[builder] Copying patcher"
Copy-Item -Path "./patcher" -Destination "ProgramRelease/" -Recurse

Write-Output "[builder] Copying patcher dependencies"

Write-Output "[builder] Copying lz4"
Copy-Item -Path "./ProgramRelease/build/external_libs/lib_lz4/$m/lz4_static.lib" -Destination "ProgramRelease/patcher"

Write-Output "[builder] Copying patcher archiver"
Copy-Item -Path "./ProgramRelease/build/archiver/$m/archiver_static.lib" -Destination "ProgramRelease/patcher"

Write-Output "[builder] Copying headers"

new-item -Name ./ProgramRelease/archiver -ItemType directory
new-item -Name ./ProgramRelease/shared -ItemType directory
new-item -Name ./ProgramRelease/external_libs -ItemType directory
new-item -Name ./ProgramRelease/external_libs/lib_lz4 -ItemType directory

Copy-Item -Path "./archiver/*.h" -Destination "ProgramRelease/archiver" -Recurse
Copy-Item -Path "./external_libs/lib_lz4/*.h" -Destination "ProgramRelease/external_libs/lib_lz4" -Recurse
Copy-Item -Path "./shared" -Destination "ProgramRelease/shared" -Recurse

Write-Output "[builder] Clean up"
Remove-Item -LiteralPath ./ProgramRelease/build -Force -Recurse

Write-Output "[builder] Creating changelog.txt"
Start-Process -FilePath "git" -ArgumentList 'log','--pretty="%ai %aN - %s"' -NoNewWindow -Wait -RedirectStandardOutput './ProgramRelease/changelog.txt'
Remove-Item "[builder] All done"
