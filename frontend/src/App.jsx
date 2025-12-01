// homepage / home button - first page to appear when opened

import { useEffect, useState } from 'react'
import { Routes, Route } from 'react-router-dom'
import './styles/App.css'

// components
import NavBar from './components/NavBar.jsx'
import SearchBar from './components/SearchBar.jsx'
import TeamInfo from './components/TeamInfo.jsx'
import LoginPage from './pages/login.jsx'
import TripPage from './pages/trip.jsx'
import SummaryPage from './pages/summary.jsx'
import AdminDash from './pages/adminDash.jsx'

function currency(n) {
  try { return n == null ? '' : `$${Number(n).toFixed(2)}` } catch { return String(n) }
}

function Home() {
  const [stadiumsData, setStadiumsData] = useState([])
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)
  const [selectedTeam, setSelectedTeam] = useState(null)
  const [filterOption, setFilterOption] = useState('allTeams')

  useEffect(() => {
    let mounted = true
    setLoading(true); setError(null)
    fetch('http://localhost:18080/stadiums')
      .then(res => { if (!res.ok) throw new Error(`HTTP ${res.status}`); return res.json() })
      .then(j => { if (!mounted) return; setStadiumsData(j.stadiums || []); setLoading(false) })
      .catch(err => { if (!mounted) return; setError(String(err)); setLoading(false) })
    return () => { mounted = false }
  }, [])

  function uniqueStadiumsMap(data) {
    const map = new Map()
    for (const s of data) {
      const key = (s.stadiumName || '').trim()
      if (!map.has(key)) map.set(key, s)
    }
    return map
  }

  const teamsSorted = [...stadiumsData].sort((a,b)=> (a.teamName||'').localeCompare(b.teamName||''))
  const stadiumsByName = [...uniqueStadiumsMap(stadiumsData).values()].sort((a,b)=> (a.stadiumName||'').localeCompare(b.stadiumName||''))
  const afcTeams = teamsSorted.filter(t => (t.conference||'').toLowerCase() === 'afc')
  const nfcTeams = teamsSorted.filter(t => (t.conference||'').toLowerCase() === 'nfc')
  const nfcNorthTeams = teamsSorted.filter(t => ((t.conference||'').toLowerCase() === 'nfc') && ((t.division||'').toLowerCase() === 'nfc north' || (t.division||'').toLowerCase() === 'north'))

  const openRoofStadiums = [...uniqueStadiumsMap(stadiumsData).values()].filter(s => (s.roofType||'').toLowerCase() === 'open').sort((a,b)=> (a.stadiumName||'').localeCompare(b.stadiumName||''))
  const openRoofCount = openRoofStadiums.length
  const byYear = [...uniqueStadiumsMap(stadiumsData).values()].sort((a,b)=> (Number(a.yearOpened||0) - Number(b.yearOpened||0)))
  const byCapacity = [...uniqueStadiumsMap(stadiumsData).values()].sort((a,b)=> (Number(a.capacity||0) - Number(b.capacity||0)))
  const totalCapacity = byCapacity.reduce((sum,s)=> sum + (Number(s.capacity||0)), 0)
  const byConference = [...stadiumsData].sort((a,b)=> ((a.conference||'').localeCompare(b.conference||'') || (a.teamName||'').localeCompare(b.teamName||'')))

  function handleJump(query) {
    if (!query) return
    const q = query.trim().toLowerCase()
    const found = teamsSorted.find(t => (t.teamName||'').toLowerCase().includes(q) || (t.stadiumName||'').toLowerCase().includes(q))
    if (found) setSelectedTeam(found)
  }

  function teamsForLeft() {
    switch (filterOption) {
      case 'afcTeams': return afcTeams
      case 'nfcTeams': return nfcTeams
      case 'nfcNorth': return nfcNorthTeams
      case 'stadiumsByName': return stadiumsByName
      case 'openRoofs': return openRoofStadiums
      case 'byYear': return byYear
      case 'byCapacity': return byCapacity
      case 'byConference': return byConference
      default: return teamsSorted
    }
  }

  const leftItems = teamsForLeft()

  return (
    <div>
      <div className="title" style={{ marginBottom: 12 }}>NFL Data Explorer</div>

      <div style={{ display: 'flex', gap: 12, marginBottom: 12, alignItems: 'center' }}>
        <div style={{ flex: 1 }}>
          <SearchBar onSearch={handleJump} placeholder={'Jump to team or stadium'} />
        </div>
        <div>
          <select value={filterOption} onChange={(e)=> setFilterOption(e.target.value)}>
            <option value='teamDetails'>Team Details (select a team)</option>
            <option value='allTeams'>All Teams (by team name)</option>
            <option value='stadiumsByName'>Stadiums (by stadium name)</option>
            <option value='afcTeams'>AFC Teams</option>
            <option value='nfcTeams'>NFC Teams</option>
            <option value='nfcNorth'>NFC North Teams</option>
            <option value='openRoofs'>Open Roof Stadiums</option>
            <option value='byYear'>Stadiums by Year Opened</option>
            <option value='byCapacity'>Stadiums by Capacity</option>
            <option value='byConference'>Teams by Conference</option>
            <option value='teamSouvenirs'>Team Souvenirs (select a team)</option>
          </select>
        </div>
      </div>

      <div style={{ display: 'flex', gap: 12 }}>
        <div style={{ flex: '0 0 280px', border: '1px solid var(--border)', borderRadius: 6, overflow: 'auto', maxHeight: '70vh' }}>
          {loading ? <div style={{ padding: 12 }}>Loading...</div> : error ? <div style={{ padding: 12, color: 'var(--danger)' }}>{error}</div> : (
            <div>
              {leftItems.map((t, idx) => (
                <div key={t.teamName || t.stadiumName || idx} onClick={() => setSelectedTeam(t)} style={{ padding: 8, cursor: 'pointer', borderBottom: '1px solid var(--border)', background: selectedTeam && selectedTeam.teamName === t.teamName ? 'rgba(0,0,0,0.03)' : 'transparent' }}>
                  <div style={{ fontWeight: 600 }}>{t.teamName || t.stadiumName}</div>
                  {t.stadiumName && <div style={{ color: 'var(--muted)' }}>{t.stadiumName}</div>}
                </div>
              ))}
            </div>
          )}
        </div>

        <div style={{ flex: 1, border: '1px solid var(--border)', borderRadius: 6, padding: 12, minHeight: '60vh', overflow: 'auto' }}>
          {!selectedTeam ? (
            <div style={{ padding: 8 }}>Select a team from the left to view its details.</div>
          ) : (
            <div>
              <h2>{selectedTeam.teamName}</h2>
              <p><strong>Stadium:</strong> {selectedTeam.stadiumName}</p>
              <p><strong>Capacity:</strong> {Number(selectedTeam.capacity || 0).toLocaleString()}</p>
              <p><strong>Location:</strong> {selectedTeam.location}</p>
              <p><strong>Conference:</strong> {selectedTeam.conference}</p>
              <p><strong>Division:</strong> {selectedTeam.division}</p>
              <p><strong>Surface:</strong> {selectedTeam.surface}</p>
              <p><strong>Roof:</strong> {selectedTeam.roofType}</p>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

function App() {
  const [showLogin, setShowLogin] = useState(false)
  const [auth, setAuth] = useState({ isAuthenticated: false, isAdmin: false, user: null })
  const [showAdmin, setShowAdmin] = useState(false)

  // measure nav height so future sticky elements can be positioned (kept simple for now)
  useEffect(() => {
    function updateTop() {
      // placeholder in case we later compute offsets
    }
    window.addEventListener('resize', updateTop)
    updateTop()
    return () => window.removeEventListener('resize', updateTop)
  }, [])

  return (
    <div>
      <NavBar />
      <div style={{ padding: 16 }}>
        <Routes>
          <Route path='/' element={<Home />} />
          <Route path='/login' element={<LoginPage />} />
          <Route path='/trip' element={<TripPage />} />
          <Route path='/summary' element={<SummaryPage />} />
          <Route path='/admin' element={<AdminDash />} />
        </Routes>
      </div>
    </div>
  )
}

export default App
