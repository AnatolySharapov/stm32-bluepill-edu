```text
===================================================================
Script Name   : list_txt_to_pdf.bat/.py
Description   : Project overview, installation, and usage guidelines
Location      : Root directory of the repository
Usage         : Double-click list_txt_to_pdf.bat to execute
Author        : Gemini (Google AI)
Verifier      : Anatoly Sharapov
Email         : anatoly.a.sharapov@gmail.com
Repository    : https://github.com
License       : MIT
Date Created  : August 15, 2026
Last Modified : August 15, 2026
Version       : 1.4.0

Change History:
-------------------------------------------------------------------
Version 1.4.0 | Aug 15, 2026 | Synchronized guide with pygments syntax highlighting and v1.4.0/v2.0.4 core logic.
Version 1.3.2 | Aug 15, 2026 | Updated tab expansion layout references to compact 2-space grid.
Version 1.3.1 | Aug 15, 2026 | Fixed tab/space indentation definitions inside layout paragraphs.
Version 1.3.0 | Aug 15, 2026 | Added sequential line numbering specifications for text source files.
===================================================================
```

# Text List to PDF Converter

A lightweight, automated Windows utility designed to validate a list of text files and compile their content into a single, cohesive PDF document. Each embedded file block is automatically preceded by a stylized title displaying its full absolute system path. It is optimized for source code structures (C/C++, logs, configs) where exact indentation formatting, syntax highlighting, and precise orientation are critical.

---

## Project Structure

The repository contains the following core files, designed to work together seamlessly:

* **`list_txt_to_pdf.bat`** – The main Windows entry point (v1.4.0). It dynamically handles target lists via `"%list_file%"`, automatically verifies and installs Python dependencies (`reportlab`, `pygments`) via pip if missing, validates the presence of the input list file, and executes the processing core.
* **`list_txt_to_pdf.py`** – The processing engine written in Python (v2.0.4). It parses paths, handles UTF-8 encoding, ensures Cyrillic character rendering, runs line-by-line index formatting, translates Pygments CSS token tags into compatible XML formats, and generates the final layout via `reportlab`.
* **`build_exe.bat`** – An automation script to compile the Python code into a completely standalone `.exe` binary using PyInstaller.
* **`requirements.txt`** – Contains the necessary third-party package signatures (`reportlab`, `pygments`).

---

## Getting Started & Usage

### Method 1: Running via Batch Script
1. Clone or download this repository into a local folder (e.g., `D:\saa\work\...\list_txt_to_pdf\`).
2. Run **`list_txt_to_pdf.bat`** once by double-clicking it. It will automatically detect, verify dependencies, and generate an empty configuration file named **`sources_to_pdf.lst`** (dynamically resolved via `%list_file%`) in the same directory if no index exists.
3. Open the generated list file and populate it with relative or absolute paths to the text files you want to merge (one path per line, lines starting with `#` are treated as comments).
4. Double-click **`list_txt_to_pdf.bat`** again. The script will read the workspace listing registry and build the final **`sources_to_pdf_merged.pdf`** document.

### Method 2: Creating a Standalone Executable
If you need to distribute this utility to machines without a Python environment:
1. Place a copy of **`Arial.ttf`** (found in `C:\Windows\Fonts\`) directly into the project directory.
2. Launch **`build_exe.bat`**.
3. Once compilation succeeds, navigate to the newly created `dist/` directory to find your standalone **`list_txt_to_pdf.exe`**.

---

## Layout and Styling Details

* **Advanced Syntax Highlighting:** Integrates `pygments` lexers to automatically colorize language-specific structural operators, variables, and expressions inside your source files.
* **ReportLab Guard Engine:** Contains a regex processing sequence that translates modern inline CSS HTML `<span>` attributes into safe `<font>`, `<b>`, and `<i>` tags, avoiding native `ValueError` parse errors.
* **Line Numbering:** Automatically enumerates every row in each text file independently, starting from 1. Line indexes are rendered in a sleek, muted gray font to distinguish metadata from content.
* **Indentation Preservation:** Explicitly expands all tabs into a compact 2-space alignment grid and replaces leading spacing tokens with non-breaking symbols (`&nbsp;`). This forces the PDF engine to strictly preserve code hierarchy layouts, block nested statements, and tabular data scopes while allowing native line wrapping for long code strings.
* **Compact Text Geometry:** Configured with a precise typography style (`fontSize=9`, `leading=11`) providing a strict 2-point vertical line spacing layout to compress maximum blocks per page seamlessly.
* **Font Support:** Explicitly embeds and uses the `Arial` font family to provide absolute out-of-the-box compatibility with Cyrillic (Russian) text layouts, comments, and mathematical symbols.
* **Two-Pass Page Numbering:** Utilizes a custom structural canvas workflow to render minimalist, right-aligned page counts in a strict **`X / Y`** format (e.g., `1 / 17`), with dynamic calculations and safe bottom page margins.
* **Visual Anchors:** Each source file block is cleanly separated with dedicated spacing, highlighted by a bold, deep navy header displaying the original data path.

---

## License

This project is licensed under the terms of the **MIT License**.
