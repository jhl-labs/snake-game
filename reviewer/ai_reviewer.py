"""Ollama API를 통한 AI 리뷰 및 채팅 기능."""

from openai import OpenAI

MODEL = "qwen3-vl:235b-instruct-cloud"
BASE_URL = "http://localhost:11434/v1"
API_KEY = "ollama"  # Ollama는 API 키가 필요 없지만 SDK 요구사항

REVIEW_SYSTEM_PROMPT = """당신은 교육 자료 리뷰 전문가입니다. Storybook 기반 AI Agent 교육 자료를 리뷰합니다.

다음 관점에서 리뷰해주세요:
1. **콘텐츠 품질**: 내용의 정확성, 완성도, 논리적 흐름
2. **교육 효과**: 학습 목표 달성 가능성, 난이도 적절성, 실습 유무
3. **이미지 적절성**: 이미지가 설명에 도움이 되는지, 품질은 적절한지
4. **개선 제안**: 구체적인 개선 방안 (추가할 내용, 수정할 부분 등)

리뷰는 구조화된 마크다운으로 작성하고, 한국어로 답변하세요."""

CHAT_SYSTEM_PROMPT = """당신은 교육 자료에 대해 질의응답하는 AI 어시스턴트입니다.
현재 보고 있는 교육 페이지의 내용을 바탕으로 질문에 답변합니다.
한국어로 답변하세요."""


def _get_client() -> OpenAI:
    """OpenAI 클라이언트를 생성합니다."""
    return OpenAI(base_url=BASE_URL, api_key=API_KEY)


def _build_content_with_images(text: str, images: list[dict]) -> list[dict]:
    """텍스트와 이미지를 멀티모달 content로 구성합니다."""
    content = [{"type": "text", "text": text}]
    for img in images:
        data_uri = f"data:{img['media_type']};base64,{img['base64']}"
        content.append({
            "type": "image_url",
            "image_url": {"url": data_uri},
        })
    return content


def review_page(markdown: str, images: list[dict]):
    """페이지 콘텐츠를 리뷰합니다 (스트리밍).

    Args:
        markdown: 파싱된 페이지 마크다운
        images: [{"base64": str, "media_type": str}, ...]

    Yields:
        응답 텍스트 청크
    """
    client = _get_client()

    user_text = f"다음 교육 페이지를 리뷰해주세요:\n\n---\n{markdown}\n---"
    if images:
        user_text += f"\n\n(이 페이지에는 {len(images)}개의 이미지가 포함되어 있습니다. 아래 이미지들을 함께 확인해주세요.)"

    user_content = _build_content_with_images(user_text, images)

    stream = client.chat.completions.create(
        model=MODEL,
        messages=[
            {"role": "system", "content": REVIEW_SYSTEM_PROMPT},
            {"role": "user", "content": user_content},
        ],
        stream=True,
        temperature=0.7,
        max_tokens=4096,
    )

    for chunk in stream:
        if chunk.choices and chunk.choices[0].delta.content:
            yield chunk.choices[0].delta.content


def chat(messages: list[dict], page_markdown: str, images: list[dict]):
    """페이지 컨텍스트를 유지하며 채팅합니다 (스트리밍).

    Args:
        messages: 대화 히스토리 [{"role": ..., "content": ...}, ...]
        page_markdown: 현재 페이지 마크다운
        images: 현재 페이지 이미지

    Yields:
        응답 텍스트 청크
    """
    client = _get_client()

    context_text = f"현재 페이지 내용:\n\n---\n{page_markdown}\n---"
    context_content = _build_content_with_images(context_text, images)

    api_messages = [
        {"role": "system", "content": CHAT_SYSTEM_PROMPT},
        {"role": "user", "content": context_content},
        {"role": "assistant", "content": "네, 이 페이지의 내용을 확인했습니다. 질문해주세요."},
    ]
    api_messages.extend(messages)

    stream = client.chat.completions.create(
        model=MODEL,
        messages=api_messages,
        stream=True,
        temperature=0.7,
        max_tokens=4096,
    )

    for chunk in stream:
        if chunk.choices and chunk.choices[0].delta.content:
            yield chunk.choices[0].delta.content
