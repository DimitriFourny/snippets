
WMI Process Watcher
======================

A technique to asynchronously detect when a process is created using WMI.
I have got this idea by the CIA Applied Engineering Division (AED).

The program will have this following ouput in a new console::

    [+] New Process!
    -----------------------------
    SECURITY_DESCRIPTOR : 0x0
    TargetInstance : 0xCA1DB0
        Caption : sublime_text.exe
        CommandLine : "C:\Program Files\Sublime Text 3\sublime_text.exe"
        CreationClassName : Win32_Process
        CreationDate : 0xC948EC
        CSCreationClassName : Win32_ComputerSystem
        CSName : DESKTOP-K4H7ATJ
        Description : sublime_text.exe
        ExecutablePath : C:\Program Files\Sublime Text 3\sublime_text.exe
        ExecutionState : 0x0
        Handle : 18360
        HandleCount : 0x84
        InstallDate : 0x0
        KernelModeTime : 0xC9824C
        MaximumWorkingSetSize : 0x564
        MinimumWorkingSetSize : 0xC8
        Name : sublime_text.exe
        OSCreationClassName : Win32_OperatingSystem
        OSName : Microsoft Windows 10 Professionnel|C:\WINDOWS|\Device\Harddisk0\Partition4
        OtherOperationCount : 0xC98184
        OtherTransferCount : 0xC98684
        PageFaults : 0xB64
        PageFileUsage : 0xFA4
        ParentProcessId : 0x3538
        PeakPageFileUsage : 0xFA4
        PeakVirtualSize : 0xCAA734
        PeakWorkingSetSize : 0x2A20
        Priority : 0x8
        PrivatePageCount : 0xCAA75C
        ProcessId : 0x47B8
        QuotaNonPagedPoolUsage : 0xB
        QuotaPagedPoolUsage : 0xC7
        QuotaPeakNonPagedPoolUsage : 0xB
        QuotaPeakPagedPoolUsage : 0xC8
        ReadOperationCount : 0xCA6344
        ReadTransferCount : 0xCAA11C
        SessionId : 0xB
        Status : NULL_STR
        TerminationDate : 0x0
        ThreadCount : 0x5
        UserModeTime : 0xCAA2D4
        VirtualSize : 0xCAA4DC
        WindowsVersion : 10.0.14393
        WorkingSetSize : 0xCAA1BC
        WriteOperationCount : 0xCA63BC
        WriteTransferCount : 0xCA61DC
    TIME_CREATED : 0xC97D2C