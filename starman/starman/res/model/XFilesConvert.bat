@echo off
rem ------------------------------------------------------------
rem  XtoBinaryX.bat  ?  MeshConvert.exe �� .x ���o�C�i����
rem  �g���� : .x �t�@�C�������� BAT �Ƀh���b�O���h���b�v
rem ------------------------------------------------------------
if "%~1"=="" (
    echo �ϊ������� .x �t�@�C�����h���b�O���h���b�v���Ă��������B
    pause
    goto :eof
)

rem �J�����g�� MeshConvert.exe �̂���t�H���_�[�ɕύX
pushd "%~dp0"

for %%F in (%*) do call :CONVERT "%%~fF"

popd
goto :eof


:CONVERT
rem ---------- 1�t�@�C���� ----------
setlocal
set "SRC=%~1"

rem �g���q�`�F�b�N
if /I not "%~x1"==".x" (
    echo [%SRC%] �� .x �ł͂Ȃ����߃X�L�b�v�B
    goto :EOF
)

echo.
echo === [%SRC%] ���o�C�i�� .x �ɕϊ� ===

rem �ꎞ�t�@�C�����i�����t�H���_�[�� *_bin.x ���쐬�j
set "DST=%~dpn1_bin.x"

rem /x  : .x �o�C�i���`���ŏo��
rem /o  : �o�̓t�@�C����
rem /y  : �����t�@�C�����㏑��
MeshConvert.exe "%SRC%" /x /o "%DST%" /y >nul
if errorlevel 1 (
    echo �ϊ����s...
    goto :EOF
)

rem ���t�@�C����u������
move /Y "%DST%" "%SRC%" >nul
echo �ϊ������B

endlocal
goto :EOF
