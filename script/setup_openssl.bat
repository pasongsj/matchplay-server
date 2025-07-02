@echo off
setlocal

REM OpenSSL 3.5.x 브랜치 or 태그 (예: openssl-3.5.1)
set OPENSSL_TAG=openssl-3.5.1
set CLONE_DIR=%~dp0..\build\openssl_src
set DEST_INCLUDE=%~dp0..\lib\ssl\include\openssl

REM 1. 디렉터리 생성
if not exist "%CLONE_DIR%" mkdir "%CLONE_DIR%"
if not exist "%DEST_INCLUDE%" mkdir "%DEST_INCLUDE%"

REM 2. Git clone (이미 되어있으면 스킵)
cd /d "%CLONE_DIR%"
if not exist ".git" (
    echo [INFO] OpenSSL 소스 클론 중...
    git clone --depth 1 --branch %OPENSSL_TAG% https://github.com/openssl/openssl.git .
) else (
    echo [INFO] OpenSSL 소스 이미 존재함
)

REM 3. 헤더 복사
echo [INFO] openssl 헤더 복사 중...
xcopy /Y /S /I "%CLONE_DIR%\include\openssl\*" "%DEST_INCLUDE%\"

echo [INFO] 완료!

endlocal
