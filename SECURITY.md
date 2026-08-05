# Security Policy

## Supported Versions

The latest version of this project receives security updates.

| Version | Supported |
| :------ | :-------: |
| 1.x | ✅ |
| < 1.0 | ❌ |

## Reporting a Vulnerability

If you discover a security vulnerability, please **do not** report it through a public GitHub Issue.

Instead, report it privately by contacting the project maintainer:

**Anatoly Sharapov**

Email: anatoly.a.sharapov@gmail.com

Please include the following information:

- Project version
- STM32CubeIDE version
- Target hardware
- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Proof-of-concept (if available)

## Response Process

Every reported vulnerability will be:

1. Acknowledged within a reasonable time.
2. Investigated by the project maintainer.
3. Fixed if confirmed.
4. Documented in the project changelog when appropriate.

## Disclosure Policy

Please allow reasonable time for investigation and remediation before publicly disclosing a vulnerability.

Responsible disclosure helps protect users of the project.

## Scope

This policy applies to:

- Source code
- Build scripts
- Project configuration files
- Documentation contained in this repository

Third-party software, including but not limited to:

- STM32CubeIDE
- STM32CubeMX
- CMSIS
- STM32 HAL/LL Drivers
- GNU Arm Embedded Toolchain

is maintained by their respective vendors and is outside the scope of this security policy.

## Security Best Practices

When using this project:

- Keep STM32CubeIDE up to date.
- Use the latest STM32 firmware packages.
- Verify changes before flashing production hardware.
- Review external contributions before merging.
- Protect firmware signing keys and programming interfaces when applicable.

Thank you for helping improve the security of this project.