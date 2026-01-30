"""MDX 페이지 목록을 구성하고 정렬합니다."""

import os
import glob
from dataclasses import dataclass
from mdx_parser import extract_title


@dataclass
class PageInfo:
    """페이지 메타 정보."""
    file_path: str
    title: str
    chapter: str  # 예: "Introduction", "Chapters/01. 소개 및 환경 설정"
    sort_key: tuple  # 정렬 키


# preview.ts의 storySort order 기반 정렬 순서
TOP_LEVEL_ORDER = [
    "Introduction",
    "Chapters",
    "Exercises",
    "Appendix",
]

CHAPTER_ORDER = [
    "01. 소개 및 환경 설정",
    "02. Agent 기본 개념",
    "03. Rule 파일 작성",
    "04. Command 파일 작성",
    "05. CLAUDE.md 및 agent.md",
    "06. Subagent와 Agent Skills",
    "07. Test Case 작성 실습",
    "08. 버그 수정 실습",
]

# 디렉터리 이름 → 챕터 제목 매핑
DIR_TO_CHAPTER = {
    "01-introduction": "01. 소개 및 환경 설정",
    "02-agent-concepts": "02. Agent 기본 개념",
    "03-rule-files": "03. Rule 파일 작성",
    "04-command-files": "04. Command 파일 작성",
    "05-claude-md": "05. CLAUDE.md 및 agent.md",
    "06-subagent-skills": "06. Subagent와 Agent Skills",
    "07-mcp": "07. MCP",
    "08-github-actions": "08. GitHub Actions",
}


def _get_top_level_index(category: str) -> int:
    """최상위 카테고리의 정렬 인덱스를 반환합니다."""
    try:
        return TOP_LEVEL_ORDER.index(category)
    except ValueError:
        return len(TOP_LEVEL_ORDER)


def _get_chapter_index(chapter_name: str) -> int:
    """챕터의 정렬 인덱스를 반환합니다."""
    try:
        return CHAPTER_ORDER.index(chapter_name)
    except ValueError:
        return len(CHAPTER_ORDER)


def _classify_page(file_path: str, docs_root: str) -> tuple[str, str, tuple]:
    """파일 경로에서 카테고리, 챕터, 정렬 키를 결정합니다."""
    rel = os.path.relpath(file_path, os.path.join(docs_root, "stories"))
    parts = rel.replace("\\", "/").split("/")
    filename = parts[-1]

    # 파일명에서 숫자 접두사 추출 (예: "01-Overview.mdx" → 1)
    file_order = 999
    base = os.path.splitext(filename)[0]
    num_match = __import__("re").match(r"(\d+)", base)
    if num_match:
        file_order = int(num_match.group(1))

    if len(parts) == 1:
        # stories/ 직접 하위 파일 (예: Introduction.mdx)
        if base.lower() == "introduction":
            return "Introduction", "Introduction", (0, 0, 0)
        return base, base, (_get_top_level_index(base), 0, 0)

    if parts[0] == "chapters" and len(parts) >= 3:
        dir_name = parts[1]
        chapter = DIR_TO_CHAPTER.get(dir_name, dir_name)
        sort_key = (
            _get_top_level_index("Chapters"),
            _get_chapter_index(chapter),
            file_order,
        )
        return "Chapters", chapter, sort_key

    if parts[0] == "exercises":
        if len(parts) >= 3:
            sub_dir = parts[1]
            sort_key = (
                _get_top_level_index("Exercises"),
                hash(sub_dir) % 1000,
                file_order,
            )
            return "Exercises", f"Exercises/{sub_dir}", sort_key
        sort_key = (
            _get_top_level_index("Exercises"),
            0,
            file_order,
        )
        return "Exercises", "Exercises", sort_key

    return parts[0], "/".join(parts[:-1]), (99, 0, file_order)


def scan_pages(docs_root: str) -> list[PageInfo]:
    """docs 디렉터리에서 모든 MDX 파일을 스캔하고 정렬된 목록을 반환합니다.

    Args:
        docs_root: docs/ 디렉터리 경로

    Returns:
        정렬된 PageInfo 리스트
    """
    pattern = os.path.join(docs_root, "stories", "**", "*.mdx")
    mdx_files = glob.glob(pattern, recursive=True)

    pages = []
    for fp in mdx_files:
        with open(fp, "r", encoding="utf-8") as f:
            content = f.read()

        title = extract_title(content)
        category, chapter, sort_key = _classify_page(fp, docs_root)

        pages.append(PageInfo(
            file_path=fp,
            title=title,
            chapter=chapter,
            sort_key=sort_key,
        ))

    pages.sort(key=lambda p: p.sort_key)
    return pages


def group_by_chapter(pages: list[PageInfo]) -> dict[str, list[PageInfo]]:
    """페이지를 챕터별로 그룹화합니다.

    Returns:
        {챕터명: [PageInfo, ...]} (순서 유지)
    """
    groups: dict[str, list[PageInfo]] = {}
    for page in pages:
        if page.chapter not in groups:
            groups[page.chapter] = []
        groups[page.chapter].append(page)
    return groups
