import type { StorybookConfig } from '@storybook/react-vite';
import remarkGfm from 'remark-gfm';
import mdxMermaid from 'mdx-mermaid';

const config: StorybookConfig = {
  stories: [
    '../stories/**/*.mdx',
    '../stories/**/*.stories.@(js|jsx|ts|tsx)'
  ],
  addons: [
    '@storybook/addon-links',
    '@storybook/addon-essentials',
    {
      name: '@storybook/addon-docs',
      options: {
        mdxPluginOptions: {
          mdxCompileOptions: {
            remarkPlugins: [remarkGfm, mdxMermaid],
          },
        },
      },
    },
  ],
  framework: {
    name: '@storybook/react-vite',
    options: {}
  },
  docs: {
    defaultName: 'Documentation'
  },
  staticDirs: ['../public'],
  viteFinal: async (config) => {
    // GitHub Pages 배포 시 베이스 경로 설정
    if (process.env.NODE_ENV === 'production') {
      config.base = '/snake-game/';
    }
    return config;
  }
};

export default config;
