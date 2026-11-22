import csv
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont
from docx import Document
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Inches, Pt, RGBColor


root = Path(__file__).resolve().parent
chart_file = root / "speedup_comparison.png"
report_file = root / "report_openmp_task4.docx"


def font(size, bold=False):
    name = "arialbd.ttf" if bold else "arial.ttf"
    return ImageFont.truetype(str(Path("C:/Windows/Fonts") / name), size)


def make_chart(rows):
    data = [r for r in rows if int(r["size"]) == 3000]
    threads = [int(r["threads"]) for r in data]
    red = [float(r["red_speedup"]) for r in data]
    nored = [float(r["nored_speedup"]) for r in data]
    w, h = 1300, 660
    left, right, top, bottom = 120, 55, 75, 100
    image = Image.new("RGB", (w, h), "white")
    draw = ImageDraw.Draw(image)
    pw, ph = w - left - right, h - top - bottom

    def x(i):
        return left + i * pw / (len(threads) - 1)

    def y(value):
        return top + ph - value / 7 * ph

    for value in range(8):
        yy = y(value)
        draw.line((left, yy, w - right, yy), fill="#d9e1e8", width=2)
        draw.text((left - 48, yy - 12), str(value), font=font(20), fill="#263238")

    draw.line((left, top, left, h - bottom), fill="#263238", width=3)
    draw.line((left, h - bottom, w - right, h - bottom), fill="#263238", width=3)

    for i, value in enumerate(threads):
        xx = x(i)
        label = str(value)
        box = draw.textbbox((0, 0), label, font=font(20))
        draw.text((xx - (box[2] - box[0]) / 2, h - bottom + 15), label, font=font(20), fill="#263238")

    def line(values, color):
        points = [(x(i), y(value)) for i, value in enumerate(values)]
        draw.line(points, fill=color, width=6)
        for px, py in points:
            draw.ellipse((px - 8, py - 8, px + 8, py + 8), fill=color, outline="white", width=2)

    line(red, "#1565c0")
    line(nored, "#e65100")
    title = "Ускорение для матрицы 3000 x 3000"
    box = draw.textbbox((0, 0), title, font=font(34, True))
    draw.text(((w - box[2] + box[0]) / 2, 18), title, font=font(34, True), fill="#1f2933")
    draw.text((w / 2 - 105, h - 48), "Число потоков", font=font(24), fill="#1f2933")
    draw.text((15, 18), "S", font=font(24), fill="#1f2933")
    lx, ly = 760, 90
    draw.line((lx, ly, lx + 70, ly), fill="#1565c0", width=6)
    draw.text((lx + 85, ly - 14), "С редукцией", font=font(22), fill="#1f2933")
    draw.line((lx, ly + 42, lx + 70, ly + 42), fill="#e65100", width=6)
    draw.text((lx + 85, ly + 28), "Без редукции", font=font(22), fill="#1f2933")
    image.save(chart_file)


def shade(cell, color):
    props = cell._tc.get_or_add_tcPr()
    element = props.find(qn("w:shd"))
    if element is None:
        element = OxmlElement("w:shd")
        props.append(element)
    element.set(qn("w:fill"), color)


def borders(cell):
    props = cell._tc.get_or_add_tcPr()
    group = props.first_child_found_in("w:tcBorders")
    if group is None:
        group = OxmlElement("w:tcBorders")
        props.append(group)
    for edge in ("top", "left", "bottom", "right", "insideH", "insideV"):
        tag = "w:" + edge
        item = group.find(qn(tag))
        if item is None:
            item = OxmlElement(tag)
            group.append(item)
        item.set(qn("w:val"), "single")
        item.set(qn("w:sz"), "6")
        item.set(qn("w:color"), "D9D9D9")


def heading(doc, value):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(8)
    p.paragraph_format.space_after = Pt(4)
    p.paragraph_format.keep_with_next = True
    r = p.add_run(value)
    r.bold = True
    r.font.name = "Times New Roman"
    r.font.size = Pt(14)
    r.font.color.rgb = RGBColor(0, 0, 0)


def text(doc, value):
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
    p.paragraph_format.first_line_indent = Cm(1)
    p.paragraph_format.space_after = Pt(4)
    p.paragraph_format.line_spacing = 1.08
    r = p.add_run(value)
    r.font.name = "Times New Roman"
    r.font.size = Pt(11)


def make_report(rows):
    doc = Document()
    section = doc.sections[0]
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(1.6)
    section.bottom_margin = Cm(1.5)
    section.left_margin = Cm(1.7)
    section.right_margin = Cm(1.5)
    doc.styles["Normal"].font.name = "Times New Roman"
    doc.styles["Normal"].font.size = Pt(11)

    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_after = Pt(8)
    r = p.add_run("Задание 4 Максимум среди минимумов строк матрицы")
    r.bold = True
    r.font.name = "Times New Roman"
    r.font.size = Pt(15)
    r.font.color.rgb = RGBColor(0, 0, 0)

    heading(doc, "Условие задания")
    text(doc, "Разработать программу поиска максимального значения среди минимальных элементов строк матрицы. Провести вычислительные эксперименты для матриц разного размера и разного количества потоков, измерить время выполнения и вычислить ускорение параллельных вариантов.")
    heading(doc, "Суть решения")
    text(doc, "Для каждой строки квадратной матрицы сначала находится минимальный элемент. Затем среди полученных минимумов выбирается максимальное значение. seq.cpp выполняет оба шага последовательно. red.cpp распределяет строки между потоками и использует reduction(max:result). nored.cpp находит локальный результат в каждом потоке и обновляет общий результат внутри critical-секции.")
    text(doc, "Каждый вариант повторяет вычисление не менее 0,05 с. В таблицу записывается среднее время одного вычисления. Ускорение равно отношению времени последовательного варианта ко времени параллельного варианта.")
    heading(doc, "Результаты запусков")
    caption = doc.add_paragraph("Таблица 1 - Время выполнения и ускорение")
    caption.paragraph_format.space_after = Pt(4)
    caption.runs[0].italic = True
    caption.runs[0].font.size = Pt(10)

    headers = ["Размер N", "Потоки", "T посл., с", "T red, с", "S red", "T без red, с", "S без red"]
    table = doc.add_table(rows=1, cols=7)
    table.alignment = WD_ALIGN_PARAGRAPH.CENTER
    table.autofit = False
    widths = [Cm(2.45), Cm(1.35), Cm(2.15), Cm(2.05), Cm(1.5), Cm(2.35), Cm(1.7)]

    for i, value in enumerate(headers):
        cell = table.rows[0].cells[i]
        cell.width = widths[i]
        cell.text = value
        cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
        shade(cell, "2F5597")
        borders(cell)
        p = cell.paragraphs[0]
        p.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p.paragraph_format.space_after = Pt(0)
        for r in p.runs:
            r.bold = True
            r.font.name = "Arial"
            r.font.size = Pt(8)
            r.font.color.rgb = RGBColor(255, 255, 255)

    for number, row in enumerate(rows):
        values = [
            row["size"], row["threads"], f'{float(row["seq_time"]):.6f}',
            f'{float(row["red_time"]):.6f}', f'{float(row["red_speedup"]):.2f}',
            f'{float(row["nored_time"]):.6f}', f'{float(row["nored_speedup"]):.2f}',
        ]
        cells = table.add_row().cells
        for i, value in enumerate(values):
            cells[i].width = widths[i]
            cells[i].text = value
            cells[i].vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
            borders(cells[i])
            if number % 2:
                shade(cells[i], "EDF3F8")
            p = cells[i].paragraphs[0]
            p.alignment = WD_ALIGN_PARAGRAPH.CENTER
            p.paragraph_format.space_after = Pt(0)
            for r in p.runs:
                r.font.name = "Arial"
                r.font.size = Pt(7.5)

    doc.add_page_break()
    heading(doc, "График сравнения")
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_after = Pt(2)
    p.add_run().add_picture(str(chart_file), width=Inches(6.6))
    p = doc.add_paragraph("Рисунок 1 - Зависимость ускорения от количества потоков")
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_after = Pt(6)
    p.runs[0].italic = True
    p.runs[0].font.size = Pt(10)
    heading(doc, "Короткий анализ")
    text(doc, "Для матрицы 500 x 500 лучшее ускорение с reduction равно 2,12 при 4 потоках, а без reduction - 2,26 при 6 потоках. При дальнейшем увеличении числа потоков ускорение уменьшается.")
    text(doc, "Для матрицы 1000 x 1000 лучший результат получен при 12 потоках: 5,04 с reduction и 5,41 без reduction.")
    text(doc, "Для матрицы 3000 x 3000 максимальное ускорение также получено при 12 потоках: 6,45 с reduction и 6,32 без reduction. При 24 потоках оба варианта работают немного медленнее, потому что расходы на управление потоками и доступ к памяти начинают ограничивать производительность.")
    doc.save(report_file)


with (root / "results.csv").open("r", encoding="utf-8-sig", newline="") as f:
    rows = list(csv.DictReader(f))

make_chart(rows)
make_report(rows)
print(report_file)
