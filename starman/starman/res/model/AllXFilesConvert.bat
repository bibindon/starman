@echo off
setlocal enabledelayedexpansion

rem MeshConvert.exe のパス（必要ならフルパスで）
set MESHCONVERT=MeshConvert.exe

for %%f in (*.x) do (
    echo Converting %%f to binary format...

    rem 出力を一時ファイルに保存
    %MESHCONVERT% /x /o "%%f.tmp" /y "%%f"

    if exist "%%f.tmp" (
        move /Y "%%f.tmp" "%%f" >nul
        echo Done: %%f
    ) else (
        echo Error converting %%f
    )
)

echo All conversions completed.
pause