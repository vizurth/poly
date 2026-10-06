from __future__ import annotations

import re
from pathlib import Path

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.style import WD_STYLE_TYPE
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor
import pdfplumber


SOURCE = Path('/Users/t.adiatullin/Downloads/backalor-4-2020.pdf')
OUTPUT = Path('output/docx/laboratornye_raboty_unix_editable.docx')


def normalize(text: str) -> str:
    return re.sub(r'\s+', ' ', text.replace('\u00a0', ' ')).strip()


def set_cell_shading(cell, fill: str) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
    shade = OxmlElement('w:shd')
    shade.set(qn('w:fill'), fill)
    tc_pr.append(shade)


def add_page_number(paragraph) -> None:
    paragraph.alignment = WD_ALIGN_PARAGRAPH.RIGHT
    run = paragraph.add_run()
    field_char = OxmlElement('w:fldChar')
    field_char.set(qn('w:fldCharType'), 'begin')
    instr = OxmlElement('w:instrText')
    instr.set(qn('xml:space'), 'preserve')
    instr.text = ' PAGE '
    field_char_end = OxmlElement('w:fldChar')
    field_char_end.set(qn('w:fldCharType'), 'end')
    run._r.append(field_char)
    run._r.append(instr)
    run._r.append(field_char_end)


def configure_document(doc: Document) -> None:
    section = doc.sections[0]
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(1.8)
    section.bottom_margin = Cm(1.6)
    section.left_margin = Cm(2.1)
    section.right_margin = Cm(1.7)

    normal = doc.styles['Normal']
    normal.font.name = 'Arial'
    normal._element.rPr.rFonts.set(qn('w:eastAsia'), 'Arial')
    normal.font.size = Pt(10.5)
    pf = normal.paragraph_format
    pf.space_after = Pt(5)
    pf.line_spacing = 1.08

    for name, size, before, after in [
        ('Title', 18, 0, 14),
        ('Heading 1', 15, 16, 8),
        ('Heading 2', 13, 12, 6),
        ('Heading 3', 11.5, 8, 4),
    ]:
        style = doc.styles[name]
        style.font.name = 'Arial'
        style._element.rPr.rFonts.set(qn('w:eastAsia'), 'Arial')
        style.font.size = Pt(size)
        style.font.bold = True
        style.font.color.rgb = RGBColor(0, 0, 0)
        style.paragraph_format.space_before = Pt(before)
        style.paragraph_format.space_after = Pt(after)
        style.paragraph_format.keep_with_next = True
        if name == 'Title':
            # Word's built-in Title style can carry a blue bottom rule.
            # This document deliberately contains no answer lines or rules.
            p_pr = style.element.find(qn('w:pPr'))
            if p_pr is not None:
                p_bdr = p_pr.find(qn('w:pBdr'))
                if p_bdr is not None:
                    p_pr.remove(p_bdr)

    toc = doc.styles.add_style('Contents Item', WD_STYLE_TYPE.PARAGRAPH)
    toc.base_style = normal
    toc.font.size = Pt(10.5)
    toc.paragraph_format.space_after = Pt(2)
    toc.paragraph_format.left_indent = Cm(0.5)

    footer = section.footer.paragraphs[0]
    footer.style = doc.styles['Normal']
    add_page_number(footer)


def is_heading(text: str) -> str | None:
    if re.match(r'^Лабораторная работа \d+\.', text, re.I):
        return 'Heading 1'
    if re.match(r'^Упражнение \d+\.\d+\.', text, re.I):
        return 'Heading 2'
    if re.match(r'^(Основы программирования|Создание скелета сценария|Реализация алгоритма)', text, re.I):
        return 'Heading 2'
    return None


def is_item(text: str) -> str | None:
    if re.match(r'^\d+\.\s+', text):
        return 'List Number'
    if re.match(r'^[a-zа-я]\.', text, re.I):
        return 'List Bullet 2'
    if text.startswith('•'):
        return 'List Bullet'
    return None


def write_paragraph(doc: Document, text: str) -> None:
    text = normalize(text)
    if not text:
        return
    heading = is_heading(text)
    if heading:
        # Some PDF pages omit a paragraph break after a heading.  In those
        # cases pypdf joins the heading and the first ordinary paragraph.
        body_match = re.search(r'\s(?=(?:При помощи|Используя|Примечание:))', text)
        if body_match:
            doc.add_paragraph(text[:body_match.start()], style=heading)
            write_paragraph(doc, text[body_match.start():])
            return
        doc.add_paragraph(text, style=heading)
        return
    style = is_item(text)
    if style:
        # Keep the source numbering as text.  Applying Word's numbered-list
        # style would visibly duplicate every number (e.g. "1. 1. ...").
        paragraph = doc.add_paragraph(text)
        if text.startswith('•') or re.match(r'^[a-zа-я]\.', text, re.I):
            paragraph.paragraph_format.left_indent = Cm(1.25)
            paragraph.paragraph_format.first_line_indent = Cm(-0.55)
        else:
            paragraph.paragraph_format.left_indent = Cm(0.75)
            paragraph.paragraph_format.first_line_indent = Cm(-0.75)
        return
    doc.add_paragraph(text)


def extract_content() -> tuple[list[str], list[str]]:
    paragraphs: list[str] = []
    current: list[str] = []

    # Page 1 is a static contents list. The actual lab material starts on page 2.
    with pdfplumber.open(SOURCE) as pdf:
        for page in pdf.pages[1:]:
            # pdfplumber rebuilds text from character coordinates, avoiding
            # spurious spaces introduced by pypdf around manually kerned words.
            raw = page.extract_text() or ''
            for raw_line in raw.splitlines():
                line = normalize(raw_line)
                if not line or re.fullmatch(r'\d+', line):
                    if current:
                        paragraphs.append(' '.join(current))
                        current = []
                    continue
                if line in {'Лабораторная работа зачтена:', 'Дата:', 'Подпись преподавателя:'}:
                    if current:
                        paragraphs.append(' '.join(current))
                        current = []
                    continue
                # Notes have their own paragraph in the source even where its
                # PDF text stream has no blank-line marker.
                if is_heading(line) or is_item(line) or line.startswith('Примечание'):
                    if current:
                        paragraphs.append(' '.join(current))
                    current = [line]
                else:
                    current.append(line)
            if current:
                paragraphs.append(' '.join(current))
                current = []

    headings = [p for p in paragraphs if is_heading(normalize(p)) == 'Heading 1']
    return paragraphs, headings


def build_document() -> None:
    paragraphs, lab_headings = extract_content()
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)

    doc = Document()
    configure_document(doc)

    title = doc.add_paragraph('Лабораторные работы по UNIX', style='Title')
    title.alignment = WD_ALIGN_PARAGRAPH.CENTER
    note = doc.add_paragraph('Сборник заданий для выполнения лабораторных работ по UNIX.')
    note.alignment = WD_ALIGN_PARAGRAPH.CENTER
    note.paragraph_format.space_after = Pt(18)

    doc.add_paragraph('Содержание', style='Heading 1')
    for heading in lab_headings:
        doc.add_paragraph(heading, style='Contents Item')
    doc.add_page_break()

    for paragraph in paragraphs:
        write_paragraph(doc, paragraph)

    doc.core_properties.title = 'Лабораторные работы по UNIX'
    doc.core_properties.subject = 'Редактируемая версия заданий без линий для ответов'
    doc.core_properties.author = 'Converted from backalor-4-2020.pdf'
    doc.save(OUTPUT)
    print(f'Created {OUTPUT}')
    print(f'Content paragraphs: {len(paragraphs)}; lab sections: {len(lab_headings)}')


if __name__ == '__main__':
    build_document()
