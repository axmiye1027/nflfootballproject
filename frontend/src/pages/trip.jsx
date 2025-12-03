// Trip Planning Page / Trip button
import React, { useEffect, useMemo, useState } from 'react'
import { useNavigate } from 'react-router-dom'

import '../styles/trip.css'

export default function TripPage() {
    const [stadiums, setStadiums] = useState([])
    const [loading, setLoading] = useState(true)
    const [error, setError] = useState(null)

    const [selectedIds, setSelectedIds] = useState([])
    const [activeCity, setActiveCity] = useState('')
    const [startingId, setStartingId] = useState(null)

    const [bfsCity, setBfsCity] = useState("")
    const [mstCity, setMstCity] = useState("")
    const [bfsResult, setBfsResult] = useState(null);
    const [mstResult, setMstResult] = useState(null);

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

    async function handleCalculate() {
        try {
            // Make sure we always send strings, not null/undefined
            const payload = {
                bfsCity: bfsCity || "",
                mstCity: mstCity || ""
            };

            const res = await fetch("http://localhost:18080/calculateDist", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(payload)
            });

            const json = await res.json();

            // Assume backend returns { bfs: number, mst: number }
            setBfsResult(json.bfs);
            setMstResult(json.mst);

        } catch (err) {
            console.error("Calculation error:", err);
        }
    }


    const selectedStadiums = stadiums.filter((s) => selectedIds.includes(s.stadiumId))

    //STYLES
    const containerStyle = { display: 'flex', gap: '1rem', padding: '1rem' }

    // container for teams
    const listStyle = { 
        display: 'flex',
        border: 'var(--border)',
        borderRadius: '1em',
        padding: '.5em'
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
            <div className="header-row">
                <div className="createTripButton">
                    <button onClick={handleCreateTrip} disabled={selectedIds.length === 0}>
                        Create Trip
                    </button>
                </div>

                <div className="trip-calc-box">

                    <div className="calc-item">
                        <label>BFS:</label>
                        <input 
                            type="text"
                            value={bfsCity}
                            onChange={(e) => { setBfsCity(e.target.value); setBfsResult(null); }}
                        />
                        {bfsResult !== null && <span className="calc-output"> {bfsResult} </span>}
                    </div>

                    <div className="calc-item">
                        <label>MST:</label>
                        <input 
                            type="text"
                            value={mstCity}
                            onChange={(e) => { setMstCity(e.target.value); setMstCity(null); }}
                        />
                        {mstResult !== null && <span className="calc-output"> {mstResult} </span>}
                    </div>

                    <button 
                        className="calc-button" 
                        onClick={handleCalculate}
                        disabled={!bfsCity && !mstCity}
                    >
                        Calculate
                    </button>

                    <div className="calc-output">
                        {bfsResult !== null && (
                            <div>Bfs Result: {bfsResult}</div>
                        )}
                        {mstResult !== null && (
                            <div>MST Result: {mstResult}</div>
                        )}
                    </div>

                </div>


            </div>
            {loading && <div className="loadingStyle">Loading stadiums…</div>}
            {error && <div className="errorStyle">Error: {error}</div>}

            {!loading && !error && (
                <>
                <div style={{ padding: '1rem' }}>
                    
                    {/* Selected (top) */}
                    <div className='columnStyle' onDragOver={(e) => e.preventDefault()} onDrop={onDropToChoose}>
                        <div className="headerStyle">Selected</div>
                        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: '0.5rem' }}>

                            {/* starting card as first grid item */}
                            <div key='starting-card' style={{ minHeight: '4.5rem' }}>
                                <div onDragOver={(e) => e.preventDefault()} onDrop={onDropToStarting} className="teamStyle">
                                    <div style={{ fontWeight: 700 }}>Starting point</div>
                                    <div style={{ marginTop: '0.25rem' }}>
                                        {startingId ? (
                                            (() => {
                                                const s = stadiums.find((st) => st.stadiumId === startingId)
                                                if (!s) return <div>Starting stadium not found.</div>
                                                return (
                                                    <div>
                                                        <div draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'starting')}>
                                                            <div className="nameStyle">{s.teamName}</div>
                                                            <div className="nameStyle">{s.stadiumName} — {s.location}</div>
                                                        </div>
                                                        <div style={{ marginTop: '0.5rem', textAlign: 'right' }}>
                                                            <button onClick={() => setStartingId(null)}>Remove</button>
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
                                <div className='teamStyle' key={s.stadiumId} draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'selected')}>
                                    <div>
                                        <div style={{ fontWeight: 600 }}>{s.teamName || s.stadiumName}</div>
                                        <div className="nameStyle">{s.stadiumName} — {s.location}</div>
                                    </div>
                                    <div style={{ marginTop: '0.25rem', textAlign: 'right' }}>
                                        <button onClick={() => removeStadium(s.stadiumId)} style={{ cursor: 'pointer' }}>Remove</button>
                                    </div>
                                </div>
                            ))}
                        </div>
                    </div>

                    {/* Choose (below) */}
                    <div className="columnStyle" onDragOver={(e) => e.preventDefault()} onDrop={onDropToChoose}>
                        <div className="headerStyle">Choose</div>
                        <div className="gridStyle">
                            {stadiums.filter(s => !selectedIds.includes(s.stadiumId) && s.stadiumId !== startingId).map((s) => (
                                <div key={s.stadiumId} className='teamStyle' draggable onDragStart={(e) => onDragStart(e, s.stadiumId, 'choose')}>
                                    <div>
                                        <div style={{ fontWeight: 600 }}>{s.teamName}</div>
                                        <div className="nameStyle">{s.stadiumName} — {s.location}</div>
                                    </div>
                                    <button onClick={() => addStadium(s.stadiumId)}>Add</button>
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