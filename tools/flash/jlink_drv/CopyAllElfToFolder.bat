:: 
:: 
:: 将当前目录下所有的ELF 复制到指定目录
:: 
:: 

@echo off
    setlocal enabledelayedexpansion
    
    :: Prompt user for destination directory
    set /p "DEST_DIR=Please enter the target directory path: "
    
    :: Check if user entered a directory
    if "!DEST_DIR!"=="" (
        echo Error: Target directory cannot be empty
        pause
        exit /b 1
    )
    
    :: Check if target directory exists, exit if not
    if not exist "!DEST_DIR!" (
        echo Error: Target directory does not exist - "!DEST_DIR!"
        pause
        exit /b 1
    )
    
    :: Find and copy all ELF files in current directory and subdirectories
    echo Searching for and copying ELF files... 
	echo Current directory:%cd%
    set "COUNT=0"
    
    for /r %%f in (*.elf) do (
        if exist "%%f" (
            copy "%%f" "!DEST_DIR!" >nul
            if errorlevel 1 (
                echo Failed to copy: %%f
            ) else (
                set /a COUNT+=1
                echo Copied: %%f
            )
        )
    )
    
    echo.
    echo Copy completed, successfully copied !COUNT! ELF files to !DEST_DIR!
    pause
    endlocal