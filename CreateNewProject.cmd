@echo off
setlocal enabledelayedexpansion

:: 设置仓库名称和路径
set repo_name=STC32G12K128_Library
set repo_url=https://gitee.com/seekfree/STC32G12K128_Library.git
set repo_path=%cd%\.library\%repo_name%

:: 获取当前时间
for /f "tokens=2 delims==" %%a in ('wmic os get localdatetime /value') do set datetime=%%a
set year=%datetime:~0,4%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set hour=%datetime:~8,2%
set minute=%datetime:~10,2%

:: 设置新项目名称
set new_project_name=STC32G_Project_%year%_%month%_%day%_%hour%_%minute%

:: 保存当前目录
set original_path=%cd%

:: 检查仓库目录是否存在
if exist "%repo_path%" (
    echo 更新仓库 %repo_name%
    cd "%repo_path%"
    git pull
    cd "%original_path%"
) else (
    echo 克隆仓库 %repo_name%
    mkdir .library
    cd .library
    git clone %repo_url%
    cd "%original_path%"
)

:: 复制并重命名文件夹
xcopy /E /I /Q ".library\%repo_name%\Seekfree_STC32G12K128_Opensource_Library" "%new_project_name%"

echo 已成功创建 %new_project_name%
pause
