"""이미지 참조를 파일로 매핑하고 base64로 인코딩합니다."""

import os
import base64
import glob
from io import BytesIO
from PIL import Image


# 지원 이미지 확장자
IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".gif", ".webp", ".svg"}
MAX_DIMENSION = 1024


def build_image_index(public_dir: str) -> dict[str, str]:
    """docs/public/ 디렉터리의 이미지를 인덱싱합니다.

    Returns:
        {파일이름: 절대경로} 매핑
    """
    index = {}
    if not os.path.isdir(public_dir):
        return index

    for fp in glob.glob(os.path.join(public_dir, "*")):
        ext = os.path.splitext(fp)[1].lower()
        if ext in IMAGE_EXTENSIONS:
            name = os.path.basename(fp)
            index[name] = os.path.abspath(fp)
    return index


def resolve_image_path(ref: str, image_index: dict[str, str], public_dir: str) -> str | None:
    """이미지 참조를 실제 파일 경로로 해석합니다.

    Args:
        ref: MDX에서 추출한 이미지 참조 (예: "001.gif", "./images/foo.png")
        image_index: build_image_index 결과
        public_dir: docs/public 경로

    Returns:
        절대 파일 경로 또는 None
    """
    # 직접 이름 매칭
    basename = os.path.basename(ref)
    if basename in image_index:
        return image_index[basename]

    # 상대 경로로 시도
    candidate = os.path.join(public_dir, ref)
    if os.path.isfile(candidate):
        return os.path.abspath(candidate)

    return None


def _resize_image(img: Image.Image) -> Image.Image:
    """이미지를 최대 크기로 리사이즈합니다."""
    w, h = img.size
    if w <= MAX_DIMENSION and h <= MAX_DIMENSION:
        return img
    ratio = min(MAX_DIMENSION / w, MAX_DIMENSION / h)
    new_size = (int(w * ratio), int(h * ratio))
    return img.resize(new_size, Image.LANCZOS)


def image_to_base64(file_path: str) -> tuple[str, str] | None:
    """이미지 파일을 base64 문자열로 변환합니다.

    GIF는 첫 프레임을 PNG로 변환합니다.

    Returns:
        (base64_string, media_type) 또는 None
    """
    if not os.path.isfile(file_path):
        return None

    ext = os.path.splitext(file_path)[1].lower()

    try:
        if ext == ".svg":
            with open(file_path, "r", encoding="utf-8") as f:
                svg_data = f.read()
            b64 = base64.b64encode(svg_data.encode("utf-8")).decode("ascii")
            return b64, "image/svg+xml"

        img = Image.open(file_path)

        # GIF → 첫 프레임 PNG
        if ext == ".gif":
            img.seek(0)
            img = img.convert("RGBA")

        img = _resize_image(img)
        img = img.convert("RGB")

        buf = BytesIO()
        img.save(buf, format="PNG")
        b64 = base64.b64encode(buf.getvalue()).decode("ascii")
        return b64, "image/png"
    except Exception:
        return None


def resolve_and_encode(refs: list[str], image_index: dict[str, str], public_dir: str) -> list[dict]:
    """이미지 참조 목록을 해석하고 base64 인코딩합니다.

    Returns:
        [{"ref": str, "path": str, "base64": str, "media_type": str}, ...]
    """
    results = []
    seen = set()
    for ref in refs:
        if ref in seen:
            continue
        seen.add(ref)

        path = resolve_image_path(ref, image_index, public_dir)
        if path is None:
            continue

        encoded = image_to_base64(path)
        if encoded is None:
            continue

        b64, media_type = encoded
        results.append({
            "ref": ref,
            "path": path,
            "base64": b64,
            "media_type": media_type,
        })
    return results
