/**
 * @file fix-gif-loop.mjs
 * @brief GIF에 무한 루프(NETSCAPE2.0) 확장 블록을 삽입합니다.
 *
 * Storybook MDX에서 GIF 반복 재생은 파일 자체에 loop 정보가 있어야 확실합니다.
 * 이 스크립트는 GIF에 NETSCAPE2.0 Application Extension을 삽입/수정하여 loopCount=0(무한)으로 만듭니다.
 */
import fs from 'node:fs';

/**
 * "NETSCAPE2.0" Application Extension을 찾아 loopCount를 갱신하거나, 없으면 삽입합니다.
 *
 * @param {Uint8Array} data GIF 바이너리
 * @returns {{ output: Uint8Array, changed: boolean }}
 */
function ensureInfiniteLoop(data) {
  // "NETSCAPE2.0" 검사 (latin1 문자열로 검색)
  const text = Buffer.from(data).toString('latin1');
  const idx = text.indexOf('NETSCAPE2.0');
  if (idx >= 0) {
    // loopCount 바이트는 "NETSCAPE2.0" 이후의 고정 오프셋에 존재합니다.
    // 구조: 21 FF 0B "NETSCAPE2.0" 03 01 [lo] [hi] 00
    // idx는 문자열 'N' 위치를 가리킵니다.
    // 따라서 loopCount는 idx + 11(문자열 끝) + 2(03,01) 이후에 위치합니다.
    const loopLoOffset = idx + 13;
    const loopHiOffset = idx + 14;
    if (loopHiOffset >= data.length) {
      throw new Error('Invalid GIF: NETSCAPE2.0 block is truncated');
    }
    const out = Uint8Array.from(data);
    const changed = out[loopLoOffset] !== 0x00 || out[loopHiOffset] !== 0x00;
    out[loopLoOffset] = 0x00;
    out[loopHiOffset] = 0x00;
    return { output: out, changed };
  }

  // GIF 헤더/스크린 디스크립터 파싱
  // Header(6) + Logical Screen Descriptor(7) = 13 bytes
  if (data.length < 13) {
    throw new Error('Invalid GIF: too small');
  }
  const header = Buffer.from(data.slice(0, 6)).toString('ascii');
  if (header !== 'GIF87a' && header !== 'GIF89a') {
    throw new Error(`Invalid GIF header: ${header}`);
  }

  const packed = data[10]; // LSD packed field (overall offset 10)
  const hasGct = (packed & 0x80) !== 0;
  const gctSizeBits = packed & 0x07;
  const gctSize = hasGct ? 3 * (2 ** (gctSizeBits + 1)) : 0;

  const insertAt = 13 + gctSize;
  if (insertAt > data.length) {
    throw new Error('Invalid GIF: global color table exceeds file size');
  }

  // NETSCAPE2.0 Application Extension (loopCount=0 => infinite)
  const ext = Uint8Array.from([
    0x21, 0xFF, 0x0B,
    0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30, // "NETSCAPE2.0"
    0x03, 0x01, 0x00, 0x00, 0x00,
  ]);

  const out = new Uint8Array(data.length + ext.length);
  out.set(data.slice(0, insertAt), 0);
  out.set(ext, insertAt);
  out.set(data.slice(insertAt), insertAt + ext.length);
  return { output: out, changed: true };
}

/**
 * 디렉터리를 재귀 순회하며 *.gif 파일 경로 목록을 반환합니다.
 *
 * @param {string} dirPath 디렉터리 경로
 * @returns {string[]} gif 파일 경로 배열
 */
function listGifFilesRecursive(dirPath) {
  /** @type {string[]} */
  const results = [];
  const entries = fs.readdirSync(dirPath, { withFileTypes: true });

  for (const entry of entries) {
    const fullPath = `${dirPath}/${entry.name}`;
    if (entry.isDirectory()) {
      results.push(...listGifFilesRecursive(fullPath));
      continue;
    }

    if (entry.isFile() && entry.name.toLowerCase().endsWith('.gif')) {
      results.push(fullPath);
    }
  }

  return results;
}

/**
 * 단일 GIF 파일을 무한 루프로 보정합니다.
 *
 * @param {string} filePath GIF 파일 경로
 * @returns {{ changed: boolean, error?: string }}
 */
function fixOneGif(filePath) {
  try {
    const input = fs.readFileSync(filePath);
    const { output, changed } = ensureInfiniteLoop(new Uint8Array(input));
    if (changed) {
      fs.writeFileSync(filePath, Buffer.from(output));
    }
    return { changed };
  } catch (err) {
    const message = err instanceof Error ? err.message : String(err);
    return { changed: false, error: message };
  }
}

function main() {
  const targetPath = process.argv[2];
  if (!targetPath) {
    console.error('Usage: node scripts/fix-gif-loop.mjs <path/to.gif|dir>');
    process.exit(1);
  }

  const stat = fs.statSync(targetPath);

  /** @type {string[]} */
  let gifFiles = [];
  if (stat.isDirectory()) {
    gifFiles = listGifFilesRecursive(targetPath);
  } else if (stat.isFile()) {
    gifFiles = [targetPath];
  } else {
    console.error(`Error: not a file or directory: ${targetPath}`);
    process.exit(1);
  }

  let updated = 0;
  let unchanged = 0;
  let failed = 0;

  for (const filePath of gifFiles) {
    const result = fixOneGif(filePath);
    if (result.error) {
      failed += 1;
      // eslint-disable-next-line no-console
      console.warn(`Failed: ${filePath} (${result.error})`);
      continue;
    }
    if (result.changed) {
      updated += 1;
      // eslint-disable-next-line no-console
      console.log(`Updated: ${filePath}`);
    } else {
      unchanged += 1;
    }
  }

  // eslint-disable-next-line no-console
  console.log(`Done. updated=${updated}, unchanged=${unchanged}, failed=${failed}`);
  if (failed > 0) {
    process.exit(1);
  }
}

main();

