import { useState } from "react";

function SearchBar({ searchInput, setSearchInput }) {
    // FUNCTIONALITY

    // STYLES
    const searchBarStyle = {
        display: 'flex',
        alignItems: 'center',
        border: 'var(--border)',
        borderRadius: '4px',
        padding: '0.5em',
        color: 'var(--purple)',
        backgroundColor: 'var(--white)',
    };

    // DISPLAY
    return (
        <div>
            <input 
                type="text"
                placeholder="Search teams..." 
                className="search-bar"
                style={searchBarStyle} 
                value={searchInput}
                onChange={(e) => setSearchInput(e.target.value)}
            />
        </div>
    );
}

export default SearchBar;