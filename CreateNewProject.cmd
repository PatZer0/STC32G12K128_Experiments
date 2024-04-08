@echo off
setlocal enabledelayedexpansion

:: ���òֿ����ƺ�·��
set repo_name=STC32G12K128_Library
set repo_url=https://gitee.com/seekfree/STC32G12K128_Library.git
set repo_path=%cd%\.library\%repo_name%

:: ��ȡ��ǰʱ��
for /f "tokens=2 delims==" %%a in ('wmic os get localdatetime /value') do set datetime=%%a
set year=%datetime:~0,4%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set hour=%datetime:~8,2%
set minute=%datetime:~10,2%

:: ��������Ŀ����
set new_project_name=STC32G_Project_%year%_%month%_%day%_%hour%_%minute%

:: ���浱ǰĿ¼
set original_path=%cd%

:: ���ֿ�Ŀ¼�Ƿ����
if exist "%repo_path%" (
    echo ���²ֿ� %repo_name%
    cd "%repo_path%"
    git pull
    cd "%original_path%"
) else (
    echo ��¡�ֿ� %repo_name%
    mkdir .library
    cd .library
    git clone %repo_url%
    cd "%original_path%"
)

:: ���Ʋ��������ļ���
xcopy /E /I /Q ".library\%repo_name%\Seekfree_STC32G12K128_Opensource_Library" "%new_project_name%"

echo �ѳɹ����� %new_project_name%
pause
