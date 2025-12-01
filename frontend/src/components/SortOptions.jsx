// SortOptions.jsx

import React from 'react';

function SortOptions({ sortBy,setSortBy }) {
  return (
    <div className="sort-options">
      <div className="sort-row">
        <label>Sort By:</label>
        <select value={sortBy} onChange={e => setSortBy(e.target.value)}>
          <option value="">None</option>
          <option value="teamName">Team Name</option>
          <option value="stadiumName">Stadium Name</option>
          <option value="capacity">Capacity</option>
          <option value="yearOpened">Year Opened</option>
        </select>
      </div>

      {/* <div className="sort-row">
        <label>Order:</label>
        <select value={sortDir} onChange={e => setSortDir(e.target.value)}>
          <option value="asc">Ascending</option>
          <option value="desc">Descending</option>
        </select>
      </div> */}
    </div>
  );
}

export default React.memo(SortOptions);
