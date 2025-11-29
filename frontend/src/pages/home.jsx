// home.jsx

import React, { useEffect, useState } from 'react'

// components
import SearchBar from '../components/SearchBar.jsx'
import TeamInfo from '../components/TeamInfo.jsx'

function HomePage() {
  const [stadiums, setStadiums] = useState([]);
  const [division, setDivision] = useState("All Teams");

  const [search,      setSearch]      = useState("");
  const [searchInput, setSearchInput] = useState("");

  // Debounce: wait 400ms AFTER user stops typing
  useEffect(() => {
    const timeout = setTimeout(() => {
      setSearch(searchInput);
    }, 400);

    return () => clearTimeout(timeout);
  }, [searchInput]);


  useEffect(() => {
    const url =
      division === "All Teams"
        ? `http://localhost:18080/stadiums?search=${search}`
        : `http://localhost:18080/stadiums?division=${division}&search=${search}`;

    fetch(url)
      .then((res) => res.json())
      .then((data) => {
        setStadiums(data.stadiums || []);
      })
      .catch((err) => {
        console.error(err);
      });
  }, [division, search]);

  return (
    <div>
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

          {/* FILTERS */}
          <select
            value={division}
            onChange={(e) => setDivision(e.target.value)}
          >
            <option value="All Teams">All Teams</option>
            <option value="AFC">AFC</option>
            <option value="NFC">NFC</option>
          </select>

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
