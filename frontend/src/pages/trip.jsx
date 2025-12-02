// Trip Planning Page / Trip button
import React, { useEffect, useMemo, useState } from 'react'
import { useNavigate } from 'react-router-dom'

export default function TripPage() {
    const [stadiums, setStadiums] = useState([])
    const [loading, setLoading] = useState(true)
    const [error, setError] = useState(null)

    const [selectedIds, setSelectedIds] = useState([])
    const [activeCity, setActiveCity] = useState('')
    const [startingId, setStartingId] = useState(null)

    useEffect(() => {
        let mounted = true
        setLoading(true)
        fetch('http://localhost:18080/stadiums')
            .then((res) => res.json())
            .then((json) => {
                // backend returns { stadiums: [...] }
                const list = (json && json.stadiums) ? json.stadiums : []
                if (mounted) setStadiums(list)
            })
            .catch((err) => {
                console.error('Failed to load stadiums', err)
                if (mounted) setError(err?.message || String(err))
            })
            .finally(() => mounted && setLoading(false))

        return () => { mounted = false }
    }, [])

    const stadiumsByCity = useMemo(() => {
        const map = {}
        stadiums.forEach((s) => {
            const loc = (s.location || '')
            const city = loc.split(',')[0].trim() || 'Unknown'
            if (!map[city]) map[city] = []
            map[city].push(s)
        })
        return map
    }, [stadiums])

    function addStadium(id) {
        setSelectedIds((prev) => (prev.includes(id) ? prev : [...prev, id]))
    }

    function removeStadium(id) {
        setSelectedIds((prev) => prev.filter((v) => v !== id))
    }

    // Drag & drop helpers
    function onDragStart(e, id, source) {
        const payload = JSON.stringify({ id, source })
        try {
            e.dataTransfer.setData('application/json', payload)
        } catch (err) {
            e.dataTransfer.setData('text/plain', payload)
        }
        e.dataTransfer.effectAllowed = 'move'
    }

    function onDropToStarting(e) {
        e.preventDefault()
        const raw = e.dataTransfer.getData('application/json') || e.dataTransfer.getData('text/plain')
        if (!raw) return
        let parsed
        try { parsed = JSON.parse(raw) } catch (err) { return }
        const { id, source } = parsed
        if (!id) return
        // When replacing the starting point we want the previous starting to go
        // into the Selected column (not back to Choose). Handle each source.
        const prevStart = startingId

        // If dragging from selected, remove the dragged id from selected and
        // move the previous starting (if any) into selected at the front.
        if (source === 'selected') {
            setSelectedIds((prev) => {
                const filtered = prev.filter((v) => v !== id)
                if (prevStart && prevStart !== id && !filtered.includes(prevStart)) {
                    return [prevStart, ...filtered]
                }
                return filtered
            })
            setStartingId(id)
            return
        }

        // If dragging from choose, set the new starting and move previous
        // starting into Selected (so it appears in the Selected column).
        if (source === 'choose') {
            if (prevStart) {
                setSelectedIds((prev) => (prev.includes(prevStart) ? prev : [prevStart, ...prev]))
            }
            setStartingId(id)
            return
        }
    }

    function onDropToSelectedList(e) {
        e.preventDefault()
        const raw = e.dataTransfer.getData('application/json') || e.dataTransfer.getData('text/plain')
        if (!raw) return
        let parsed
        try { parsed = JSON.parse(raw) } catch (err) { return }
        const { id, source } = parsed
        if (!id) return
        // If dropped from choose, add to selected
        if (source === 'choose') {
            addStadium(id)
            return
        }
        // If dropped from starting, clear starting and add to selected at front
        if (source === 'starting') {
            setStartingId(null)
            setSelectedIds((prev) => (prev.includes(id) ? prev : [id, ...prev]))
            return
        }
    }

    function onDropToChoose(e) {
        e.preventDefault()
        const raw = e.dataTransfer.getData('application/json') || e.dataTransfer.getData('text/plain')
        if (!raw) return
        let parsed
        try { parsed = JSON.parse(raw) } catch (err) { return }
        const { id, source } = parsed
        if (!id) return
        // If dragging from selected, remove it (moves back to choose)
        if (source === 'selected') {
            removeStadium(id)
            return
        }
        // If dragging from starting, clear starting (it will appear in choose)
        if (source === 'starting') {
            setStartingId(null)
            return
        }
    }

    const selectedStadiums = stadiums.filter((s) => selectedIds.includes(s.stadiumId))

    //STYLES
    const containerStyle = { display: 'flex', gap: '1rem', padding: '1rem' }
    const columnStyle = { flex: 1, border: '1px solid var(--border)', borderRadius: 8, padding: '0.75rem', minHeight: '12rem', background: 'var(--panel)' }
    const headerStyle = { fontWeight: '700', marginBottom: '0.5rem' }

    // container for teams
    const listStyle = { 
        display: 'flex',
        border: 'var(--border)',
        borderRadius: '1em',
        padding: '.5em'
    }

    // team box
    const teamStyle = {
        background: 'var(--muted)',
        border: 'var(--border)',
        borderRadius: '1em',
    }

    const cityButtonStyle = (active) => ({ padding: '0.4rem 0.6rem', cursor: 'pointer', borderRadius: 6, background: active ? 'var(--purple)' : 'transparent', color: active ? 'white' : 'var(--text)', border: '1px solid var(--border)' })

    // DISPLAY 
    const navigate = useNavigate()

    function handleCreateTrip() {
        // Prepare ordered stadiums as they appear in selectedIds
        const ordered = selectedIds.map((id) => stadiums.find((s) => s.stadiumId === id)).filter(Boolean)
        const payload = { stadiums: ordered, startingId }
        // persist to sessionStorage so Summary can reload on refresh
        try { sessionStorage.setItem('lastTrip', JSON.stringify(payload)) } catch (e) {}
        navigate('/summary', { state: payload })
    }

    return (
        <div>
            <div className='title'>Create a Trip</div>

            {/* Create Trip button */}
            <div style={{ width: '100%', padding: '0 1rem 1rem' }}>
                <button onClick={handleCreateTrip} disabled={selectedIds.length === 0} style={{ padding: '0.6rem 1rem', fontWeight: 700 }}>
                    Create Trip
                </button>
            </div>

            {loading && <div style={{ padding: '1rem' }}>Loading stadiums…</div>}
            {error && <div style={{ padding: '1rem', color: 'var(--danger)' }}>Error: {error}</div>}

            {!loading && !error && (
                <>
                <div style={{ padding: '1rem' }}>
                    {/* Selected (top) */}
                    <div style={columnStyle} onDragOver={(e) => e.preventDefault()} onDrop={onDropToChoose}>
                        <div style={headerStyle}>Selected</div>
                        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: '0.5rem' }}>
                            {/* starting card as first grid item */}
                            <div key='starting-card' style={{ minHeight: '4.5rem' }}>
                                <div onDragOver={(e) => e.preventDefault()} onDrop={onDropToStarting} className='teamStyle' style={{ ...teamStyle, padding: '0.5rem', borderRadius: 6, minHeight: '4.5rem', display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
                                    <div style={{ fontWeight: 700 }}>Starting point</div>
                                    <div style={{ marginTop: '0.25rem' }}>
                                        {startingId ? (
                                            (() => {
                                                const s = stadiums.find((st) => st.stadiumId === startingId)
                                                if (!s) return <div>Starting stadium not found.</div>
                                                return (
                                                    <div>
                                                        <div draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'starting')}>
                                                            <div style={{ fontWeight: 600 }}>{s.teamName}</div>
                                                            <div style={{ fontSize: '0.9rem', color: 'var(--muted)' }}>{s.stadiumName} — {s.location}</div>
                                                        </div>
                                                        <div style={{ marginTop: '0.5rem', textAlign: 'right' }}>
                                                            <button onClick={() => setStartingId(null)} style={{ cursor: 'pointer' }}>Remove</button>
                                                        </div>
                                                    </div>
                                                )
                                            })()
                                        ) : (
                                            <div>Drag a stadium here to set the starting point.</div>
                                        )}
                                    </div>
                                </div>
                            </div>

                            {selectedStadiums.map((s) => (
                                <div key={s.stadiumId} draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'selected')} className='teamStyle' style={{ ...teamStyle, padding: '0.5rem', borderRadius: 6, minHeight: '4.5rem', display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
                                    <div>
                                        <div style={{ fontWeight: 600 }}>{s.teamName || s.stadiumName}</div>
                                        <div style={{ fontSize: '0.9rem', color: 'var(--muted)' }}>{s.stadiumName} — {s.location}</div>
                                    </div>
                                    <div style={{ marginTop: '0.25rem', textAlign: 'right' }}>
                                        <button onClick={() => removeStadium(s.stadiumId)} style={{ cursor: 'pointer' }}>Remove</button>
                                    </div>
                                </div>
                            ))}
                        </div>
                    </div>

                    {/* Choose (below) */}
                    <div style={{ ...columnStyle, marginTop: '1rem' }} onDragOver={(e) => e.preventDefault()} onDrop={onDropToChoose}>
                        <div style={headerStyle}>Choose</div>
                        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: '0.5rem' }}>
                            {stadiums.filter(s => !selectedIds.includes(s.stadiumId) && s.stadiumId !== startingId).map((s) => (
                                <div key={s.stadiumId} className='teamStyle' draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'choose')} style={{ ...teamStyle, padding: '0.5rem', borderRadius: 6, minHeight: '4.5rem', display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
                                    <div>
                                        <div style={{ fontWeight: 600 }}>{s.teamName}</div>
                                        <div style={{ fontSize: '0.9rem', color: 'var(--muted)' }}>{s.stadiumName} — {s.location}</div>
                                    </div>
                                    <div style={{ marginTop: '0.25rem', textAlign: 'right' }}>
                                        <button onClick={() => addStadium(s.stadiumId)} style={{ cursor: 'pointer' }}>Add</button>
                                    </div>
                                </div>
                            ))}
                        </div>
                    </div>
                </div>

                    
                </>
            )}
        </div>
    )
}