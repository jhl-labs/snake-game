import React from 'react';

interface PlaceholderProps {
  type: 'image' | 'video';
  alt: string;
  width?: string;
  height?: string;
}

export const Placeholder: React.FC<PlaceholderProps> = ({
  type,
  alt,
  width = '100%',
  height = type === 'video' ? '400px' : '280px'
}) => {
  const isImage = type === 'image';

  const iconPath = isImage
    ? 'M4 16l4.586-4.586a2 2 0 012.828 0L16 16m-2-2l1.586-1.586a2 2 0 012.828 0L20 14m-6-6h.01M6 20h12a2 2 0 002-2V6a2 2 0 00-2-2H6a2 2 0 00-2 2v12a2 2 0 002 2z'
    : 'M14.752 11.168l-3.197-2.132A1 1 0 0010 9.87v4.263a1 1 0 001.555.832l3.197-2.132a1 1 0 000-1.664z M21 12a9 9 0 11-18 0 9 9 0 0118 0z';

  const bgColor = isImage ? '#f1f5f9' : '#fef3c7';
  const borderColor = isImage ? '#cbd5e1' : '#fbbf24';
  const textColor = isImage ? '#64748b' : '#92400e';

  return (
    <div
      style={{
        width,
        height,
        backgroundColor: bgColor,
        border: `2px dashed ${borderColor}`,
        borderRadius: '12px',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        margin: '1.5rem 0',
        color: textColor,
        gap: '0.75rem'
      }}
    >
      <svg
        width="48"
        height="48"
        fill="none"
        stroke="currentColor"
        viewBox="0 0 24 24"
        strokeWidth={1.5}
      >
        <path
          strokeLinecap="round"
          strokeLinejoin="round"
          d={iconPath}
        />
      </svg>
      <div style={{ textAlign: 'center', padding: '0 1rem' }}>
        <p style={{
          margin: 0,
          fontSize: '0.875rem',
          fontWeight: 600
        }}>
          [{isImage ? '스크린샷' : '영상'}]
        </p>
        <p style={{
          margin: '0.25rem 0 0 0',
          fontSize: '0.8rem',
          opacity: 0.8
        }}>
          {alt}
        </p>
      </div>
    </div>
  );
};

export default Placeholder;
