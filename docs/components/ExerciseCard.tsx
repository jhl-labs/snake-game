import React from 'react';

type Difficulty = '초급' | '중급' | '고급';

interface ExerciseCardProps {
  number: number;
  title: string;
  difficulty: Difficulty;
  hint?: string;
  description?: string;
}

const difficultyConfig: Record<Difficulty, { color: string; bg: string }> = {
  '초급': { color: '#15803d', bg: '#dcfce7' },
  '중급': { color: '#b45309', bg: '#fef3c7' },
  '고급': { color: '#dc2626', bg: '#fee2e2' }
};

export const ExerciseCard: React.FC<ExerciseCardProps> = ({
  number,
  title,
  difficulty,
  hint,
  description
}) => {
  const { color, bg } = difficultyConfig[difficulty];

  return (
    <div
      style={{
        border: '1px solid #e2e8f0',
        borderRadius: '12px',
        padding: '16px 20px',
        marginBottom: '12px',
        backgroundColor: '#ffffff',
        boxShadow: '0 1px 3px rgba(0,0,0,0.05)'
      }}
    >
      <div style={{
        display: 'flex',
        alignItems: 'flex-start',
        gap: '14px'
      }}>
        <span
          style={{
            backgroundColor: '#6366f1',
            color: '#ffffff',
            borderRadius: '50%',
            width: '32px',
            height: '32px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            fontWeight: 700,
            fontSize: '0.875rem',
            flexShrink: 0
          }}
        >
          {number}
        </span>
        <div style={{ flex: 1, minWidth: 0 }}>
          <div style={{
            display: 'flex',
            alignItems: 'center',
            gap: '10px',
            flexWrap: 'wrap'
          }}>
            <h4 style={{
              margin: 0,
              fontSize: '1rem',
              fontWeight: 600,
              color: '#1f2937'
            }}>
              {title}
            </h4>
            <span
              style={{
                backgroundColor: bg,
                color: color,
                padding: '3px 10px',
                borderRadius: '12px',
                fontSize: '0.7rem',
                fontWeight: 600,
                textTransform: 'uppercase',
                letterSpacing: '0.025em'
              }}
            >
              {difficulty}
            </span>
          </div>
          {description && (
            <p style={{
              margin: '8px 0 0 0',
              fontSize: '0.875rem',
              color: '#4b5563',
              lineHeight: 1.5
            }}>
              {description}
            </p>
          )}
          {hint && (
            <p style={{
              margin: '10px 0 0 0',
              padding: '8px 12px',
              backgroundColor: '#eff6ff',
              borderRadius: '6px',
              fontSize: '0.8rem',
              color: '#1e40af',
              lineHeight: 1.4
            }}>
              <strong>힌트:</strong> {hint}
            </p>
          )}
        </div>
      </div>
    </div>
  );
};

export default ExerciseCard;
