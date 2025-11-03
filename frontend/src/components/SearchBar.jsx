function SearchBar() {
    // FUNCTIONALITY

    // STYLES
    const searchBarStyle = {
        display: 'flex',
        alignItems: 'center',
        border: '1px solid var(--outline)',
        borderRadius: '4px',
        padding: '0.5em',
        backgroundColor: 'var(--bg)',
    };

    const searchInputStyle = {
        border: 'none',
        outline: 'none',
        flex: '1',
        padding: '0.5em',
        fontSize: '1em',
        color: 'var(--text)',
    };

    // DISPLAY
    return (
        <div className="search-bar">
            <input type="text" placeholder="Search teams..." />
        </div>
    );
}

export default SearchBar;