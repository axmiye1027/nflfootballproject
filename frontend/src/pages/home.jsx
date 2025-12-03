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
    surfaces:  [],
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

  
  // NEW - roof type count - ONLY unique staidums
  //const [roofCount, setRoofCount] = useState(0);

  // useEffect(() => {
  //   const roofType = filters.roofTypes[0] || ""; // first selected roof or empty for "All"

  //   const fetchRoofCount = async () => {
  //     try {
  //       let url = "http://localhost:18080/stadiums/count";
  //       if (roofType) {
  //         url += `?roofTypes=${encodeURIComponent(roofType)}`;
  //       }
  //       const res = await fetch(url);
  //       const data = await res.json();
  //       setRoofCount(data.count); // update count
  //     } catch (err) {
  //       console.error("Failed to fetch roof count:", err);
  //     }
  //   };

  //   fetchRoofCount();
  // }, [filters.roofTypes]);
  // Roof count (unique stadium names)
// useEffect(() => {
//   const roofType = filters.roofTypes[0] || "";
//   if (!roofType) return setRoofCount(0);

//   const fetchRoofCount = async () => {
//     const res = await fetch(`http://localhost:18080/stadiums/count?roofType=${roofType}`);
//     const data = await res.json();
//     // count unique stadium names from returned data
//     const uniqueNames = new Set(data.stadiums.map(s => s.stadiumName));
//     setRoofCount(uniqueNames.size);
//   };

//   fetchRoofCount();
//   }, [filters.roofTypes]);
// NEW - roof type count (unique stadiums)
const roofCount = useMemo(() => {
  if (!filters.roofTypes[0]) return 0; // no roof selected

  // Keep only unique stadiums by name
  const uniqueStadiums = Array.from(
    new Map(stadiums.map(s => [s.stadiumName, s])).values()
  );

  // Filter unique stadiums by the selected roof type
  return uniqueStadiums.filter(
    stadium => stadium.roofType === filters.roofTypes[0]
  ).length;
}, [stadiums, filters.roofTypes]);





  //NEW - total seating capacity
  // const [totalCapacity, setTotalCapacity] = useState(0);
  // useEffect(() => {
  // if (sortBy === "capacity") {
  //   const sum = stadiums.reduce((acc, stadium) => acc + (stadium.capacity || 0), 0);
  //   setTotalCapacity(sum);
  // } else {
  //   setTotalCapacity(0); // reset when not sorting by capacity
  // }
  // }, [stadiums, sortBy]);

  // const totalCapacity = useMemo(() => {
  //   return stadiums.reduce((acc, stadium) => acc + (stadium.capacity || 0), 0);
  // }, [stadiums]);

  // NEW - total seating capacity ONLY unique stadium names
  const totalCapacity = useMemo(() => {
  const uniqueStadiums = Array.from(new Map(stadiums.map(s => [s.stadiumName, s])).values());
  return uniqueStadiums.reduce((acc, stadium) => acc + (stadium.capacity || 0), 0);
}, [stadiums]);



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

     {/* DISPLAY ROOF COUNT */}
      <div className="roof-count" style={{ margin: "20px", textAlign: "center" }}>
        <p>Number of {filters.roofTypes[0] || "All"} Roof Stadiums: {roofCount}</p>
      </div>

     {/* DISPLAY TOTAL SEATING CAPACITY (old - only displays values when filtering by capacity*/}
      {/* {sortBy === "capacity" && (
      <div style={{ margin: "20px", textAlign: "center" }}>
        <p>Total Seating Capacity: {totalCapacity.toLocaleString()}</p>
      </div>
      )} */}

    {/* NEW - DISPLAY TOTAL SEATING CAPACITY */}
    <div style={{ margin: "20px", textAlign: "center" }}>
      <p>Total Seating Capacity: {totalCapacity.toLocaleString()}</p>
    </div>

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
