import { addons } from '@storybook/manager-api';
import theme from './theme';

addons.setConfig({
  theme,
  sidebar: {
    showRoots: true,
    collapsedRoots: ['appendix']
  },
  toolbar: {
    title: { hidden: false },
    zoom: { hidden: true },
    eject: { hidden: true },
    copy: { hidden: true },
    fullscreen: { hidden: false }
  }
});
