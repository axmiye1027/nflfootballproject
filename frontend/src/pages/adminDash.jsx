import React, { useState, useEffect } from 'react'
import { useNavigate } from 'react-router-dom'
import SearchBar from '../components/SearchBar'
import StadiumEditForm from '../components/StadiumEditForm'

export default function AdminDash() {
  const [tab, setTab] = useState('stadiums')
  const [stadiums, setStadiums] = useState([])
  const [stadiumsLoading, setStadiumsLoading] = useState(false)
  const [stadiumsError, setStadiumsError] = useState(null)
  const [souvenirs, setSouvenirs] = useState([])
  const [editing, setEditing] = useState({})
  const [searchTerm, setSearchTerm] = useState('')
  const [highlightedId, setHighlightedId] = useState(null)
  const [searchTop, setSearchTop] = useState(8)
  const [message, setMessage] = useState(null)
  const navigate = useNavigate()

  useEffect(() => {
    // placeholder: guard for real auth later
    const isAdmin = sessionStorage.getItem('isAdmin') === 'true'
    if (!isAdmin) {
      // not blocking navigation — this is a preview skeleton
    }
    // load stadiums when admin opens the dashboard
    if (tab === 'stadiums' || tab === 'souvenirs') {
      loadStadiums()
    }
  }, [])

  // compute sticky top offset so the search bar doesn't overlap the NavBar
  useEffect(() => {
    function updateTop() {
      const nav = document.querySelector('nav')
      const navH = nav ? nav.offsetHeight : 0
      setSearchTop((navH || 0) + 8)
    }
    updateTop()
    window.addEventListener('resize', updateTop)
    return () => window.removeEventListener('resize', updateTop)
  }, [])

  async function loadStadiums() {
    setStadiumsLoading(true)
    setStadiumsError(null)
    try {
      const res = await fetch('http://localhost:18080/stadiums')
      if (!res.ok) throw new Error(`HTTP ${res.status}`)
      const j = await res.json()
      const loaded = (j && j.stadiums) ? j.stadiums : []
      setStadiums(loaded)
      // build a global souvenirs list (unique by name)
      const map = new Map()
      for (const s of loaded) {
        for (const sv of (s.souvenirs || [])) {
          const key = (sv.souvenirName || '').trim()
          if (!key) continue
          if (!map.has(key)) {
            map.set(key, { souvenirId: sv.souvenirId || Date.now() + Math.random(), souvenirName: sv.souvenirName, souvenirPrice: sv.souvenirPrice })
          }
        }
      }
      setSouvenirs(Array.from(map.values()))
    } catch (err) {
      setStadiumsError(String(err))
    } finally {
      setStadiumsLoading(false)
    }
  }

  function performSearch() {
    if (!searchTerm || !searchTerm.trim()) return
    const q = searchTerm.trim().toLowerCase()
    const found = stadiums.find(s => (s.teamName || '').toLowerCase().includes(q) || (s.stadiumName || '').toLowerCase().includes(q))
    if (found) {
      const el = document.getElementById(`stad-${found.stadiumId}`)
      if (el) el.scrollIntoView({ behavior: 'smooth', block: 'center' })
      setHighlightedId(found.stadiumId)
      setTimeout(() => setHighlightedId(null), 3000)
    } else {
      setMessage('No stadium found')
    }
  }

  // Save stadium info (attempt API PUT then update local state)
  async function saveStadiumInfo(id, updated) {
    setMessage(null)
    try {
      const res = await fetch(`http://localhost:18080/stadiums/${id}`, {
        method: 'PUT', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(updated)
      })
      if (!res.ok) {
        // API might not support PUT; fall back to local update
        throw new Error(`HTTP ${res.status}`)
      }
      const j = await res.json()
      // update local list with returned object if available
      setStadiums(prev => prev.map(s => s.stadiumId === id ? (j || updated) : s))
      setEditing(prev => ({ ...prev, [id]: false }))
      setMessage('Saved to server')
    } catch (err) {
      // fallback: local only
      setStadiums(prev => prev.map(s => s.stadiumId === id ? ({ ...s, ...updated }) : s))
      setEditing(prev => ({ ...prev, [id]: false }))
      setMessage('Saved locally (server update failed)')
    }
  }

  // Add stadium from uploaded JSON (attempt API POST, else local)
  async function addStadiumFromJson(obj) {
    setMessage(null)
    try {
      const res = await fetch('http://localhost:18080/stadiums', { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(obj) })
      if (!res.ok) throw new Error(`HTTP ${res.status}`)
      const j = await res.json()
      setStadiums(prev => [...prev, j])
      setMessage('Uploaded and added (server)')
    } catch (err) {
      // fallback: generate temporary id and add locally
      const tempId = Math.max(0, ...stadiums.map(s => Number(s.stadiumId) || 0)) + 1
      const withId = { ...obj, stadiumId: tempId }
      setStadiums(prev => [...prev, withId])
      setMessage('Added locally (server POST failed)')
    }
  }

  return (
    <div style={{ padding: 16 }}>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 16 }}>
        <h2>Admin Dashboard (Preview)</h2>
        <div style={{ display: 'flex', gap: 8 }}>
          <button onClick={() => { sessionStorage.removeItem('isAdmin'); navigate('/') }} style={{ cursor: 'pointer' }}>Exit Admin Preview</button>
        </div>
      </div>

      <div style={{ display: 'flex', gap: 8, marginBottom: 12 }}>
        <button onClick={() => setTab('souvenirs')} style={{ fontWeight: tab === 'souvenirs' ? 700 : 400 }}>Souvenirs</button>
        <button onClick={() => setTab('stadiums')} style={{ fontWeight: tab === 'stadiums' ? 700 : 400 }}>Stadiums</button>
      </div>

      <div style={{ border: '1px solid var(--border)', borderRadius: 8, padding: 12, background: 'var(--panel)' }}>
        {/* Overview removed per request */}

        {tab === 'souvenirs' && (
          <div>
            <h3>Manage Souvenirs (Global)</h3>
            <p style={{ color: 'var(--muted)' }}>Souvenirs are managed globally here — add, edit price/name, or delete. Changes attempt server update and fall back to local state.</p>
            <div style={{ marginTop: 8 }}>
              <button style={{ marginRight: 8 }} onClick={() => { setMessage(null); loadStadiums() }}>Refresh</button>
            </div>

            {stadiumsLoading ? <div>Loading…</div> : (
              <div style={{ display: 'grid', gap: 12 }}>
                {souvenirs.map(sv => (
                  <div key={`gs-${sv.souvenirId}`} style={{ border: '1px solid var(--border)', padding: 8, borderRadius: 6, display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                    <div>
                      <div style={{ fontWeight: 600 }}>{sv.souvenirName}</div>
                      <div style={{ color: 'var(--muted)' }}>{currency(Number(sv.souvenirPrice))}</div>
                    </div>
                    <div style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
                      <input type='text' defaultValue={sv.souvenirName} id={`g-name-${sv.souvenirId}`} style={{ width: 220 }} />
                      <input type='number' defaultValue={Number(sv.souvenirPrice)} id={`g-price-${sv.souvenirId}`} style={{ width: 96 }} />
                      <button onClick={async () => {
                        const nameEl = document.getElementById(`g-name-${sv.souvenirId}`)
                        const priceEl = document.getElementById(`g-price-${sv.souvenirId}`)
                        const newName = nameEl?.value || ''
                        const newPrice = Number(priceEl?.value || 0)
                        if (!newName) { setMessage('Provide a name'); return }
                        try {
                          const res = await fetch(`http://localhost:18080/souvenirs/${sv.souvenirId}`, { method: 'PUT', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ souvenirName: newName, souvenirPrice: newPrice }) })
                          if (!res.ok) throw new Error(`HTTP ${res.status}`)
                          setMessage('Updated on server')
                          // update local global list
                          setSouvenirs(prev => prev.map(x => x.souvenirId === sv.souvenirId ? ({ ...x, souvenirName: newName, souvenirPrice: newPrice }) : x))
                        } catch (err) {
                          setSouvenirs(prev => prev.map(x => x.souvenirId === sv.souvenirId ? ({ ...x, souvenirName: newName, souvenirPrice: newPrice }) : x))
                          setMessage('Updated locally (server failed)')
                        }
                      }}>Save</button>
                      <button onClick={async () => {
                        // delete global souvenir
                        try {
                          const res = await fetch(`http://localhost:18080/souvenirs/${sv.souvenirId}`, { method: 'DELETE' })
                          if (!res.ok) throw new Error(`HTTP ${res.status}`)
                          setSouvenirs(prev => prev.filter(x => x.souvenirId !== sv.souvenirId))
                          setMessage('Deleted on server')
                        } catch (err) {
                          setSouvenirs(prev => prev.filter(x => x.souvenirId !== sv.souvenirId))
                          setMessage('Deleted locally (server failed)')
                        }
                      }} style={{ cursor: 'pointer' }}>Delete</button>
                    </div>
                  </div>
                ))}

                <div style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
                  <input placeholder='New souvenir name' id={`global-new-name`} />
                  <input placeholder='Price' id={`global-new-price`} type='number' />
                  <button onClick={async () => {
                    const nameEl = document.getElementById('global-new-name')
                    const priceEl = document.getElementById('global-new-price')
                    const name = nameEl?.value || ''
                    const price = Number(priceEl?.value || 0)
                    if (!name) { setMessage('Provide a name'); return }
                    try {
                      const res = await fetch(`http://localhost:18080/souvenirs`, { method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ souvenirName: name, souvenirPrice: price }) })
                      if (!res.ok) throw new Error(`HTTP ${res.status}`)
                      const nj = await res.json()
                      setSouvenirs(prev => [...prev, nj])
                      setMessage('Added on server')
                    } catch (err) {
                      const tempId = Date.now()
                      setSouvenirs(prev => [...prev, { souvenirId: tempId, souvenirName: name, souvenirPrice: price }])
                      setMessage('Added locally (server failed)')
                    }
                  }}>Add</button>
                </div>
              </div>
            )}
          </div>
        )}

        {tab === 'stadiums' && (
          <div>
            <h3>Manage Stadiums</h3>
            <p style={{ color: 'var(--muted)' }}>Edit stadium metadata, capacity, location, etc. Upload a JSON file to add teams (expected JSON array or single object).</p>
            <div style={{ marginTop: 8, display: 'flex', flexDirection: 'column', gap: 8 }}>

              <div style={{ display: 'flex', gap: 8, alignItems: 'center' }}>
                <input type='file' id='stadium-upload' accept='.json' />
                <button onClick={async () => {
                  const input = document.getElementById('stadium-upload')
                  if (!input || !input.files || input.files.length === 0) { setMessage('Choose a file'); return }
                  const file = input.files[0]
                  try {
                    const text = await file.text()
                    const parsed = JSON.parse(text)
                    if (Array.isArray(parsed)) {
                      for (const obj of parsed) await addStadiumFromJson(obj)
                    } else if (typeof parsed === 'object') {
                      await addStadiumFromJson(parsed)
                    } else {
                      setMessage('JSON root must be object or array')
                    }
                  } catch (err) { setMessage('Invalid JSON file') }
                }}>Upload JSON</button>
                <button onClick={() => loadStadiums()}>Refresh from server</button>
              </div>

              <div style={{ marginTop: 8, position: 'sticky', top: `${searchTop}px`, zIndex: 30, background: 'var(--panel)', padding: 8, borderRadius: 6 }}>
                <SearchBar
                  inputId='stadium-search'
                  value={searchTerm}
                  onChange={(v) => { setSearchTerm(v); setMessage(null) }}
                  onSearch={() => performSearch()}
                  placeholder='Search stadium or team name'
                />
              </div>

              <div style={{ marginTop: 12 }}>
                {stadiumsLoading && <div>Loading stadiums…</div>}
                {stadiumsError && <div style={{ color: 'var(--danger)' }}>{stadiumsError}</div>}
                {!stadiumsLoading && !stadiumsError && (
                  <div style={{ display: 'grid', gap: 12 }}>
                    {stadiums.map(s => (
                      <div id={`stad-${s.stadiumId}`} key={`stad-${s.stadiumId}`} style={{ border: (s.stadiumId === highlightedId ? '2px solid var(--accent)' : '1px solid var(--border)'), padding: 8, borderRadius: 6 }}>
                        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                          <div style={{ fontWeight: 700 }}>{s.teamName} — {s.stadiumName}</div>
                          <div>
                            {!editing[s.stadiumId] ? (
                              <button onClick={() => setEditing(prev => ({ ...prev, [s.stadiumId]: true }))}>Edit</button>
                            ) : (
                              <button onClick={() => setEditing(prev => ({ ...prev, [s.stadiumId]: false }))}>Cancel</button>
                            )}
                          </div>
                        </div>
                        {!editing[s.stadiumId] ? (
                          <div style={{ marginTop: 8 }}>
                            <div><strong>Location:</strong> {s.location}</div>
                            <div><strong>Capacity:</strong> {s.capacity}</div>
                            <div><strong>Opened:</strong> {s.yearOpened}</div>
                          </div>
                        ) : (
                          <StadiumEditForm stadium={s} onSave={(upd) => saveStadiumInfo(s.stadiumId, upd)} onCancel={() => setEditing(prev => ({ ...prev, [s.stadiumId]: false }))} />
                        )}
                      </div>
                    ))}
                  </div>
                )}
              </div>
            </div>
          </div>
        )}

        {/* Import tab removed per request */}
      </div>
    </div>
  )
}

// StadiumEditForm moved to `frontend/src/components/StadiumEditForm.jsx`

function currency(n) { return '$' + Number(n || 0).toFixed(2) }
