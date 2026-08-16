"""
===================================================================
Script Name   : list_txt_to_pdf.py
                text list to pdf converter python script
Description   : Validates and converts a list of text files into a single 
                merged PDF with file path headers, dynamic page numbers,
                and precise 2-space indentation preservation
Location      : Can be placed in any directory alongside list_txt_to_pdf.bat
Usage         : Automatically invoked by list_txt_to_pdf.bat with list path

Author        : Gemini (Google AI)
Verifier      : Anatoly Sharapov
Email         : anatoly.a.sharapov@gmail.com
Repository    : https://github.com
License       : MIT

Date Created  : August 15, 2026
Last Modified : August 15, 2026
Version       : 2.0.4

Change History:
-------------------------------------------------------------------
Version 2.0.4 | Aug 15, 2026 | Added conversion of Pygments CSS span styles to ReportLab compatible font tags.
Version 2.0.3 | Aug 15, 2026 | Reformatted python script source code to use strict 2-space indentation.
Version 2.0.2 | Aug 15, 2026 | Fixed internal indentation by converting only leading spaces to HTML entities.
Version 2.0.1 | Aug 15, 2026 | Adjusted text style leading for exact 2-point line spacing.
Version 2.0.0 | Aug 15, 2026 | Added C syntax highlighting (Pygments) & Windows font autodetect.
Version 1.3.2 | Aug 15, 2026 | Updated tab expansion to 2 spaces for compact layout.
Version 1.3.1 | Aug 15, 2026 | Fixed tab/space indentation stripping inside paragraphs.
Version 1.3.0 | Aug 15, 2026 | Added sequential line numbering for text source files.
===================================================================
"""

import os
import sys
import re
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont

# Pygments imports for advanced syntax highlighting
try:
  from pygments import highlight
  from pygments.lexers import get_lexer_for_filename, TextLexer
  from pygments.formatters import HtmlFormatter
except ImportError:
  highlight = None

class NumberedCanvas(canvas.Canvas):
  """Two-pass canvas wrapper to dynamically render total page count."""
  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)
    self._saved_page_states = []

  def showPage(self):
    self._saved_page_states.append(dict(self.__dict__))
    self._startPage()

  def save(self):
    num_pages = len(self._saved_page_states)
    for state in self._saved_page_states:
      self.__dict__.update(state)
      self.draw_page_number(num_pages)
      super().showPage()
    super().save()

  def draw_page_number(self, page_count):
    self.saveState()
    try:
      self.setFont("Arial", 9)
    except Exception:
      self.setFont("Helvetica", 9)
    self.setFillColorRGB(0.3, 0.3, 0.3)
    page_text = f"{self._pageNumber} / {page_count}"
    self.drawRightString(612 - 54, 36, page_text)
    self.restoreState()


def print_script_header():
  print("-" * 67)
  lines = __doc__.strip().split('\n')
  for line in lines:
    print(line)
  print("-" * 67)
  print()


def convert_pygments_to_reportlab(html_text):
  """Converts <span style="..."> tags from Pygments into ReportLab-compatible tags."""
  # Ищем все конструкции <span style="... (свойства) ...">Текст</span>
  span_pattern = re.compile(r'<span\s+style="([^"]+)"\s*>(.*?)</span>')
  
  def replace_span(match):
    style_content = match.group(1)
    inner_text = match.group(2)
    
    # Извлекаем цвет (например, color: #3D7B7B)
    color_match = re.search(r'color:\s*(#[0-9A-Fa-f]{6})', style_content)
    # Проверяем наличие курсива и жирного стиля
    is_italic = 'font-style: italic' in style_content or 'italic' in style_content
    is_bold = 'font-weight: bold' in style_content or 'bold' in style_content
    
    result = inner_text
    if is_italic:
      result = f"<i>{result}</i>"
    if is_bold:
      result = f"<b>{result}</b>"
    if color_match:
      hex_color = color_match.group(1)
      result = f"<font color='{hex_color}'>{result}</font>"
      
    return result

  # Выполняем замену до тех пор, пока регулярное выражение находит совпадения
  old_text = ""
  while old_text != html_text:
    old_text = html_text
    html_text = span_pattern.sub(replace_span, html_text)
    
  return html_text


def create_pdf_from_list(list_file_path):
  list_file_path = os.path.abspath(list_file_path)
  if not os.path.exists(list_file_path):
    print(f"[Error] List file not found: {list_file_path}")
    return

  script_dir = os.path.dirname(list_file_path)
  os.chdir(script_dir)
  print(f"[Info] Working directory changed to: {script_dir}\n")

  # --- СИСТЕМНЫЙ ПОИСК ШРИФТА ARIAL ДЛЯ КИРИЛЛИЦЫ ---
  font_name = 'Helvetica'
  font_paths = [
    'Arial.ttf',
    'arial.ttf',
    'C:\\Windows\\Fonts\\Arial.ttf',
    'C:\\Windows\\Fonts\\arial.ttf'
  ]
  
  for path in font_paths:
    if os.path.exists(path):
      try:
        pdfmetrics.registerFont(TTFont('Arial', path))
        font_name = 'Arial'
        print(f"[Success] TrueType Cyrillic font registered from: {path}")
        break
      except Exception:
        continue
  if font_name == 'Helvetica':
    print("[Warning] Arial.ttf not found. Cyrillic comments might look empty.")

  styles = getSampleStyleSheet()
  title_style = ParagraphStyle(
    'FileHeader',
    parent=styles['Heading2'],
    fontName=font_name,
    fontSize=11,
    spaceBefore=18,
    spaceAfter=6,
    textColor='navy'
  )
  
  text_style = ParagraphStyle(
    'FileContent',
    parent=styles['Normal'],
    fontName=font_name,
    fontSize=9,
    leading=11,
    spaceBefore=0,
    spaceAfter=0
  )

  story = []

  with open(list_file_path, 'r', encoding='utf-8') as f:
    files_to_process = [
      line.strip() for line in f
      if line.strip() and not line.strip().startswith('#')
    ]

  if not files_to_process:
    print("[Warning] The list file 'txt_to_pdf.lst' is empty.")
    return

  for file_path in files_to_process:
    file_path = file_path.strip('"\'')
    if not os.path.exists(file_path):
      print(f"[Skip] File not found -> {file_path}")
      continue

    print(f"[Processing] {file_path}")
    abs_path_display = os.path.abspath(file_path)
    story.append(Paragraph(f"<b>FILE: {abs_path_display}</b>", title_style))
    story.append(Spacer(1, 4))

    if highlight:
      try:
        lexer = get_lexer_for_filename(file_path)
      except Exception:
        lexer = TextLexer()
    else:
      lexer = None

    with open(file_path, 'r', encoding='utf-8', errors='replace') as txt_file:
      for line_num, line in enumerate(txt_file, 1):
        expanded_line = line.rstrip().expandtabs(2)
        actual_leading_spaces = len(expanded_line) - len(expanded_line.lstrip())

        if lexer and highlight:
          formatter = HtmlFormatter(nowrap=True, noclasses=True)
          html_line = highlight(expanded_line.lstrip(), lexer, formatter).strip()
          if html_line.startswith("<pre>"):
            html_line = html_line.replace("<pre>", "").replace("</pre>", "")
          # Преобразование стилей Pygments в валидный XML формат ReportLab
          html_line = convert_pygments_to_reportlab(html_line)
        else:
          html_line = expanded_line.lstrip().replace('<', '&lt;').replace('>', '&gt;')

        indentation = '&nbsp;' * actual_leading_spaces
        numbered_line = f"<font color='gray'>{line_num}:</font> {indentation}{html_line}"
        story.append(Paragraph(numbered_line, text_style))

    story.append(Spacer(1, 15))

  base_name, _ = os.path.splitext(list_file_path)
  output_pdf = f"{base_name}_merged.pdf"

  try:
    doc = SimpleDocTemplate(
      output_pdf,
      pagesize=letter,
      leftMargin=54, rightMargin=54, topMargin=54, bottomMargin=54
    )
    doc.build(story, canvasmaker=NumberedCanvas)
    print(f"\n[Success] Merged PDF saved to:\n{output_pdf}")
  except Exception as e:
    print(f"[Error] Failed to build PDF document: {e}")


if __name__ == "__main__":
  print_script_header()
  if len(sys.argv) < 2:
    print("[Error] Missing argument: Path to the list file.")
  else:
    create_pdf_from_list(sys.argv[1])
