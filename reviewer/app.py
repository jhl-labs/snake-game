"""강의자료 리뷰어 - Streamlit 메인 앱."""

import os
import sys
import streamlit as st

# 모듈 경로 추가
sys.path.insert(0, os.path.dirname(__file__))

from mdx_parser import parse_mdx
from page_manager import scan_pages, group_by_chapter
from image_resolver import build_image_index, resolve_and_encode
from ai_reviewer import review_page, chat

# ── 경로 설정 ──────────────────────────────────────────
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
DOCS_ROOT = os.path.join(PROJECT_ROOT, "docs")
PUBLIC_DIR = os.path.join(DOCS_ROOT, "public")

# ── 페이지 설정 ────────────────────────────────────────
st.set_page_config(
    page_title="강의자료 리뷰어",
    page_icon="📖",
    layout="wide",
    initial_sidebar_state="expanded",
)


# ── 세션 상태 초기화 ───────────────────────────────────
def init_session_state():
    """세션 상태를 초기화합니다."""
    if "page_index" not in st.session_state:
        st.session_state.page_index = 0
    if "parse_cache" not in st.session_state:
        st.session_state.parse_cache = {}
    if "review_cache" not in st.session_state:
        st.session_state.review_cache = {}
    if "chat_histories" not in st.session_state:
        st.session_state.chat_histories = {}
    if "image_index" not in st.session_state:
        st.session_state.image_index = build_image_index(PUBLIC_DIR)
    if "pages" not in st.session_state:
        st.session_state.pages = scan_pages(DOCS_ROOT)


init_session_state()

pages = st.session_state.pages
image_index = st.session_state.image_index

if not pages:
    st.error("MDX 파일을 찾을 수 없습니다. docs/stories/ 디렉터리를 확인하세요.")
    st.stop()


# ── 페이지 데이터 로드 ─────────────────────────────────
def get_parsed_page(idx: int):
    """파싱된 페이지를 캐시에서 가져오거나 새로 파싱합니다."""
    if idx in st.session_state.parse_cache:
        return st.session_state.parse_cache[idx]

    page_info = pages[idx]
    with open(page_info.file_path, "r", encoding="utf-8") as f:
        content = f.read()

    parsed = parse_mdx(content)
    st.session_state.parse_cache[idx] = parsed
    return parsed


def get_page_images(parsed_page):
    """페이지의 이미지를 해석하고 인코딩합니다."""
    return resolve_and_encode(parsed_page.image_refs, image_index, PUBLIC_DIR)


# ── 사이드바: 네비게이션 ───────────────────────────────
with st.sidebar:
    st.title("📖 강의자료 리뷰어")
    st.divider()

    # 챕터별 트리 네비게이션
    grouped = group_by_chapter(pages)

    for chapter, chapter_pages in grouped.items():
        with st.expander(chapter, expanded=(chapter == pages[st.session_state.page_index].chapter)):
            for cp in chapter_pages:
                global_idx = pages.index(cp)
                is_current = global_idx == st.session_state.page_index
                label = f"{'▶ ' if is_current else '  '}{cp.title}"
                if st.button(
                    label,
                    key=f"nav_{global_idx}",
                    use_container_width=True,
                    type="primary" if is_current else "secondary",
                ):
                    st.session_state.page_index = global_idx
                    st.rerun()

    st.divider()

    # 이전/다음 버튼
    col_prev, col_info, col_next = st.columns([1, 2, 1])
    with col_prev:
        if st.button("◀ 이전", disabled=(st.session_state.page_index == 0)):
            st.session_state.page_index -= 1
            st.rerun()
    with col_info:
        st.caption(f"{st.session_state.page_index + 1} / {len(pages)}")
    with col_next:
        if st.button("다음 ▶", disabled=(st.session_state.page_index >= len(pages) - 1)):
            st.session_state.page_index += 1
            st.rerun()


# ── 현재 페이지 데이터 ─────────────────────────────────
current_idx = st.session_state.page_index
parsed = get_parsed_page(current_idx)
page_images = get_page_images(parsed)

# ── 메인 레이아웃 (3:2) ───────────────────────────────
col_content, col_review = st.columns([3, 2])

# ── 좌측: 콘텐츠 표시 ─────────────────────────────────
with col_content:
    st.header(parsed.title)

    tab_content, tab_source = st.tabs(["📄 콘텐츠", "📝 MDX 소스"])

    with tab_content:
        # 마크다운 렌더링
        st.markdown(parsed.markdown, unsafe_allow_html=True)

        # 이미지 표시
        if page_images:
            st.divider()
            st.subheader("이미지")
            for img_data in page_images:
                st.image(
                    f"data:{img_data['media_type']};base64,{img_data['base64']}",
                    caption=img_data["ref"],
                    use_container_width=True,
                )

    with tab_source:
        st.code(parsed.raw_source, language="markdown", line_numbers=True)


# ── 우측: AI 리뷰 + 채팅 ──────────────────────────────
with col_review:
    st.header("AI 리뷰")

    tab_review, tab_chat = st.tabs(["🔍 리뷰", "💬 채팅"])

    # ── 리뷰 탭 ───────────────────────────────────────
    with tab_review:
        if st.button("리뷰 요청", type="primary", use_container_width=True):
            with st.spinner("AI가 리뷰 중입니다..."):
                review_container = st.empty()
                full_response = ""
                try:
                    for chunk in review_page(parsed.markdown, page_images):
                        full_response += chunk
                        review_container.markdown(full_response)
                    st.session_state.review_cache[current_idx] = full_response
                except Exception as e:
                    st.error(f"리뷰 요청 실패: {e}")

        # 캐시된 리뷰 표시
        if current_idx in st.session_state.review_cache:
            st.markdown(st.session_state.review_cache[current_idx])
        else:
            st.info("'리뷰 요청' 버튼을 클릭하여 AI 리뷰를 받아보세요.")

    # ── 채팅 탭 ───────────────────────────────────────
    with tab_chat:
        # 페이지별 대화 히스토리
        chat_key = f"chat_{current_idx}"
        if chat_key not in st.session_state.chat_histories:
            st.session_state.chat_histories[chat_key] = []

        history = st.session_state.chat_histories[chat_key]

        # 대화 히스토리 표시
        for msg in history:
            with st.chat_message(msg["role"]):
                st.markdown(msg["content"])

        # 채팅 입력
        if prompt := st.chat_input("이 페이지에 대해 질문하세요..."):
            history.append({"role": "user", "content": prompt})
            with st.chat_message("user"):
                st.markdown(prompt)

            with st.chat_message("assistant"):
                response_container = st.empty()
                full_response = ""
                try:
                    for chunk in chat(history, parsed.markdown, page_images):
                        full_response += chunk
                        response_container.markdown(full_response)
                    history.append({"role": "assistant", "content": full_response})
                except Exception as e:
                    st.error(f"채팅 요청 실패: {e}")

        if not history:
            st.info("이 페이지에 대해 자유롭게 질문하세요.")
