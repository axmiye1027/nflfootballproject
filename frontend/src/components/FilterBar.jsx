// Filter Bar Selection
function FilterBar({ filter, setFilter }) {
    return (
        <div className="filter-bar">
            <label style={{ marginRight: '0.5em' }}>Conference:</label>
            <select value={filter} onChange={(e) => setFilter(e.target.value)}>
                <option value="">All Teams</option>
                <option value="AFC">AFC</option>
                <option value="NFC">NFC</option>
                <option value="AFC">AFC</option>
                <option value="NFC">NFC</option>
            </select>
        </div>
    );
}