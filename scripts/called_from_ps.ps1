$ppid = (Get-CimInstance -ClassName Win32_Process -Filter "ProcessId = $PID").ParentProcessId

if ($ppid) {
    Write-Host "Parent PID: $ppid"
    $process = Get-CimInstance -ClassName Win32_Process -Filter "ProcessId = $ppid"
    if ($process.Name -like "*powershell*" -or $process.Name -like "*pwsh*") {
        Write-Host "PowerShell"
        exit 0
    }
}
Write-Host "Not PowerShell"
exit 1