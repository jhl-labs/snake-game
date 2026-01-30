"""MDX 파일을 파싱하여 마크다운 텍스트와 이미지 참조를 추출합니다."""

import re
from dataclasses import dataclass, field


@dataclass
class ParsedPage:
    """파싱된 MDX 페이지 데이터."""
    title: str = ""
    markdown: str = ""
    image_refs: list[str] = field(default_factory=list)
    raw_source: str = ""


def extract_title(content: str) -> str:
    """<Meta title="..."/> 에서 제목을 추출합니다."""
    match = re.search(r'<Meta\s+title="([^"]+)"\s*/>', content)
    return match.group(1) if match else "Untitled"


def _remove_imports(content: str) -> str:
    """import 문을 제거합니다."""
    return re.sub(r"^import\s+.*$", "", content, flags=re.MULTILINE)


def _remove_meta(content: str) -> str:
    """<Meta .../> 태그를 제거합니다."""
    return re.sub(r"<Meta\s+[^/]*/\s*>", "", content)


def _convert_mermaid(content: str) -> str:
    """<Mermaid chart={`...`}/> → ```mermaid 코드 블록으로 변환합니다."""
    def replacer(m):
        chart = m.group(1).strip()
        return f"```mermaid\n{chart}\n```"
    return re.sub(
        r'<Mermaid\s+chart=\{`(.*?)`\}\s*/>',
        replacer,
        content,
        flags=re.DOTALL,
    )


def _convert_source(content: str) -> str:
    """<Source language="..." code={`...`}/> → ```lang 코드 블록으로 변환합니다."""
    def replacer(m):
        lang = m.group(1) or ""
        code = m.group(2).strip()
        return f"```{lang}\n{code}\n```"
    return re.sub(
        r'<Source\s+language="([^"]*)"\s+code=\{`(.*?)`\}\s*/>',
        replacer,
        content,
        flags=re.DOTALL,
    )


def _convert_exercise_card(content: str) -> str:
    """<ExerciseCard .../> → 구조화된 텍스트로 변환합니다."""
    def replacer(m):
        tag = m.group(0)
        number = re.search(r'number=\{(\d+)\}', tag)
        title = re.search(r'title="([^"]*)"', tag)
        difficulty = re.search(r'difficulty="([^"]*)"', tag)
        description = re.search(r'description="([^"]*)"', tag)
        hint = re.search(r'hint="([^"]*)"', tag)

        parts = []
        num = number.group(1) if number else "?"
        ttl = title.group(1) if title else ""
        diff = f" [{difficulty.group(1)}]" if difficulty else ""
        parts.append(f"**실습 {num}: {ttl}{diff}**")
        if description:
            parts.append(f"\n{description.group(1)}")
        if hint:
            parts.append(f"\n> **힌트:** {hint.group(1)}")
        return "\n".join(parts)

    return re.sub(
        r'<ExerciseCard\s+[^>]*/>',
        replacer,
        content,
        flags=re.DOTALL,
    )


def _remove_components(content: str) -> str:
    """ChapterNav, Placeholder, ComparisonTable 등 제거합니다."""
    content = re.sub(r'<ChapterNav\s+[^/]*/\s*>', '', content, flags=re.DOTALL)
    content = re.sub(r'<Placeholder\s*/\s*>', '', content)
    content = re.sub(r'<ComparisonTable\s*/\s*>', '', content)
    return content


def _extract_image_refs(content: str) -> list[str]:
    """이미지 참조를 추출합니다 (img 태그 + markdown 이미지)."""
    refs = []
    # <img src="..."/>
    for m in re.finditer(r'<img\s+[^>]*src="([^"]+)"', content):
        refs.append(m.group(1))
    # ![...](...)
    for m in re.finditer(r'!\[[^\]]*\]\(([^)]+)\)', content):
        refs.append(m.group(1))
    return refs


def _convert_img_tags(content: str) -> str:
    """<img src="..." alt="..." .../> → markdown 이미지로 변환합니다."""
    def replacer(m):
        tag = m.group(0)
        src_match = re.search(r'src="([^"]+)"', tag)
        alt_match = re.search(r'alt="([^"]*)"', tag)
        src = src_match.group(1) if src_match else ""
        alt = alt_match.group(1) if alt_match else ""
        return f"![{alt}]({src})"
    return re.sub(r'<img\s+[^>]*/?>', replacer, content, flags=re.DOTALL)


def _remove_jsx_divs(content: str) -> str:
    """JSX div 블록을 단순 텍스트로 변환합니다.

    style 속성이 있는 <div> 태그를 제거하고 내용만 남깁니다.
    """
    # style={{ ... }}가 있는 div 태그를 빈 문자열로 치환
    content = re.sub(
        r'<div\s+style=\{\{[^}]*\}\}\s*>',
        '',
        content,
        flags=re.DOTALL,
    )
    content = re.sub(r'</div>', '', content)
    # <strong>, <p> 등 HTML 태그는 markdown에서도 렌더링되므로 유지
    # 단, JSX style 속성 제거
    content = re.sub(
        r'<(\w+)\s+style=\{\{[^}]*\}\}\s*>',
        r'<\1>',
        content,
        flags=re.DOTALL,
    )
    return content


def parse_mdx(content: str) -> ParsedPage:
    """MDX 콘텐츠를 파싱하여 ParsedPage를 반환합니다."""
    page = ParsedPage()
    page.raw_source = content
    page.title = extract_title(content)
    page.image_refs = _extract_image_refs(content)

    md = content
    md = _remove_imports(md)
    md = _remove_meta(md)
    md = _convert_mermaid(md)
    md = _convert_source(md)
    md = _convert_exercise_card(md)
    md = _remove_components(md)
    md = _convert_img_tags(md)
    md = _remove_jsx_divs(md)

    # 연속 빈 줄 정리
    md = re.sub(r'\n{3,}', '\n\n', md).strip()
    page.markdown = md
    return page
