// Displaying basic team information card
import React, { useEffect, useState } from 'react'
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faHorseHead } from '@fortawesome/free-solid-svg-icons'


function TeamInfo() {
    const [stadiums, setStadiums] = useState([])
    const [loading, setLoading] = useState(true)
    const [error, setError] = useState(null)

    useEffect(() => {
        const url = 'http://localhost:18080/stadiums'
        console.log('TeamInfo: fetching', url)
        fetch(url)
            .then((res) => {
                if (!res.ok) throw new Error(`HTTP ${res.status}`)
                return res.json()
            })
            .then((data) => {
                setStadiums(data.stadiums || [])
                setLoading(false)
            })
            .catch((err) => {
                console.error('TeamInfo fetch error:', err)
                setError(err.message)
                setLoading(false)
            })
    }, [])

    // STYLES SECTION

    // overall container of team info
    const cardContainerStyle = {
        display: 'flex',
        flexWrap: 'wrap',
        gap: '1em',
        justifyContent: 'center',
        alignItems: 'stretch',
        padding: '2em',
        width: '100%',
        boxSizing: 'border-box',
    }

    // card width and height
    const cardWidth = {
        flex: '0 0 calc(23% - 2em)',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
    }

    // horse icon appearance
    const iconStyle = {
        padding: '0.5em',
        borderRadius: '50%',
        border: 'var(--border)',
    }

    const dividerStyle = {
        width: '100%',
        borderTop: 'var(--border)',
    }

    // inside of card - where data will go
    const infoCardStyle = {
        width: 'calc(100% - 4em)',
        height: 'calc(100% - 4em)',
        overflowY: 'auto',
        overflowX: 'hidden',
        padding: '1em',
        marginLeft: '.5em',
        marginRight: '.5em',
        borderRight: 'var(--border)',
        borderLeft: 'var(--border)',
    }

    if (loading) return <p>Loading stadiums...</p>
    if (error) return <p>Error loading stadiums: {error}</p>
    if (!loading && stadiums.length === 0) return <p>No stadiums found</p>

    return (
        <div style={cardContainerStyle}>
            {stadiums.map((stadium) => (
                <div id={`stad-${stadium.stadiumId}`} key={stadium.stadiumId} style={cardWidth}>
                    <div className="row" style={{ gap: '0em' }}>
                        <FontAwesomeIcon icon={faHorseHead} className="bt-0 bl-0" style={iconStyle} />
                        <div style={dividerStyle}></div>
                        <FontAwesomeIcon icon={faHorseHead} className="bt-0 br-0" style={iconStyle} />
                    </div>

                    <div style={infoCardStyle}>
                        <h2>{stadium.teamName}</h2>
                        <p><b>Stadium:</b> {stadium.stadiumName}</p>
                        <p><b>Location:</b> {stadium.location}</p>
                        <p><b>Capacity:</b> {stadium.capacity?.toLocaleString()}</p>
                    </div>

                    <div className="row" style={{ gap: '0em' }}>
                        <FontAwesomeIcon icon={faHorseHead} className="bb-0 bl-0" style={iconStyle} />
                        <div style={dividerStyle}></div>
                        <FontAwesomeIcon icon={faHorseHead} className="bb-0 br-0" style={iconStyle} />
                    </div>
                </div>
            ))}
        </div>
    )
}

export default TeamInfo