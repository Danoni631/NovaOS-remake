#include "../../include/stdint.h"
#include "../../Graphics/graphics.h"
#include "../../Graphics/novagl2.h"
#include "../../Hardware/disk.h"
#include "../../Font/text.h"
#include "../../Kernel/sysfiles.h"
#include "../GUI/win.h"
#include "../../Shell/shell.h"
#include "../userspace.h"

#include "install.h"

typedef enum
{
    STATE_WELCOME,
    STATE_DISK_SELECT,
    STATE_INSTALLING
} InstallState;

InstallState currentState = STATE_WELCOME;

void Setup(void* arg);
void StartInstallation(void* arg);

void SystemLoop(WINDOW buttonTemplate)
{
    if (currentState == STATE_WELCOME)
    {
        InitSetupUI();
    }

    else if (currentState == STATE_DISK_SELECT)
    {
        DrawDiskSelectionUI(buttonTemplate);
    }
    
    else if (currentState == STATE_INSTALLING)
    {
        InstallingScreen();
    }
}

void InitSetupUI()
{
    DrawBootScr();
    DrawUI();
}

void DrawUI()
{
    WINDOW start;

    int x = WSCREEN / 2 - 300 / 2;
    int y = HSCREEN / 2 - 150 - 50;

    DrawRect(0, 0, WSCREEN, HSCREEN, OTHER_GUI_GRAY2);
    SetCursorX(x + 120);
    SetCursorY(y + 20);
    Print("Welcome to NovaOS Core installation setup!", WHITE);
    int pressed = CheckClickButton(x, y, start.w, start.h);

    const char* text = "Install";
    DrawGUIButton(x, y, 50, 300, pressed, start, 0xFF1A1A1A, text, Setup);
}

void Setup(void* arg)
{
    (void)arg;
    currentState = STATE_DISK_SELECT;
    DrawRect(0, 0, WSCREEN, HSCREEN, OTHER_GUI_GRAY2);
    SetCursorX(600);
    SetCursorY(400);
    Print("Select target disk to install NovaOS Core", WHITE);
    SetCursorX(50);
    SetCursorY(100);
    DrawRoundedRect(400, 300, 500, 400, 8, OTHER_GUI_GRAY);
}

void StartInstallation(void* arg)
{
    (void)arg;
    currentState = STATE_INSTALLING;
}

void DrawDiskSelectionUI(WINDOW diskButtonMaster)
{
    Setup(NULL);
    int win_w = 450;
    int win_h = 300;
    int x = WSCREEN / 2 - win_w / 2;
    int y = HSCREEN / 2 - win_h / 2;

    int button_x = x + 40;
    int button_y = y + 70;
    int button_spacing = 60;

    DWORD DiskCapacity =
    GetDiskCapacity(IDE_PRIMARY_COMMAND_PORT);
    
    if (DiskCapacity > 0)
    {
        WINDOW disk0_btn = diskButtonMaster;
        disk0_btn.w = 370;
        disk0_btn.h = 45;

        int pressed =
        CheckClickButton
        (
            button_x, button_y,
            disk0_btn.w, disk0_btn.h
        );

        SetCursorX(button_x + 10);
        SetCursorY(button_y + 15);

        const char* textB = "Disk 0: IDE Primary Drive";

        DrawGUIButton
        (
            button_x, button_y,
            disk0_btn.w, disk0_btn.h,
            pressed, disk0_btn,
            OTHER_GUI_GRAY2,
            textB,
            StartInstallation
        );
        
        button_y += button_spacing;
    }

    DWORD FloppyCapacity = GetFloppyCapacity();
    if (FloppyCapacity > 0)
    {
        WINDOW floppy_btn = diskButtonMaster;
        floppy_btn.w = 370;
        floppy_btn.h = 45;

        int pressed =
        CheckClickButton
        (
            button_x,
            button_y,
            floppy_btn.w,
            floppy_btn.h
        );

        SetCursorX(button_x + 10);
        SetCursorY(button_y + 15);

        const char* textA = "Floppy Disk Drive";
        DrawGUIButton
        (
            button_x, button_y,
            floppy_btn.w, floppy_btn.h,
            pressed, floppy_btn,
            OTHER_GUI_GRAY2,
            textA,
            StartInstallation
        );
        
        button_y += button_spacing;
    }

    if (DiskCapacity == 0 && FloppyCapacity == 0)
    {
        SetCursorX(x + 40);
        SetCursorY(y + 120);
        Print
        (
            "No drives detected! Please check your IDE connections.",
            RED
        );
    }
}

void InstallingScreen()
{
    DrawRect(0, 0, WSCREEN, HSCREEN, OTHER_GUI_GRAY2);
    Print("Installing NovaOS Core...", WHITE);
    all_system_files();
    ClearScreen();
    DrawBootScr();
    SetCursorX(0);
    SetCursorY(-600);
    Print("NovaOS Core is starting...", WHITE);
    ClearScreen();
    StartShellNoGUI();
    UserSpace();
}