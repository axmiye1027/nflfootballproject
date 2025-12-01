import React, { useState } from 'react'

export default function StadiumEditForm({ stadium, onSave, onCancel }) {
  const [form, setForm] = useState({ ...stadium })
  return (
    <div style={{ marginTop: 8 }}>
      <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: 8 }}>
        <div>
          <label>
            Team name
            <input value={form.teamName || ''} onChange={(e) => setForm({ ...form, teamName: e.target.value })} />
          </label>
        </div>
        <div>
          <label>
            Stadium name
            <input value={form.stadiumName || ''} onChange={(e) => setForm({ ...form, stadiumName: e.target.value })} />
          </label>
        </div>
        <div>
          <label>
            Location
            <input value={form.location || ''} onChange={(e) => setForm({ ...form, location: e.target.value })} />
          </label>
        </div>
        <div>
          <label>
            Capacity
            <input type='number' value={form.capacity || 0} onChange={(e) => setForm({ ...form, capacity: Number(e.target.value) })} />
          </label>
        </div>
        <div>
          <label>
            Year Opened
            <input type='number' value={form.yearOpened || ''} onChange={(e) => setForm({ ...form, yearOpened: Number(e.target.value) })} />
          </label>
        </div>
        <div>
          <label>
            Conference
            <input value={form.conference || ''} onChange={(e) => setForm({ ...form, conference: e.target.value })} />
          </label>
        </div>
      </div>
      <div style={{ marginTop: 8, display: 'flex', gap: 8 }}>
        <button onClick={() => onSave(form)}>Save</button>
        <button onClick={onCancel}>Cancel</button>
      </div>
    </div>
  )
}
