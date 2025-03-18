[CmdletBinding()]
param (
    [string] $WarnRedirect = ""
)

function Write-A-Warning {
    [CmdletBinding()]
    param()
    Write-Warning "`nTest`n"
}

$cmd = "Write-A-Warning"
#if ($WarnRedirect) {
#    $WarningLogFile = "Warnings-" + $(Get-Date -Format "yyyyMMdd-HHmm") + ".log"
#    $cmd += " 3>$WarningLogFile"
#}

Invoke-Expression $cmd