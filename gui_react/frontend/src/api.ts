/* ── Electron bridge (available when running inside Electron) ──── */
interface ChsmElectron {
  pickFile: (opts: { mode: string; filters?: { name: string; extensions: string[] }[] }) => Promise<string | null>;
  pickDirectory: () => Promise<string | null>;
  isElectron: boolean;
}
declare global {
  interface Window { chsmElectron?: ChsmElectron; }
}

const BASE = '';

async function request<T>(method: string, url: string, body?: unknown): Promise<T> {
  const opts: RequestInit = {
    method,
    headers: { 'Content-Type': 'application/json' },
  };
  if (body !== undefined) opts.body = JSON.stringify(body);
  const res = await fetch(`${BASE}${url}`, opts);
  return res.json() as Promise<T>;
}

export const api = {
  health: () => request<{ status: string; debug_connected: boolean }>('GET', '/api/health'),

  pickFile: async (mode: 'open' | 'save' = 'open', filetypes = '.h,.html'): Promise<{ ok: boolean; filepath?: string; cancelled?: boolean }> => {
    // Use Electron native dialog when available
    if (window.chsmElectron?.isElectron) {
      const exts = filetypes.split(',').map((e) => e.replace('.', ''));
      const path = await window.chsmElectron.pickFile({
        mode,
        filters: [{ name: 'Files', extensions: exts }],
      });
      if (path) return { ok: true, filepath: path };
      return { ok: false, cancelled: true };
    }
    return request<{ ok: boolean; filepath?: string; cancelled?: boolean }>(
      'GET',
      `/api/pick-file?mode=${mode}&filetypes=${encodeURIComponent(filetypes)}`,
    );
  },

  openFile: (filepath: string) =>
    request<{
      ok: boolean;
      model?: Record<string, unknown>;
      filename?: string;
      filepath?: string;
      html_path?: string;
      src_dir?: string;
      doc_dir?: string;
      file_cfg?: Record<string, unknown>;
      error?: string;
    }>('POST', `/api/open?filepath=${encodeURIComponent(filepath)}`),

  save: (filepath: string, model: Record<string, unknown>, svg = '') =>
    request<{ ok: boolean; filepath?: string; filename?: string; error?: string }>(
      'POST',
      '/api/save',
      { filepath, model, svg },
    ),

  codegen: (model: Record<string, unknown>, h_filepath: string, file_cfg: Record<string, unknown> = {}) =>
    request<{ ok: boolean; c_file?: string; h_file?: string; error?: string }>(
      'POST',
      '/api/codegen',
      { model, h_filepath, file_cfg },
    ),

  newModule: (data: Record<string, string>) =>
    request<{ ok: boolean; error?: string }>('POST', '/api/new-module', data),
};
