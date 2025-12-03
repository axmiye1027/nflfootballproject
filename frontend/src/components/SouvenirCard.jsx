// Displaying basic team information card
// Will modify later to accept props for dynamic data

import React, { useState, useEffect } from 'react'

import '../styles/souvenirCard.css'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faHorseHead }     from '@fortawesome/free-solid-svg-icons'
    
function SouvenirCard({ stadium, onSubtotalChange }) {
    if (!stadium) return null;

    // Track quantity per souvenir
    const [quantities, setQuantities] = useState(() =>
        stadium.souvenirs?.map(() => 0) || []
    );

    useEffect(() => {
        const subtotal = stadium.souvenirs.reduce(
            (total, s, i) => total + s.souvenirPrice * quantities[i],
            0
        );
        if (onSubtotalChange) onSubtotalChange(subtotal);
    }, [quantities, stadium.souvenirs, onSubtotalChange]);

    const handleChange = (index, value) => {
        const newQuantity = Math.max(0, Number(value) || 0); // ensure it's a number and ≥ 0
        setQuantities(prev => {
            const newQuantities = [...prev];
            newQuantities[index] = newQuantity;
            return newQuantities;
        });
    };

    const subtotal = stadium.souvenirs.reduce(
        (total, s, i) => total + s.souvenirPrice * quantities[i],
        0
    );

    // STYLE
    const cardContainerStyle = {
        display: 'flex',
        flexWrap: 'wrap',
        gap: '1em',
        justifyContent: 'center',
        alignItems: 'stretch',
        padding: '2em',
        width: '100%',
        boxSizing: 'border-box',
    };

    // card lengths
    const cardWidth = {
        flex: '0 0 calc(23% - 2em)',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
    };

    // horse icon
    const iconStyle = {
        padding: '0.5em',
        borderRadius: '50%', 
        border: 'var(--border)',
    };

    // border between icons
    const dividerStyle = {
        width: '100%',
        borderTop: 'var(--border)',
    };

    // DISPLAY
    return (
        <div style={cardWidth}>

            { /* TOP */}
            <div className="row" style={{ gap: '0em' }}>
                <FontAwesomeIcon icon={faHorseHead} className="bt-0 bl-0" style={iconStyle} />
                <div style={dividerStyle}></div>
                <FontAwesomeIcon icon={faHorseHead} className="bt-0 br-0" style={iconStyle} />
            </div>

            { /* MIDDLE - INFO CARD */}
            <div className="infoCardStyle">
                <h2>{stadium.stadiumName}</h2>
                <p>{stadium.teamName}</p>
                <p>Souvenirs:</p>
                <ul>
                    {stadium.souvenirs?.map((souvenir, index) => (
                        <li key={index} className="souvenir-row">
                            {souvenir.souvenirName} — ${souvenir.souvenirPrice.toFixed(2)}

                            <div className="quantity-control">

                                {/* <button onClick={() => handleChange(index, quantities[index] - 1)}>-</button> */}

                                <input
                                    type="number"
                                    value={quantities[index]}
                                    min={0}
                                    onChange={e => handleChange(index, e.target.value)}
                                />

                                {/* <button onClick={() => handleChange(index, quantities[index] + 1)}>+</button> */}


                            </div>
                        </li>
                    ))}
                </ul>
                <div className="subtotal">
                    Subtotal: ${subtotal.toFixed(2)}
                </div>
            </div>

            { /* BOTTOM */}
            <div className="row" style={{ gap: '0em' }}>
                <FontAwesomeIcon icon={faHorseHead} className="bb-0 bl-0" style={iconStyle} />
                <div style={dividerStyle}></div>
                <FontAwesomeIcon icon={faHorseHead} className="bb-0 br-0" style={iconStyle} />
            </div>
        </div>
    ); // END RETURN
}

export default React.memo(SouvenirCard);