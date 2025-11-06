function SearchBar() {
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
            <input type="text"
             placeholder="Search teams..." 
             className="search-bar"
             style={searchBarStyle} />
        </div>
    );
}

export default SearchBar;