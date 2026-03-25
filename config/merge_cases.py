import re
import sys

COL = 75
COL_INIT = 100   # column limit for struct-init packing (matches .clang-format ColumnLimit)

ENUM_VAL_RE  = re.compile(r'^(\t+)(\w+)(,?)\s*$')
STRUCT_ENTRY_RE = re.compile(r'^(\s+)(\{[^{}]+\},?)\s*$')
CASE_RE      = re.compile(r'(case\s+\S+\s*:)')
SINGLE_RE    = re.compile(r'^\s*case\s+\S+\s*:(\s*\{)?\s*$')
IF_RE        = re.compile(r'^(\s*)((?:else\s+)?if)\s*(\()')
CASE_BODY_RE = re.compile(r'^\s*case\s+[^:]+:\s*$')
TERMINATOR_RE = re.compile(r'\b(break|return|continue)\b.*;\s*$')
DESIG_RE     = re.compile(r'^(\s+)\[(\w+)\]\s*=\s*(.+?)(\s*,?\s*)$')


# ── case merging ──────────────────────────────────────────────────────────────

def normalize(lines):
    """Split any line with multiple case labels back to one per line."""
    out = []
    for line in lines:
        labels = CASE_RE.findall(line)
        if len(labels) < 2:
            out.append(line)
            continue
        indent_str = line[: len(line) - len(line.lstrip())]
        brace = ' {' if line.rstrip().endswith('{') else ''
        for k, label in enumerate(labels):
            tail = brace if k == len(labels) - 1 else ''
            out.append(indent_str + label + tail + '\n')
    return out


def pack(lines):
    """Merge consecutive single-case lines onto lines <= COL chars."""
    out = []
    i = 0
    while i < len(lines):
        if not SINGLE_RE.match(lines[i]):
            out.append(lines[i])
            i += 1
            continue
        indent_str = lines[i][: len(lines[i]) - len(lines[i].lstrip())]
        cases = []
        while i < len(lines):
            m = re.match(r'\s*(case\s+\S+\s*:)\s*(\{?)\s*$', lines[i])
            if not m:
                break
            cases.append((m.group(1), m.group(2)))
            i += 1
        cur = indent_str
        for label, brace in cases:
            piece = label + (' {' if brace else ' ')
            if len((cur + piece).rstrip()) > COL and cur.strip():
                out.append(cur.rstrip() + '\n')
                cur = indent_str
            cur += piece
        out.append(cur.rstrip() + '\n')
    return out


# ── if/else-if body alignment ─────────────────────────────────────────────────

def find_paren_end(s, start):
    """Return the index just after the closing ) of the paren group at start."""
    depth = 0
    for i in range(start, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return i + 1
    return -1


def align_if_chain(lines):
    """Align bodies of consecutive if/else-if single-line statements."""
    out = []
    i = 0
    while i < len(lines):
        if not IF_RE.match(lines[i]):
            out.append(lines[i])
            i += 1
            continue

        # Collect run of if / else if lines
        chain = []   # (raw_line, cond_end_idx, body_str)
        j = i
        while j < len(lines):
            line = lines[j]
            m = IF_RE.match(line)
            if not m:
                break
            paren_start = m.start(3)
            cond_end = find_paren_end(line, paren_start)
            if cond_end < 0:
                break
            body = line[cond_end:].strip()
            chain.append((line, cond_end, body))
            j += 1
            if not body or body == '{':   # condition with block body — ends the chain
                break

        # No progress — emit the line as-is and move on
        if not chain:
            out.append(lines[i])
            i += 1
            continue

        # Only align when 2+ lines have an inline body
        body_lines = [c for c in chain if c[2]]
        if len(body_lines) < 2:
            for c in chain:
                out.append(c[0])
            i = j
            continue

        max_end = max(c[1] for c in body_lines)

        for line, cond_end, body in chain:
            if body:
                prefix = line[:cond_end].rstrip()
                padding = ' ' * (max_end - len(prefix))
                out.append(prefix + padding + ' ' + body + '\n')
            else:
                out.append(line)

        i = j
    return out


# ── case body merging ─────────────────────────────────────────────────────────

def merge_case_bodies(lines):
    """Merge expanded single-case blocks back to one line if they fit in COL."""
    out = []
    i = 0
    while i < len(lines):
        if not CASE_BODY_RE.match(lines[i]):
            out.append(lines[i])
            i += 1
            continue
        case_line = lines[i]
        case_indent = len(case_line) - len(case_line.lstrip())
        stmts, j = [], i + 1
        while j < len(lines):
            line = lines[j]
            if not line.strip():
                break
            if len(line) - len(line.lstrip()) <= case_indent:
                break
            stmts.append(line.strip())
            j += 1
        if not stmts or not TERMINATOR_RE.search(stmts[-1]):
            out.append(case_line)
            i += 1
            continue
        merged = case_line.rstrip() + ' ' + ' '.join(stmts) + '\n'
        if len(merged.rstrip()) <= COL:
            out.append(merged)
            i = j
        else:
            out.append(case_line)
            i += 1
    return out


# ── case/default body alignment ──────────────────────────────────────────────

CASE_INLINE_RE = re.compile(r'^(\s*)(case\s+\S+\s*:|default\s*:)\s*(.+)$')

def align_case_chain(lines):
    """Align bodies of consecutive inline case/default lines to the same column."""
    out = []
    i = 0
    while i < len(lines):
        if not CASE_INLINE_RE.match(lines[i]):
            out.append(lines[i])
            i += 1
            continue
        chain = []  # (raw_line, label_end, body)
        j = i
        while j < len(lines):
            m = CASE_INLINE_RE.match(lines[j])
            if not m:
                break
            label_end = len(m.group(1)) + len(m.group(2))
            chain.append((lines[j], label_end, m.group(3).strip()))
            j += 1
        if len(chain) < 2:
            for c in chain:
                out.append(c[0])
            i = j
            continue
        max_end = max(c[1] for c in chain)
        for raw, label_end, body in chain:
            prefix = raw[:label_end].rstrip()
            padding = ' ' * (max_end - len(prefix))
            out.append(prefix + padding + ' ' + body + '\n')
        i = j
    return out


# ── enum value packing ───────────────────────────────────────────────────────

def pack_enum_values(lines):
    """Pack consecutive plain enum values onto lines within COL."""
    out = []
    i = 0
    while i < len(lines):
        m = ENUM_VAL_RE.match(lines[i])
        if not m or '=' in lines[i]:
            out.append(lines[i])
            i += 1
            continue
        indent_str = m.group(1)
        values = []
        while i < len(lines):
            m2 = ENUM_VAL_RE.match(lines[i])
            if not m2 or '=' in lines[i]:
                break
            values.append(m2.group(2) + m2.group(3))
            i += 1
        cur = indent_str
        for val in values:
            piece = val + ' '
            if len((cur + piece).rstrip()) > COL and cur.strip():
                out.append(cur.rstrip() + '\n')
                cur = indent_str
            cur += piece
        if cur.strip():
            out.append(cur.rstrip() + '\n')
    return out


# ── designated initializer packing ───────────────────────────────────────────

def pack_designated_init(lines):
    """Pack multiple [KEY] = val, items per line within COL."""
    out = []
    i = 0
    while i < len(lines):
        if not DESIG_RE.match(lines[i]):
            out.append(lines[i])
            i += 1
            continue
        indent_str = lines[i][: len(lines[i]) - len(lines[i].lstrip())]
        items = []
        while i < len(lines):
            m = DESIG_RE.match(lines[i])
            if not m:
                break
            key  = m.group(2)
            val  = m.group(3).rstrip()
            has_comma = ',' in m.group(4)
            items.append(f'[{key}] = {val}{"," if has_comma else ""}')
            i += 1
        cur = indent_str
        for item in items:
            piece = item + ' '
            if len((cur + piece).rstrip()) > COL and cur.strip():
                out.append(cur.rstrip() + '\n')
                cur = indent_str
            cur += piece
        if cur.strip():
            out.append(cur.rstrip() + '\n')
    return out


# ── struct initializer packing ───────────────────────────────────────────────

def pack_struct_init(lines):
    """Pack one-per-line {key, VAL} struct initializer entries onto lines <= COL_INIT."""
    out = []
    i = 0
    while i < len(lines):
        m = STRUCT_ENTRY_RE.match(lines[i])
        if not m:
            out.append(lines[i])
            i += 1
            continue
        indent_str = m.group(1)
        entries = []
        while i < len(lines):
            m2 = STRUCT_ENTRY_RE.match(lines[i])
            if not m2 or m2.group(1) != indent_str:
                break
            entries.append(m2.group(2))
            i += 1
        cur = indent_str
        for entry in entries:
            piece = entry + ' '
            if len((cur + piece).rstrip()) > COL_INIT and cur.strip():
                out.append(cur.rstrip() + '\n')
                cur = indent_str
            cur += piece
        if cur.strip():
            out.append(cur.rstrip() + '\n')
    return out


# ── pipeline ──────────────────────────────────────────────────────────────────

def merge(lines):
    return pack_struct_init(pack_designated_init(pack_enum_values(align_if_chain(align_case_chain(merge_case_bodies(pack(normalize(lines))))))))


for path in sys.argv[1:]:
    with open(path) as f:
        lines = f.readlines()
    with open(path, 'w') as f:
        f.writelines(merge(lines))
