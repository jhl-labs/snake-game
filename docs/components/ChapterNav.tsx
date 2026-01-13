import React from 'react';

interface ChapterNavProps {
  prevLink?: string;
  prevTitle?: string;
  nextLink?: string;
  nextTitle?: string;
}

const linkStyles: React.CSSProperties = {
  display: 'flex',
  flexDirection: 'column',
  textDecoration: 'none',
  padding: '12px 16px',
  borderRadius: '8px',
  transition: 'background-color 0.2s',
  maxWidth: '45%'
};

export const ChapterNav: React.FC<ChapterNavProps> = ({
  prevLink,
  prevTitle,
  nextLink,
  nextTitle
}) => {
  return (
    <div
      style={{
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'stretch',
        marginTop: '3rem',
        paddingTop: '1.5rem',
        borderTop: '1px solid #e2e8f0',
        gap: '1rem'
      }}
    >
      {prevLink ? (
        <a
          href={prevLink}
          style={{
            ...linkStyles,
            backgroundColor: '#f8fafc',
            alignItems: 'flex-start'
          }}
          onMouseOver={(e) => {
            e.currentTarget.style.backgroundColor = '#f1f5f9';
          }}
          onMouseOut={(e) => {
            e.currentTarget.style.backgroundColor = '#f8fafc';
          }}
        >
          <span style={{
            fontSize: '0.75rem',
            color: '#64748b',
            marginBottom: '4px'
          }}>
            이전
          </span>
          <span style={{
            fontSize: '0.9rem',
            color: '#6366f1',
            fontWeight: 500
          }}>
            ← {prevTitle}
          </span>
        </a>
      ) : (
        <div />
      )}

      {nextLink ? (
        <a
          href={nextLink}
          style={{
            ...linkStyles,
            backgroundColor: '#f8fafc',
            alignItems: 'flex-end',
            textAlign: 'right'
          }}
          onMouseOver={(e) => {
            e.currentTarget.style.backgroundColor = '#f1f5f9';
          }}
          onMouseOut={(e) => {
            e.currentTarget.style.backgroundColor = '#f8fafc';
          }}
        >
          <span style={{
            fontSize: '0.75rem',
            color: '#64748b',
            marginBottom: '4px'
          }}>
            다음
          </span>
          <span style={{
            fontSize: '0.9rem',
            color: '#6366f1',
            fontWeight: 500
          }}>
            {nextTitle} →
          </span>
        </a>
      ) : (
        <div />
      )}
    </div>
  );
};

export default ChapterNav;
