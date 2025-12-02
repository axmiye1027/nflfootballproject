import React, { useMemo, useState, useEffect } from 'react'
import { useLocation, useNavigate } from 'react-router-dom'

function currency(n) {
	return '$' + Number(n || 0).toFixed(2)
}

export default function SummaryPage() {
	const loc = useLocation()
	const navigate = useNavigate()
	const initial = loc.state || (() => {
		try { return JSON.parse(sessionStorage.getItem('lastTrip')) } catch (e) { return null }
	})()

	const stadiums = (initial && initial.stadiums) ? initial.stadiums : []
	const startingId = initial && initial.startingId ? initial.startingId : null

	const [activeId, setActiveId] = useState(stadiums.length > 0 ? (startingId || stadiums[0].stadiumId) : null)

	// stadiums to display in the summary. Ensure starting stadium is present here.
	const [displayStadiums, setDisplayStadiums] = useState(stadiums)

	// per-stadium cart items (user-selected quantities)
	const [souvenirs, setSouvenirs] = useState(() => {
		const map = {}
		stadiums.forEach(s => { map[s.stadiumId] = [] })
		return map
	})

	// server-provided souvenir catalog per stadium
	const [catalog, setCatalog] = useState(() => {
		const map = {}
		stadiums.forEach(s => { map[s.stadiumId] = [] })
		return map
	})

		// ensure starting stadium is included in displayStadiums; also update displayStadiums when payload changes
		useEffect(() => {
			setDisplayStadiums(stadiums)
			if (startingId && !stadiums.find(s => s.stadiumId === startingId)) {
				fetch('http://localhost:18080/stadiums')
					.then(r => r.ok ? r.json() : Promise.reject(r.status))
					.then(j => {
						const all = (j && j.stadiums) ? j.stadiums : []
						const found = all.find(x => x.stadiumId === startingId)
						if (found) {
							setDisplayStadiums(prev => {
								if (prev.find(p => p.stadiumId === found.stadiumId)) return prev
								return [found, ...prev]
							})
						}
					})
					.catch(() => {})
			}
		}, [stadiums, startingId])

		// build catalog from embedded stadium.souvenirs when available, otherwise fetch per-stadium; also fetch distances
		useEffect(() => {
			displayStadiums.forEach(s => {
				
					fetch(`http://localhost:18080/stadiums/${s.stadiumId}/souvenirs`)
						.then(r => r.ok ? r.json() : Promise.reject(r.status))
						.then(j => {
							const list = (j.souvenirs || []).map(it => ({ id: it.souvenirId, name: it.souvenirName, price: Number(it.souvenirPrice) }))
							setCatalog(prev => ({ ...prev, [s.stadiumId]: list }))
							setSouvenirs(prev => ({ ...prev, [s.stadiumId]: prev[s.stadiumId] || [] }))
						})
						.catch(() => {})
			})

			if (displayStadiums.length > 1) {
				const ids = displayStadiums.map(s => s.stadiumId)
				fetch('http://localhost:18080/trip/distances', {
					method: 'POST',
					headers: { 'Content-Type': 'application/json' },
					body: JSON.stringify({ stadiumIds: ids })
				})
					.then(r => r.ok ? r.json() : Promise.reject(r.status))
					.then(j => {
						if (j.total !== undefined) {
							let legs = j.legs || []
							const updated = displayStadiums.map((s, i) => ({ ...s, distanceToNext: (i < legs.length ? legs[i] : 0) }))
							sessionStorage.setItem('lastTripWithDistances', JSON.stringify({ stadiums: updated }))
						}
					})
					.catch(() => {})
			}
		}, [displayStadiums])

	function setSouvenirQuantity(stadiumId, souvenirId, name, price, quantity) {
		setSouvenirs(prev => {
			const cur = (prev[stadiumId] || []).filter(it => it.souvenirId !== souvenirId || it.custom)
			// add/update the item if quantity > 0
			if (quantity > 0) {
				cur.push({ souvenirId, name, price, quantity, custom: false })
			}
			return { ...prev, [stadiumId]: cur }
		})
	}

	const totals = useMemo(() => {
		const perStadium = {}
		let grand = 0
		Object.keys(souvenirs).forEach(id => {
			const sum = (souvenirs[id] || []).reduce((a, b) => a + (Number(b.price || 0) * Number(b.quantity || 0)), 0)
			perStadium[id] = sum
			grand += sum
		})
		return { perStadium, grand }
	}, [souvenirs])

	// distance calculation: try to use stadium.distanceToNext if present, otherwise unavailable
	const totalDistance = useMemo(() => {
		// try to read cached computed distances
		try {
			const saved = JSON.parse(sessionStorage.getItem('lastTripWithDistances') || '{}')
			if (saved && saved.stadiums) {
				let d = 0
				for (let i = 0; i < saved.stadiums.length - 1; ++i) {
					const s = saved.stadiums[i]
					if (s.distanceToNext !== undefined && s.distanceToNext !== null) d += Number(s.distanceToNext || 0)
					else { d = null; break }
				}
				return d
			}
		} catch (e) {}
		return null
	}, [displayStadiums])

	if (!displayStadiums || displayStadiums.length === 0) {
		return (
			<div style={{ padding: '1rem' }}>
				<h2>Summary</h2>
				<p>No trip data found. Go back to <button onClick={() => navigate('/trip')}>Create Trip</button>.</p>
			</div>
		)
	}

	return (
		<div style={{ display: 'flex', flexDirection: 'column', gap: '1rem', padding: '1rem' }}>
			<div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
				<h2>Trip Summary</h2>
				<div style={{ textAlign: 'right' }}>
					<div><strong>Total spent:</strong> {currency(totals.grand)}</div>
					<div><strong>Total distance:</strong> {totalDistance === null ? 'N/A' : `${totalDistance} mi`}</div>
				</div>
			</div>

			{/* Starting point display */}
			{startingId && (
				<div style={{ padding: '0.5rem 0', color: 'var(--muted)' }}>
					<strong>Starting Point:</strong> {(() => {
						const s = displayStadiums.find(x => x.stadiumId === startingId)
						return s ? `${s.teamName} — ${s.stadiumName}` : 'Unknown'
					})()}
				</div>
			)}

			<div style={{ display: 'flex', gap: '1rem' }}>
				{/* Left: teams list */}
				<div style={{ width: '320px', border: '1px solid var(--border)', borderRadius: 8, padding: '0.75rem', background: 'var(--panel)' }}>
					<div style={{ fontWeight: 700, marginBottom: '0.5rem' }}>Teams</div>
					<div style={{ display: 'grid', gap: '0.5rem' }}>
						{displayStadiums.map(s => (
							<div key={s.stadiumId} onClick={() => setActiveId(s.stadiumId)} style={{ padding: '0.5rem', borderRadius: 6, cursor: 'pointer', background: activeId === s.stadiumId ? 'rgba(0,0,0,0.06)' : 'transparent' }}>
								<div style={{ display: 'flex', alignItems: 'center', gap: '0.5rem' }}>
									<div style={{ fontWeight: 600 }}>{s.teamName}</div>
									{startingId === s.stadiumId && (
										<div style={{ fontSize: '0.75rem', color: 'var(--accent)', padding: '0.15rem 0.5rem', border: '1px solid var(--accent)', borderRadius: 4 }}>Starting</div>
									)}
								</div>
								<div style={{ fontSize: '0.9rem', color: 'var(--muted)' }}>{s.stadiumName}</div>
								<div style={{ fontSize: '0.8rem', color: 'var(--muted)' }}>{s.location}</div>
								<div style={{ fontSize: '0.8rem', color: 'var(--muted)', marginTop: '0.25rem' }}><strong>Spent:</strong> {currency(totals.perStadium[s.stadiumId] || 0)}</div>
							</div>
						))}
					</div>
				</div>

				{/* Right: souvenirs for active team */}
				<div style={{ flex: 1, border: '1px solid var(--border)', borderRadius: 8, padding: '0.75rem', background: 'var(--panel)' }}>
					<div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
						<div style={{ fontWeight: 700 }}>{displayStadiums.find(s => s.stadiumId === activeId)?.teamName || 'Select a team'}</div>
						<div style={{ fontSize: '0.9rem', color: 'var(--muted)' }}>Spent at this stadium: {currency(totals.perStadium[activeId] || 0)}</div>
					</div>

					{activeId ? (
						<SouvenirEditor
							stadium={displayStadiums.find(s => s.stadiumId === activeId)}
							items={souvenirs[activeId] || []}
							catalog={catalog[activeId] || []}
							onQuantityChange={(id, qty, name, price) => setSouvenirQuantity(activeId, id, name, price, qty)}
							onRemove={(i) => removeSouvenir(activeId, i)}
						/>
					) : (
						<div style={{ marginTop: '1rem', color: 'var(--muted)' }}>Click a team on the left to view or add souvenirs.</div>
					)}
				</div>
			</div>
		</div>
	)
}

function SouvenirEditor({ stadium, items, catalog, onQuantityChange, onAddCustom, onRemove }) {


	return (
		<div style={{ marginTop: '1rem' }}>
			<div style={{ marginBottom: '0.5rem' }}><strong>Souvenirs at {stadium?.stadiumName || stadium?.teamName}</strong></div>
			<div style={{ display: 'grid', gap: '0.5rem' }}>
				{/* Always show catalog items (names and prices) even when quantities/cart are empty */}

				{catalog.map((it) => {
					const selected = (items || []).find(x => x.souvenirId === it.id && !x.custom)
					const quantity = selected ? selected.quantity : 0
					return (
						<div key={it.id} style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
							<div>
								<div style={{ fontWeight: 600 }}>{it.name}</div>
								<div style={{ fontSize: '0.85rem', color: 'var(--muted)' }}>{currency(it.price)}</div>
							</div>
							<div style={{ display: 'flex', gap: '0.5rem', alignItems: 'center' }}>
								<input type='number' min={0} value={quantity} onChange={(e) => onQuantityChange(it.id, Number(e.target.value), it.name, it.price)} style={{ width: 80 }} />
							</div>
						</div>
					)
				})}

				{items.filter(i => i.custom).map((it, i) => (
					<div key={`custom-${i}`} style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
						<div>
							<div style={{ fontWeight: 600 }}>{it.name}</div>
							<div style={{ fontSize: '0.85rem', color: 'var(--muted)' }}>{currency(it.price)} x {it.quantity} = {currency(it.price * it.quantity)}</div>
						</div>
						<div>
							<button onClick={() => onRemove(i)} style={{ cursor: 'pointer' }}>Remove</button>
						</div>
					</div>
				))}

				{/* Names and prices come from the server-provided `catalog` rendered above. No custom-item inputs. */}
			</div>
		</div>
	)
}