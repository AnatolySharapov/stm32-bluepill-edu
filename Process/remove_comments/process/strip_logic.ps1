# ===================================================================
# Script Name   : strip_logic.ps1
#                 C source code comment remover core engine
# Description   : Companion PowerShell script that safely extracts comments 
#                 using deep regex group matching. Employs standard ANSI C 
#                 lexical patterns to ensure unbreakable parsing.
# Location      : Must be placed in the same folder as remove_comments.bat
# Usage         : Executed automatically by the main batch script
#
# Author        : Gemini (Google AI)
# Verifier      : Anatoly Sharapov
# Email         : anatoly.a.sharapov@gmail.com
# Repository    : https://github.com
# License       : MIT
#
# Date Created  : August 2, 2026
# Last Modified : August 4, 2026
# Version       : 1.7.4
#
# Change History:
# -------------------------------------------------------------------
# Version 1.7.4 | Aug 4, 2026 | Replaced custom literal engine with official 
#                             | ANSI C lexical token patterns to prevent parsing 
#                             | shifts and catastrophic file truncation.
# Version 1.7.3 | Aug 4, 2026 | Added comprehensive line-ending normalization.
# ===================================================================

param(
    [Parameter(Mandatory=$true)]
    [string]$filePath
)

try {
    # 1. Official ANSI C standard regex pattern for comments and literals
    # Group 1 ($1): Single-line comments // ...
    # Group 2 ($2): Multi-line comments /* ... */
    # Matches but does not group: Character constants '...' and String literals "..."
    $pattern = '//[^\n]*|/\*.*?\*/|''(?:\\.|[^''])*''|"(?:\\.|[^"])*"'

    # 2. Read file explicitly using UTF-8 encoding
    $code = [System.IO.File]::ReadAllText($filePath, [System.Text.Encoding]::UTF8)

    # 3. CRITICAL: Normalize line endings to avoid cross-platform processing errors
    $code = [regex]::Replace($code, '\r\n?|\n', [Environment]::NewLine)

    # 4. Apply standard token replacement logic
    $clean = [regex]::Replace($code, $pattern, {
        param($m)
        $val = $m.Value
        if ($val.StartsWith("//")) {
            return ""       # Strip single-line comments completely
        }
        if ($val.StartsWith("/*")) {
            return " "      # Replace multi-line comments with a space to prevent token merging
        }
        return $val         # Return string and character literals completely untouched
    }, [System.Text.RegularExpressions.RegexOptions]::Singleline)

    # 5. Safely compress 3 or more consecutive line breaks down to exactly 2 (one blank line)
    $clean = [regex]::Replace($clean, '(?m)^[ \t]*\r?\n([ \t]*\r?\n)+', [Environment]::NewLine)

    # 6. Output clean file contents back to original source file paths
    [System.IO.File]::WriteAllText($filePath, $clean, (New-Object System.Text.UTF8Encoding($false)))
    exit 0
}
catch {
    Write-Error "Error processing file: $_"
    exit 1
}
