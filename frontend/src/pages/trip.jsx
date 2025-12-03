// Trip Planning Page / Trip button
import React, { use, useEffect, useMemo, useState } from 'react'
import { useNavigate } from 'react-router-dom'

import '../styles/trip.css'

export default function TripPage() {
    const [stadiums, setStadiums] = useState([])
    const [loading, setLoading] = useState(true)
    const [error, setError] = useState(null)

    const [selectedIds, setSelectedIds] = useState([])
    const [activeCity, setActiveCity] = useState('')
    const [startingId, setStartingId] = useState(null)

    const [tripType, setTripType] = useState("customTrip");

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


    function buildTripRequest(tripType, startingId, stadiums) 
    {
        const startingStadium = stadiums.find(s => s.stadiumId === startingId);
        const selectedStadiums = stadiums.filter(s => selectedIds.includes(s.stadiumId));

        switch (tripType) {
            case "bfsTrip":
                return {
                    endpoint: "/bfsTrip",
                    body: { bfsStadium: startingStadium.stadiumName }
                };

            case "mstTrip":
                return {
                    endpoint: "/mstTrip",
                    body: { mstStadium: startingStadium.stadiumName }
                };

            case "dijkstraTrip":
                return {
                    endpoint: "/dijkstraTrip",
                    body: { stadiums: selectedStadiums, startingStadium }
                };

            case "dfsTrip":
                return {
                    endpoint: "/dfsTrip",
                    body: { dfsStadium: startingStadium.stadiumName }
                };

            case "recursiveTrip":
                return {
                    endpoint: "/recursiveTrip",
                    body: { stadiums: selectedStadiums, startingStadium }
                }

            case "customTrip":
            default:
                return {
                    endpoint: "/customTrip",
                    body: { stadiums: selectedStadiums, startingStadium }
                };
        }
    }

    async function handleCreateTrip() 
    {
        const { endpoint, body } = buildTripRequest(tripType, startingId, stadiums);

        try {
            const res = await fetch(`http://localhost:18080${endpoint}`, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(body)
            });

            const json = await res.json();
            console.log("Trip calculation result:", json);

            if (!json.success) {
                throw new Error(json.message || "Unknown error");
            }

            sessionStorage.setItem("tripResult", JSON.stringify(json));

            navigate("/summary", { state: { stadiums: json.stadiums, totalDistance: json.totalDistance } });

        } catch (err) {
            console.error("Failed to calculate trip:", err);
            setError(err.message);
        }
    }


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

    // function handleCreateTrip() {
    //     // Prepare ordered stadiums as they appear in selectedIds
    //     const ordered = selectedIds.map((id) => stadiums.find((s) => s.stadiumId === id)).filter(Boolean)
    //     const payload = { stadiums: ordered, startingId }
    //     // persist to sessionStorage so Summary can reload on refresh
    //     try { sessionStorage.setItem('lastTrip', JSON.stringify(payload)) } catch (e) {}
    //     navigate('/summary', { state: payload })
    // }

    const disableCreateTrip = (() => {
        const numSelected = selectedIds.length + (startingId ? 1 : 0);

        if (numSelected <= 0)
            return true;

        switch (tripType) {
            case 'dijkstraTrip':
                return numSelected != 2;
            case 'bfsTrip':
            case 'mstTrip':
            case 'dfsTrip':
                return numSelected > 1; 
            case 'customTrip':
            case 'recursiveTrip':
                return numSelected < 2;
            default: 
                return true;
        }
    })();

    return (
        <div>
            <div className='title'>Create a Trip</div>

            {/* Create Trip button */}
            <div className="header-row">
                <div className="createTripButton">
                    <button onClick={handleCreateTrip} disabled={disableCreateTrip}>
                        Create Trip
                    </button>
                </div>

                <div className="trip-type-container">
                    <label>Type of trip:</label>
                    <select value={tripType} onChange={e => setTripType(e.target.value)}>
                        <option value="customTrip">Custom trip</option>
                        <option value="dijkstraTrip">Shortest trip</option>
                        <option value="recursiveTrip">Recursive trip</option>
                        <option value="dfsTrip">Visit all teams</option>
                        <option value="bfsTrip">BFS</option>
                        <option value="mstTrip">MST</option>
                    </select>
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