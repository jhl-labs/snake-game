import { create } from '@storybook/theming/create';

export default create({
  base: 'light',

  // 브랜딩
  brandTitle: 'AI Agent 교육',
  brandUrl: 'https://jhl-labs.github.io/snake-game/',
  brandTarget: '_self',

  // 색상
  colorPrimary: '#6366f1',
  colorSecondary: '#8b5cf6',

  // UI
  appBg: '#f8fafc',
  appContentBg: '#ffffff',
  appBorderColor: '#e2e8f0',
  appBorderRadius: 8,

  // 타이포그래피
  fontBase: '"Pretendard Variable", "Noto Sans KR", -apple-system, BlinkMacSystemFont, sans-serif',
  fontCode: '"JetBrains Mono", "Fira Code", "Consolas", monospace',

  // 텍스트 색상
  textColor: '#1e293b',
  textInverseColor: '#f8fafc',

  // 바 색상
  barTextColor: '#64748b',
  barSelectedColor: '#6366f1',
  barBg: '#ffffff'
});
