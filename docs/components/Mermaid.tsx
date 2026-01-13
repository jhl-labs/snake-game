import React, { useEffect, useRef, useState } from 'react';
import mermaid from 'mermaid';

interface MermaidProps {
  chart: string;
}

mermaid.initialize({
  startOnLoad: false,
  theme: 'default',
  securityLevel: 'loose',
});

let idCounter = 0;

export const Mermaid: React.FC<MermaidProps> = ({ chart }) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const [svg, setSvg] = useState<string>('');
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const renderChart = async () => {
      if (!chart || !containerRef.current) return;

      try {
        const id = `mermaid-${idCounter++}`;
        const { svg } = await mermaid.render(id, chart.trim());
        setSvg(svg);
        setError(null);
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Mermaid rendering failed');
        setSvg('');
      }
    };

    renderChart();
  }, [chart]);

  if (error) {
    return (
      <div style={{
        padding: '1rem',
        backgroundColor: '#fee2e2',
        borderRadius: '8px',
        color: '#dc2626',
        fontFamily: 'monospace',
        fontSize: '0.875rem',
      }}>
        <strong>Mermaid Error:</strong> {error}
        <pre style={{ marginTop: '0.5rem', whiteSpace: 'pre-wrap' }}>{chart}</pre>
      </div>
    );
  }

  return (
    <div
      ref={containerRef}
      style={{
        display: 'flex',
        justifyContent: 'center',
        margin: '1.5rem 0',
        overflow: 'auto',
      }}
      dangerouslySetInnerHTML={{ __html: svg }}
    />
  );
};
