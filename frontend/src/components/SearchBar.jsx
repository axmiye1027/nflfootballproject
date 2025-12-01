import React, { useState, useEffect } from 'react'

export default function SearchBar({ value, onChange = () => {}, onSearch = () => {}, placeholder = 'Search...', style = {}, inputId }) {
    // support both controlled (value provided) and uncontrolled (no value prop) modes
    const [internalValue, setInternalValue] = useState(value ?? '')

    useEffect(() => {
        if (value !== undefined) setInternalValue(value)
    }, [value])

    const isControlled = value !== undefined

    const searchBarStyle = {
        display: 'flex',
        gap: 8,
        alignItems: 'center',
        width: '100%',
        ...style
    }

    const inputStyle = {
        flex: 1,
        padding: '0.5rem'
    }

    function handleChange(e) {
        const v = e.target.value
        if (!isControlled) setInternalValue(v)
        try { onChange(v) } catch (err) { /* swallow */ }
    }

    const currentValue = isControlled ? value : internalValue

    return (
        <div style={searchBarStyle}>
            <input
                id={inputId}
                type='text'
                placeholder={placeholder}
                value={currentValue}
                onChange={handleChange}
                onKeyDown={(e) => { if (e.key === 'Enter') onSearch(currentValue) }}
                style={inputStyle}
            />
            <button onClick={() => onSearch(currentValue)} style={{ cursor: 'pointer' }}>Jump</button>
        </div>
    )
}