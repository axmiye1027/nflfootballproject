// home.jsx

import React, { useEffect, useState, useMemo } from 'react'

// components
import SearchBar    from '../components/SearchBar.jsx'
import TeamInfo     from '../components/TeamInfo.jsx'
import FiltersPanel from '../components/FiltersPanel.jsx'
import SortOptions  from '../components/SortOptions.jsx'

// CSS
import '../styles/home.css'

function HomePage({ stadiums, setStadiums }) {

  const [search,      setSearch]      = useState("");
  const [searchInput, setSearchInput] = useState("");

  const [filters, setFilters] = useState({
    conference: [],
    divisions: [], 
    roofTypes: [],
    surfaceType: "", 
    capacity: "",
    yearOpened: "",
  });

  // sorting
  const [sortBy, setSortBy] = useState("teamName"); // teamName by default

  const updateFilter = (key, value) => {
    setFilters(prev => ({ ...prev, [key]: value }));
  };

  useEffect(() => {
    const t = setTimeout(() => setSearch(searchInput.trim()), 300);
    return () => clearTimeout(t);
  }, [searchInput]);


  const queryString = useMemo(() => {
    const query = new URLSearchParams();

    if (search) query.append("search", search);

    Object.entries(filters).forEach(([key, value]) => {
      if (Array.isArray(value) && value.length > 0) {
        query.append(key, value.join(","));
      } else if (typeof value === "string" && value !== "") {
        query.append(key, value);
      }
    });

    if (sortBy) query.append("sortBy", sortBy);

    return query.toString();
  }, [filters, search, sortBy]);

  useEffect(() => {
    const t = setTimeout(() => {
      fetch(`http://localhost:18080/stadiums?${queryString}`)
        .then(res => res.json())
        .then(data => setStadiums(data.stadiums || []))
        .catch(console.error);
    }, 200);

    return () => clearTimeout(t);
  }, [queryString]);


  return (
    <div className="home-page">

      {/* HEADER */}
      <div className="title" style={{ 
        marginTop: '1em', 
        backgroundSize: 'cover',
        backgroundPosition: 'center',
        backgroundImage: `url('')` }}
      >
          List of Teams
      </div>

      {/* SEARCH - ICON - FILTERS */}
      <div className="row">

        {/* SEARCH BAR */}
        <SearchBar 
          searchInput={searchInput} 
          setSearchInput={setSearchInput} 
        />

        {/* LOGO */}

        {/* SORTING */}
        <SortOptions
          sortBy={sortBy}
          setSortBy={setSortBy}
        />

        {/* FILTERS */}
        <FiltersPanel 
          filters={filters} 
          updateFilter={updateFilter} />
      </div>

      {/* END SEARCH - ICON - FILTERS */}

      {/* TEAM LIST */}
      <div className="teams-container">
        <div 
            style={{ 
            display: "flex", 
            flexWrap: "wrap", 
            gap: "20px", 
            justifyContent: "center",
            marginTop: "40px",
            }}
        >
            {stadiums.map((stadium) => (
            <TeamInfo key={stadium.stadiumId} stadium={stadium}/>
            ))}
        </div>
      </div>
    </div>
  );
}

export default HomePage;
