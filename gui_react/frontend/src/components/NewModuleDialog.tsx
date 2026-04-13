import { useState } from 'react';
import { api } from '../api';

interface Props {
  onClose: () => void;
}

export default function NewModuleDialog({ onClose }: Props) {
  const [form, setForm] = useState({
    module_name: '',
    module_version: '0.0.0.',
    author_nick_name: 'nick2',
    author_full_name: 'Nick Smith',
    module_description: '',
    device_address: '0x3F',
    comm_periph: 'i2c',
    licence: 'MIT',
    package_name: 'chsm',
    linked_libs: '',
    module_location: '',
  });
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState('');

  const set = (key: string, val: string) => setForm((f) => ({ ...f, [key]: val }));

  const browseLoc = async () => {
    const res = await api.pickFile('open', '*');
    if (res.filepath) set('module_location', res.filepath);
  };

  const generate = async () => {
    if (!form.module_name.trim()) { setError('Module name is required'); return; }
    if (!form.module_location.trim()) { setError('Module location is required'); return; }
    setBusy(true);
    setError('');
    try {
      const res = await api.newModule(form);
      if (res.error) setError(res.error);
      else onClose();
    } catch (e: unknown) {
      setError(e instanceof Error ? e.message : String(e));
    } finally {
      setBusy(false);
    }
  };

  return (
    <div className="modal-overlay" onClick={onClose}>
      <div className="modal-dialog" onClick={(e) => e.stopPropagation()}>
        <h3>New Module</h3>

        <fieldset>
          <legend>Module</legend>
          <table className="form-table">
            <tbody>
              <tr>
                <td><label>Module name</label></td>
                <td><input value={form.module_name} onChange={(e) => set('module_name', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Module version</label></td>
                <td><input value={form.module_version} onChange={(e) => set('module_version', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Author Nick Name</label></td>
                <td><input value={form.author_nick_name} onChange={(e) => set('author_nick_name', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Author Full Name</label></td>
                <td><input value={form.author_full_name} onChange={(e) => set('author_full_name', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Description</label></td>
                <td><textarea value={form.module_description} onChange={(e) => set('module_description', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Device Address</label></td>
                <td><input value={form.device_address} onChange={(e) => set('device_address', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Comm Peripheral</label></td>
                <td>
                  <select value={form.comm_periph} onChange={(e) => set('comm_periph', e.target.value)}>
                    <option value="i2c">I2C</option>
                    <option value="spi">SPI</option>
                    <option value="can">CAN</option>
                    <option value="none">None</option>
                  </select>
                </td>
              </tr>
              <tr>
                <td><label>Licence</label></td>
                <td>
                  <select value={form.licence} onChange={(e) => set('licence', e.target.value)}>
                    <option value="MIT">MIT</option>
                    <option value="BSD-3">BSD-3</option>
                    <option value="GPL-3.0">GNU GPL v3.0</option>
                    <option value="Apache-2.0">Apache Software Licence 2.0</option>
                    <option value="MPL-2.0">Mozilla Public Licence 2.0</option>
                  </select>
                </td>
              </tr>
              <tr>
                <td><label>Location</label></td>
                <td>
                  <input value={form.module_location} onChange={(e) => set('module_location', e.target.value)} />
                </td>
                <td><button type="button" onClick={browseLoc}>Browse</button></td>
              </tr>
            </tbody>
          </table>
        </fieldset>

        <fieldset>
          <legend>CMake Generation</legend>
          <table className="form-table">
            <tbody>
              <tr>
                <td><label>Package Name</label></td>
                <td><input value={form.package_name} onChange={(e) => set('package_name', e.target.value)} /></td>
              </tr>
              <tr>
                <td><label>Linked Libs</label></td>
                <td><textarea value={form.linked_libs} onChange={(e) => set('linked_libs', e.target.value)} /></td>
              </tr>
            </tbody>
          </table>
        </fieldset>

        {error && <div className="form-error">{error}</div>}

        <div className="modal-buttons">
          <button onClick={generate} disabled={busy}>{busy ? 'Generating...' : 'Generate'}</button>
          <button onClick={onClose}>Cancel</button>
        </div>
      </div>
    </div>
  );
}
