import type { Preview } from '@storybook/react';

const preview: Preview = {
  parameters: {
    controls: {
      matchers: {
        color: /(background|color)$/i,
        date: /Date$/i,
      },
    },
    options: {
      storySort: {
        order: [
          'Introduction',
          'Chapters',
          [
            '01. 소개 및 환경 설정',
            '02. Agent 기본 개념',
            '03. Rule 파일 작성',
            '04. Command 파일 작성',
            '05. CLAUDE.md 및 agent.md',
            '06. Subagent와 Agent Skills',
            '07. Test Case 작성 실습',
            '08. 버그 수정 실습'
          ],
          'Exercises',
          'Appendix'
        ]
      }
    },
    docs: {
      toc: true
    }
  }
};

export default preview;
