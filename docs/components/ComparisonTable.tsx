import React from 'react';

interface ComparisonRow {
  feature: string;
  cursor: string;
  claudeCode: string;
}

const comparisonData: ComparisonRow[] = [
  {
    feature: '인터페이스',
    cursor: 'GUI (VS Code 기반)',
    claudeCode: 'CLI (터미널)'
  },
  {
    feature: '규칙 파일',
    cursor: '.cursor/rules/*.mdc',
    claudeCode: 'CLAUDE.md, .claude/'
  },
  {
    feature: '커맨드',
    cursor: '.cursor/commands/*.md',
    claudeCode: '/ 슬래시 커맨드'
  },
  {
    feature: '서브에이전트',
    cursor: 'Background Agent',
    claudeCode: 'Task tool + subagent'
  },
  {
    feature: '모델',
    cursor: '다양한 모델 선택',
    claudeCode: 'Claude 모델 전용'
  },
  {
    feature: '가격',
    cursor: '월 $20 (Pro)',
    claudeCode: 'API 사용량 기반'
  }
];

const tableStyles: React.CSSProperties = {
  width: '100%',
  borderCollapse: 'collapse',
  marginTop: '1rem',
  marginBottom: '1.5rem',
  fontSize: '0.9rem'
};

const thStyles: React.CSSProperties = {
  padding: '12px 16px',
  textAlign: 'left',
  borderBottom: '2px solid #e2e8f0',
  backgroundColor: '#f8fafc',
  fontWeight: 600,
  color: '#374151'
};

const tdStyles: React.CSSProperties = {
  padding: '12px 16px',
  borderBottom: '1px solid #e2e8f0',
  color: '#4b5563'
};

const codeStyles: React.CSSProperties = {
  backgroundColor: '#f1f5f9',
  padding: '2px 6px',
  borderRadius: '4px',
  fontFamily: '"JetBrains Mono", monospace',
  fontSize: '0.85em'
};

export const ComparisonTable: React.FC = () => {
  return (
    <div style={{ overflowX: 'auto' }}>
      <table style={tableStyles}>
        <thead>
          <tr>
            <th style={thStyles}>특징</th>
            <th style={{ ...thStyles, color: '#7c3aed' }}>Cursor</th>
            <th style={{ ...thStyles, color: '#ea580c' }}>Claude Code</th>
          </tr>
        </thead>
        <tbody>
          {comparisonData.map((row, index) => (
            <tr key={index} style={{
              backgroundColor: index % 2 === 0 ? '#ffffff' : '#fafafa'
            }}>
              <td style={{ ...tdStyles, fontWeight: 500 }}>{row.feature}</td>
              <td style={tdStyles}>
                {row.cursor.includes('/') || row.cursor.includes('.') ? (
                  <code style={codeStyles}>{row.cursor}</code>
                ) : (
                  row.cursor
                )}
              </td>
              <td style={tdStyles}>
                {row.claudeCode.includes('/') || row.claudeCode.includes('.') ? (
                  <code style={codeStyles}>{row.claudeCode}</code>
                ) : (
                  row.claudeCode
                )}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default ComparisonTable;
